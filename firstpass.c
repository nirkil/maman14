#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "SymbolTable.h"

static bool process_code(line_info line, int i, long *ic, machine_code **code_img);/* Processes a single code line in the first pass*/

bool process_line_fpass(line_info line, long *IC, long *DC, machine_code **code_img, long *data_img,  table *symbol_table) 
{  /* Processes a single line in the first pass*/
  int i, j,baseadress,offset;
    attribute attribute; 
	char symbol[MAX_LINE_LENGTH];
    i = 0;
    baseadress= line.line_num/16;
    offset=line.line_num%16;

    SKIP_WHITE(line.content, i);

    if (!line.content[i] || line.content[i] == '\n' || line.content[i] == EOF || line.content[i] == ';')
		return TRUE; /* Empty/Comment line */

    while (line.content[i] != ':' && line.content[i] != EOF && i <= 80) /*find a label*/
	{
		symbol[j] = line.content[i];
		i++, j++; 
	}
	 symbol[j] = '\0'; /* End of string */

     if symbol[0] && !is_valid_label(symbol_dest) /* checking illegal name */
     {
        printf_error(line, "Illegal label name: %s", symbol);
		return FALSE;
	}

    if (!checkExist(table symbol_table, char *symbol)/* checking if the symbol already exist in the symbol table*/
    {
        printf_error(line, "Symbol %s is already defined in the symbol table", symbol);
        return FALSE;
    }
      SKIP_WHITE(line.content, i);

      if (line.content[i] == '\n') return TRUE; /* only label in this line*/

    attribute = find_attribute_from_index(line, &i); /* Check if it's an attrinutes (starting with '.') */

	if (attribute == ERROR_ATT) 
	{ /* Syntax error found */
		return FALSE;
	}

	 SKIP_WHITE(line.content, i);
    
    if (attribute != none_addr) 
    {
		/* if .string or .data, and symbol defined, put it into the symbol table */
	  if ((attribute == DATA_ATTR || attribute == STRING_ATTR) && symbol[0] != '\0')
	   add_table_item(symbol,*DC,baseadress ,offset, attribute, symbol_table); /* is data or string, add DC with the symbol to the table as data */
    

        if (attribute == STRING_ATTR)/* if string then encode into data image buffer and increase dc */
		return process_string_attribute(line, i, data_img, DC);
	    else if (attribute == DATA_ATTR)/* if .data encode into data image buffer and increase dc. */
		return  process_data_attribute(line, i, data_img, DC);

      else if (attribute == EXTERN_ATTR)
	   {
			SKIP_WHITE(line.content, i)
			/* if external symbol detected, start analyzing from it's deceleration end */
			for (j = 0; line.content[i] && line.content[i] != '\n' && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != EOF; i++, j++)
			{
				symbol[j] = line.content[i];
			}
			symbol[j] '\0'; /* End of string */
			
			if (!is_valid_label_(symbol)) 
			{ /* If invalid external label name, it's an error */
				printf_error(line, "Invalid external label name: %s", symbol);
				return TRUE;
			}
			add_table_item(symbol,0,0 ,0, external_symbol,symbol_table); /* Extern value is defaulted to 0 */
		}
         
	      else if (attribute == ENTRY_ATTR && symbol[0] != '\0')
		 {/* if entry and symbol defined, print error */
			printf_error(line, "Can't define a label to an entry instruction.");
			return FALSE;
		 }

    } 
	else 
	{
		/* if symbol defined, add it to the table for code symbol*/
		if (symbol[0] != '\0')
		   add_table_item(symbol,*IC, baseadress ,offset, code_symbol ,symbol_table);
		return process_code(line, i, IC, code_img); /* Analyze code*/
	}
  return TRUE;
}
	static bool process_code(line_info line, int i, long *ic, machine_code **code_img) 
{
	char operation[8]; /* stores the string of the current code instruction */
	char *operands[2]; /* 2 strings, each for operand */
	opcode curr_opcode; /* the current opcode and funct values */
	funct curr_funct;
    first_line_code *first_line_code;
	second_line_code *second_line_code;
	long ic_before;
	int j, operand_count;
	machine_code *code_to_write;
	 
	SKIP_WHITE(line.content, i)

	/* Until white char, end of line, or too big instruction, copy it: */
	for (j = 0; line.content[i] && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != '\n' && line.content[i] != EOF && j < 6; i++, j++) {
		operation[j] = line.content[i];
	}
	operation[j] = '\0'; /* End of string */
	/* Get opcode & funct by command name into curr_opcode/curr_funct */
	get_opcode_func(operation, &curr_opcode, &curr_funct);
    if (curr_opcode == NONE_OP) { /* check if invalid operation */
		printf_line_error(line, "Unrecognized instruction: %s.", operation);
		return FALSE; /* an error occurred */

	if (!analyze_operands(line, i, operands, &operand_count, operation))
	  { /* Separate operands and get their count */
		return FALSE;
	}

