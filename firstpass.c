#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "SymbolTable.h"

bool process_line_fpass(line_info line, long *IC, long *DC, machine_word **code_img, long *data_img,  table *symbol_table) 
{  /* Processes a single line in the first pass*/
  int i, j,baseadress,offset;
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

    attrinutes = find_attrinutes_from_index(line, &i); /* Check if it's an attrinutes (starting with '.') */

	if (attrinutes == ERROR_ADDR) { /* Syntax error found */
		return FALSE;
	}
    
    if (attrinutes != NONE_ADDR) 
    {
		/* if .string or .data, and symbol defined, put it into the symbol table */
		if ((instruction == DATA_INST || instruction == STRING_INST) && symbol[0] != '\0')
			add_table_item(symbol,*DC,base ,offset, attrinutes,symbol_table); /* is data or string, add DC with the symbol to the table as data */
    }

    	if (instruction == STRING_INST)/* if string then encode into data image buffer and increase dc */
			return process_string_instruction(line, i, data_img, DC);
				else if (instruction == DATA_INST)/* if .data encode into data image buffer and increase dc. */
			return process_data_instruction(line, i, data_img, DC);

    else if (instruction == EXTERN_ADDR) {
			MOVE_TO_NOT_WHITE(line.content, i)
			/* if external symbol detected, start analyzing from it's deceleration end */
			for (j = 0; line.content[i] && line.content[i] != '\n' && line.content[i] != '\t' && line.content[i] != ' ' && line.content[i] != EOF; i++, j++) {
				symbol[j] = line.content[i];
			}
			symbol[j] = 0;
			/* If invalid external label name, it's an error */
			if (!is_valid_label_name(symbol)) {
				printf_line_error(line, "Invalid external label name: %s", symbol);
				return TRUE;
			}
			add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL); /* Extern value is defaulted to 0 */
		}

      
