  /* Helper functions to process and analyze code */
#ifndef _MACHINECODE_H
#define _MACHINECODE_H
#include "SymbolTable.h"
#include "globals.h"
#include "utils.h"
  
/*Separates the operands and puts each operand into the array and found operand count */
bool analyze_operands(line_info line, int i, char **destination, int *operand_count, char *command);

/* Detects the opcode and the funct of a command by it's name*/
void get_opcode_func(char* cmd, opcode *opcode_out, funct *funct_out);

/* Returns the addressing type of an operand*/
addressing_type get_addressing_type(char *operand);

/* Builds a first line code by the opcode*/
first_line_code *get_first_line_code(line_info line, opcode curr_opcode,)

/* Validates and Builds a second line code by the funct, operand count and operand strigs*/
second_line_code *get_second_line_code(line_info line, opcode curr_opcode, funct curr_funct, int op_count,bool check_extern_symbol,char *operands[2]) 

reg get_register_by_name(char *name);/* Returns the register enum value by its name*/

data_digit *build_data_line(addressing_type addressing, long data, bool is_extern_symbol)/* Builds a data word by the operand's addressing type, value and whether the symbol (if it is one) is external.*/

