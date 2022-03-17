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

				else if (strcmp(word, ".entry")) {
					char *symbol = (char*) malloc(sizeof(char));
					symbol = get_entry_symbol(line_copy);
					add_entry_to_symbol_table(symbol_table, symbol); /*TODO: <-- */
					break;
				}

				else {
					complete_assembler_table(assembler_table, symbol_table); /*TODO: <-- */
				}

				word = strtok(NULL, " \t");
			}
		}

	}

	if (is_entey(input_file)) {
		FILE *entries_output_file;
		entries_output_file = fopen("entries.ent", "w");
		fill_entries_output_file(symbol_table, entries_output_file);
		fclose(entries_output_file);
	}

	if (is_external(input_file)) {
		FILE *extern_output_file;
		extern_output_file = fopen("externals.ext", "w");
		fill_externals_output_file(assembler_table, entries_output_file);
		fclose(extern_output_file);
	}

	FILE *object_output_file;
	object_output_file = fopen("object.ob", "w");
	fill_object_output_file(assembler_table, object_output_file);
	fclose(object_output_file);
}

void add_entry_to_symbol_table(char *word) {
	/*add_table_item(symbol,*DC,base ,offset, attrinutes,symbol_table);
	 * add_table_item(symbol_table, symbol, 0, EXTERNAL_SYMBOL)
	 *  TODO: delete*/
	add_table_item();
}

void complete_assembler_table( assembler_table, symbol_table) {

}

char* get_entry_symbol(char *line){ /*TODO: check function */
	char *word = (char*) malloc(sizeof(char));
	word = strtok(line, " \t");
	word = strtok(NULL, " \t");
	word = strtok(line, " \t");
	return word;
}

int is_entey(FILE input_file) {
	char *word;
	char line[MAX_LINE_LENGTH];

	while (feof(input_file) == 0) {
		if (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
			if (line[0] != ';' && line[0] != '\n') {
				word = strtok(line, " \t");
				while (word != NULL) {
					if (strcmp(word, ".extrn") == 0) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

int is_external(FILE input_file) {
	char *word;
	char line[MAX_LINE_LENGTH];

	while (feof(input_file) == 0) {
		if (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
			if (line[0] != ';' && line[0] != '\n') {
				word = strtok(line, " \t");
				while (word != NULL) {
					if (strcmp(word, ".entry") == 0) {
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void fill_entries_output_file(symbol_table, FILE *entries_output_file) {/*TODO: <-- */

}

void fill_externals_output_file(assembler_table, FILE *externals_output_file) {/*TODO: <-- */

}

void fill_object_output_file(assembler_table, FILE *object_output_file) {/*TODO: <-- */
}


/* TODO: delete
int main(void) {
	puts("!!!Hello World!!!");
	return EXIT_SUCCESS;
}*/

