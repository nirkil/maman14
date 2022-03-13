#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "SymbolTable.h"

void second_pass_handle(FILE *input_file) {

	char line[MAX_LINE_LENGTH];
	char line_copy[MAX_LINE_LENGTH];
	char *word;

	while (feof(input_file) == 0) {
		if (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
			strcpy(line_copy, line);

			word = strtok(line_copy, " \t");

			while (word != NULL) {
				if (is_symbol(word)) { /*move to next word in row*/
					break;
				}

				else if (strcmp(word, ".data") || strcmp(word, ".string")
						|| strcmp(word, ".extrn")) { /*skip entire row*/
					line_copy = '\0';
					break;
				}

				else if (is_entry(word)) {
					add_entry_to_symbol_table(word);
					break;
				}

				else {
					complete_assembler_table();
				}

				word = strtok(NULL, " \t");
			}
		}

	}

	create_output_files();

}

void add_entry_to_symbol_table(word){

}

void complete_assembler_table(){

}

FILE* create_output_files(){

}

int main(void) {
	puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
	return EXIT_SUCCESS;
}

