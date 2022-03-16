#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "machinecode.h"
#include "utils.h"
#include "globals.h" 
  
bool analyze_operands(line_info line, int i, char **destination, int *operand_count, char *c) 
{
	int j;
	*operand_count = 0;
	dest[0] = dest[1] = NULL;
	SKIP_WHITE(line.content, i)
	if (line.content[i] == ',') 
	{
		printf_line(line, "Unexpected comma after command.");
		return FALSE; /*an error*/  
	}    

	/* Until not too many operands (max of 2) and it's not the end of the line */
	for (*operand_count = 0; line.content[i] != EOF && line.content[i] != '\n' && line.content[i];) 
	{
		if (*operand_count == 2) /* =We already got 2 operands in, We're going ot get the third! */ 
		{
			printf_line_error(line, "Too many operands for operation (got >%d)", *operand_count);
			free(dest[0]);
			free(dest[1]);
			return FALSE; /* an error  */
		}

		/* Allocate memory to save the operand */
		dest[*operand_count] = malloc_with_check(MAX_LINE_LENGTH);
		
		for (j = 0; line.content[i] && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != '\n' && line.content[i] != EOF &&
		            line.content[i] != ','; i++, j++) {
			dest[*operand_count][j] = line.content[i];
		}
		dest[*operand_count][j] = '\0';
		(*operand_count)++; /* saved another operand! */

		SKIP_WHITE(line.content, i)

		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i]) break;
		else if (line.content[i] != ',') 
		{
			printf_error(line, "Expecting ',' between operands");
			
			free(dest[0]); /* Release operands dynamically allocated memory */
			if (*operand_count > 1) 
			{
				free(dest[1]);
			}
			return FALSE;
		}
		i++;
		SKIP_WHITE(line.content, i)
		/* if there was just a comma, then (optionally) white char(s) and then end of line */
		if (line.content[i] == '\n' || line.content[i] == EOF || !line.content[i])
			printf_error(line, "Missing operand after comma.");
		
		else if (line.content[i] == ',') printf_error(line, "Multiple consecutive commas.");
		{ 
			free(dest[0]);
			if (*operand_count > 1)
			{
			  free(dest[1]);
			}
			return FALSE;
		}
	}   
	return TRUE;
}
typedef struct cmd_lookup_element
 { /* A single line in table element*/
	char *cmd;
	opcode op;
	funct fun;
  }cmd_lookup_element;
	 
	static struct cmd_lookup_element lookup_table[] = {
		{"mov", MOV_OP, NONE_FUNCT},
		{"cmp",CMP_OP, NONE_FUNCT},
		{"add",ADD_OP, ADD_FUNCT},
		{"sub",SUB_OP, SUB_FUNCT},
		{"lea",LEA_OP, NONE_FUNCT},
		{"clr",CLR_OP, CLR_FUNCT},
		{"not",NOT_OP, NOT_FUNCT},
		{"inc",INC_OP, INC_FUNCT},
		{"dec",DEC_OP, DEC_FUNCT},
		{"jmp",JMP_OP, JMP_FUNCT},
		{"bne",BNE_OP, BNE_FUNCT},
		{"jsr",JSR_OP, JSR_FUNCT},
		{"red",RED_OP, NONE_FUNCT},
		{"prn",PRN_OP, NONE_FUNCT},
		{"rts",RTS_OP, NONE_FUNCT},
		{"stop",STOP_OP, NONE_FUNCT},
		{NULL, NONE_OP, NONE_FUNCT}
};
void get_opcode_func(char *cmd, opcode *opcode_out, funct *funct_out) {
	struct cmd_lookup_element *e;
	*opcode_out = NONE_OP;
	*funct_out = NONE_FUNCT;
	/* iterate through the lookup table, if commands are same return the opcode of found. */
	for (e = lookup_table; e->cmd != NULL; e++) {
		if (strcmp(e->cmd, cmd) == 0) {
			*opcode_out = e->op;
			*funct_out = e->fun;
			return;
		}
	}
}

addressing_type get_addressing_type(char *operand) { /* Returns the addressing type of an operand*/
	
	if (operand[0] == '\0') return none_addr; /* if nothing, just return none */

	/* if first char is 'r', second is number in range 0-15 and third is end of string, it's a register */
	if (operand[0] == 'r' && operand[1] >= '0' && operand[1] <= '15' && operand[2] == '\0') return Register_addr;

	/* if operand starts with # and a number right after that, it's immediately addressed */
	if (operand[0] == '#' && check_int(operand + 1)) return Immediate_addr;

    /* if operand is a valid label name, then check if it is index or directed */
    if (is_valid_label(operand[0]){
         if (operand[1] =='[' && operand[2] == 'r' && operand[3] >= '0' && operand[3] <= '15' && operand[4] == ']')  return index_addr;
         else return Direct_addr;
    }

	else return none_addr;
}
 
static bool validate_op_addr(line_info line, addressing_type op1_addressing, addressing_type op2_addressing, int op1_valid_addr_count,int op2_valid_addr_count, ...) 
{/* check if any of the operand addressing type has match to any of the valid ones*/
	int i;
	bool check_valid;
	va_list list;

	addressing_type op1_valids[4], op2_valids[4];
	memset(op1_valids, none_addr, sizeof(op1_valids));
	memset(op2_valids, none_addr, sizeof(op2_valids));

	va_start(list, op2_valid_addr_count);
	
	for (i = 0; i < op1_valid_addr_count && i <= 3 ;i++) /* get the variable args and put them in both arrays (op1_valids & op2_valids) */
		op1_valids[i] = va_arg(list, int);
	for (; op1_valid_addr_count > 5; va_arg(list, int), op1_valid_addr_count--); /* Go on with stack until got all (even above limitation of 4) */
	
	for (i = 0; i < op2_valid_addr_count && i <= 3 ;i++) /* same */
		op2_valids[i] = va_arg(list, int);

	va_end(list);  

	/* Make the validation itself: check if any of the operand addressing type has match to any of the valid ones: */
	check_valid = op1_valid_addr_count == 0 && op1_addressing == none_addr;
	for (i = 0; i < op1_valid_addr_count && !check_valid; i++) 
	{
		if (op1_valids[i] == op1_addressing) 
		{
			check_valid = TRUE;
		}
	}
	    if (!check_valid) 
		{
		 printf_error(line, "Invalid addressing mode for first operand.");
		 return FALSE;
	    }
	
	check_valid = op2_valid_addr_count == 0 && op2_addressing == none_addr; 
	for (i = 0; i < op2_valid_addr_count && !check_valid; i++) {
		if (op2_valids[i] == op2_addressing) {
			check_valid = TRUE;
		}
	}
	if (!check_valid) {
		printf_error(line, "Invalid addressing mode for second operand.");
		return FALSE;
	}
	return TRUE;
}

bool validate_operand_by_opcode(line_info line, addressing_type first_addressing,addressing_type second_addressing, opcode curr_opcode, int op_count) 
{
  if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) 
    { 
	 if (op_count != 2) /* 2 operands required */
     {
	   printf_error(line, "Operation requires 2 operands (got %d)", op_count);
	   return FALSE;
	 }
		/* validate operand addressing */
	  if (curr_opcode == CMP_OP) 
	  {
	    return validate_op_addr(line, first_addressing, second_addressing, 4, 4, Immediate_addr, Direct_addr, index_addr, Register_addr, Immediate_addr, Direct_addr,index_addr,  Register_addr);
	  } 
	  else if (curr_opcode == ADD_OP || curr_opcode == MOV_OP|| curr_opcode == SUB_OP) 
	  { 
	    return validate_op_addr(line, first_addressing, second_addressing, 4, 3, Immediate_addr, Direct_addr, index_addr, Register_addr,Direct_addr, index_addr, Register_addr);
	  } 
	  else if (curr_opcode == LEA_OP)
	  {
        return validate_op_addr(line, first_addressing, second_addressing, 2, 3, Direct_addr,index_addr, Direct_addr, index_addr, Register_addr);
	  }
    }     
  else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) 
    {
	  if (op_count != 1)
	    { /* 1 operand required */
			if (op_count < 1) 
            printf_error(line, "Operation requires 1 operand (got %d)", op_count);
			return FALSE;
		}
		/* validate operand addressing */
		if (curr_opcode == RED_OP || curr_opcode == CLR_OP|| curr_opcode == NOT_OP|| curr_opcode == INC_OP|| curr_opcode == DEC_OP) 
		{ 
		  return validate_op_addr(line, first_addressing, none_addr, 3, 0, Direct_addr,index_addr, Register_addr);
		} 
		else if (curr_opcode == JMP_OP|| curr_opcode == JSR_OP|| curr_opcode == BNE_OP) 
		{
		 return validate_op_addr(line, first_addressing, none_addr, 2, 0, Direct_addr, index_addr);
		} 
	    else 
		{ /* So it's PRN */
			return validate_op_addr(line, first_addressing, none_addr, 4, 0, Immediate_addr, Direct_addr, index_addr, Register_addr);
		}
	}     
  else if (curr_opcode == STOP_OP || curr_opcode == RTS_OP)
    {
	   if (op_count > 0) 
	   {/* 0 operands exactly */
		printf_error(line, "Operation requires no operands (got %d)", op_count);
		return FALSE;
		}
	}
	return TRUE;
}


reg get_register_by_name(char *name) /* Returns the register enum value by its name*/
{
  if (name[0] == 'r' && isdigit(name[1]) && name[2] == '\0') 
   {
      int digit = name[1] - '0'; /* convert digit ascii char to actual single digit */
	  if (digit >= 0 && digit <= 15) return digit;
   }
	return NONE_REG; 

 first_line_code *get_first_line_code(line_info line, opcode curr_opcode,)
    {   /* Builds a first line code by the opcode*/
        first_line_code *firstlinecode;
        firstlinecode = (first_line_code *) malloc_with_check(sizeof(first_line_code));
        firstlinecode->opcode = curr_opcode;
        firstlinecode->ARE = 4;
        return firstlinecode;
    } 

second_line_code *get_second_line_code(line_info line, opcode curr_opcode, funct curr_funct, int op_count,bool check_extern_symbol,char *operands[2]) 
{
    second_line_code *seconedlinecode;
    /* Get addressing types and validate them: */
    addressing_type first_addressing = op_count >= 1 ? get_addressing_type(operands[0]) : none_addr;
	addressing_type second_addressing = op_count == 2 ? get_addressing_type(operands[1]) : none_addr;
	/* validate operands by opcode - on failure exit */
	    if (!validate_operand_by_opcode(line, first_addressing, second_addressing, curr_opcode, op_count))
		{
		return NULL;
		}
    seconedlinecode = ( second_line_code *) malloc_with_check(sizeof(second_line_code));
    seconedlinecode->funct = curr_funct; 
     if (addressing == Direct_addr) 
     {  
        seconedlinecode->ARE? 1 : 2; /* fix it */
	 }
     else 
    seconedlinecode->ARE = 4; /* fix it */

    /* Default values of register bits are 0 */
	 seconedlinecode->adressing_des = seconedlinecode->register_des = seconedlinecode->adressing_src = seconedlinecode->registesr_src = 0;
        /* Check if need to set the registers bits */
     if (curr_opcode >= MOV_OP && curr_opcode <= LEA_OP) 
	 { /* First Group, two operands */
		seconedlinecode->adressing_des = first_addressing;
		seconedlinecode->adressing_des = second_addressing;

     /* if it's register, set it's name in the proper locations */
		if (first_addressing == Register_addr) 
		{
			seconedlinecode->registesr_src = get_register_by_name(operands[0]);
		}
		if (second_addressing == Register_addr)
		{
			seconedlinecode->register_des = get_register_by_name(operands[1]);
	 	}	
	 }
     else if (curr_opcode >= CLR_OP && curr_opcode <= PRN_OP) 
	 {
		   seconedlinecode->adressing_des = first_addressing;
		if (first_addressing == Register_addr) 
		{
			seconedlinecode->register_des = get_register_by_name(operands[0]);
		}
	}
	return seconedlinecode;
}

data_digit *build_data_line(addressing_type addressing, long data, bool is_extern_symbol) 
{
	signed long mask; /* For bitwise operations for data conversion */
	unsigned long ARE = 4, mask_un; /* 4 = 2^2 = 1 << 2 */
	data_digit *dataline = malloc_with_check(sizeof(data_digit));

	if (addressing == Direct_addr) 
	{
		ARE = is_extern_symbol ? 1 : 2;
	}
	data_digit->ARE = ARE; 

	/* Now all left is to encode the data */
	mask = -1;
	mask_un = mask; /* both hold 11...11 */
	mask_un >>= 11; /* Now mask_un holds 0..001111....111, 11 zeros and 21 ones */
	data_digit->data = mask_un & data; /* Now just the 21 lsb bits area left and assigned to data field. */
	return data_digit;
}

	
