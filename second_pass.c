#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "utils.h"
#include "SymbolTable.h"

void second_pass_handle(FILE *input_file, machine_code assembler_table,
		table symbol_table, char *input_file_name) {

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

				else if (strcmp(word, ".entry")) { /*checks to see if it's entry*/
					char *symbol = (char*) malloc(sizeof(char));
					symbol = get_entry_symbol(line_copy);
					add_entry_to_symbol_table(symbol_table, symbol);
					break;
				}

				else {
					complete_assembler_table(assembler_table, symbol_table, word); /*TODO: <-- */
				}

				word = strtok(NULL, " \t");
			}
		}

	}
	char *input_name = (char*) malloc(sizeof(char));
	input_name = cut_input_file_name_ending(input_file_name); /*clear '.as' ending from input file name*/

	if (is_entey(input_file)) { /*create and fill entry output file*/
		FILE *entries_output_file;
		char *input_name_ent = (char*) malloc(sizeof(char));
		strcpy(input_name_ent, input_name);
		strcat(input_name_ent, ".ent"); /*add '.ent' ending to file name*/
		entries_output_file = fopen(input_name_ent, "w");
		fill_entries_output_file(symbol_table, entries_output_file);
		fclose(entries_output_file);
	}

	if (is_external(input_file)) { /*create and fill ext output file*/
		FILE *extern_output_file;
		char *input_name_ext = (char*) malloc(sizeof(char));
		strcpy(input_name_ext, input_name);
		strcat(input_name_ext, ".ext"); /*add '.ext' ending to file name*/
		extern_output_file = fopen(input_name_ext, "w");
		fill_externals_output_file(assembler_table, extern_output_file);
		fclose(extern_output_file);
	}

	/*create and fill object output file*/
	FILE *object_output_file;
	char *input_name_ob = (char*) malloc(sizeof(char));
	strcpy(input_name_ob, input_name);
	strcat(input_name_ob, ".ob"); /*add '.ob' ending to file name*/
	object_output_file = fopen(input_name_ob, "w");
	fill_object_output_file(assembler_table, object_output_file);
	fclose(object_output_file);
}

void add_entry_to_symbol_table(table symbol_table, char *symbol) {
	table *current = symbol_table;
	while (strcmp(current != NULL && current->key, symbol) != 0) {
		current = current->next;
	}
	if (current == NULL) {
		printf("ERROR! symbol: %s not found in symbol table", symbol);
		break;
	} else {
		/*TODO: enter entry to attribute*/
	}
}

int is_symbol(table *symbol_table, char *word){
	table *current = symbol_table;
	while(current != NULL){
		if(strcmp(current->key, word) == 0){
			return 1;
		}
		current = current->next;
	}
	return 0;
}

void complete_assembler_table(machine_code assembler_table, table symbol_table) { /*TODO: <--finnish */
	struct ext_list ext_list;
	char symbol[MAX_LINE_LENGTH];
	long symbol_value;

	symbol_value = find_symbol_value(symbol_table, symbol);

	if(is_this_external(symbol_table, symbol)){ /*TODO: make function*/
		add_to_ext_list(ext_list, symbol);
	}
}

char* get_entry_symbol(char *line) { /*TODO: check function */
	char *word = (char*) malloc(sizeof(char));
	word = strtok(line, " \t");
	word = strtok(NULL, " \t");
	word = strtok(line, " \t");
	return word;
}

int is_entry(FILE input_file) { /*check if there are entries in input file*/
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

int is_external(FILE input_file) { /*check if there are externals in input file*/
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

char* cut_input_file_name_ending(char *input_file_name) { /*clear '.as' ending from input file name*/
	char *input_name = (char*) malloc(sizeof(char));
	strcpy(input_name, input_file_name);
	strtok(input_name, ".as");
	return input_name;
}

void add_to_ext_list(struct ext_list *list, char *symbol){
	long address;
	struct ext_list current = (char*) malloc(sizeof(char));
	current = list->ext_symbol;

	address = culc_address(symbol); /*TODO: make function*/

	while(current->next != NULL){
		current = current->next;
	}
	strcpy(current->next, symbol);
	current = current->next;
	current->address = address;
	current->next = NULL;
}

void fill_entries_output_file(table symbol_table, FILE *entries_output_file) {/*TODO: <-- */
	table *current = symbol_table;
	while (current != NULL && current->type == "entry") {/*TODO: check how it works exactly */
		fprintf(entries_output_file, "%s,", current->key);
		fprintf(entries_output_file, "%s,", current->baseadress);
		fprintf(entries_output_file, "%s\n", current->offset);

		current = current->next;
	}
}

void fill_externals_output_file(table symbol_table, struct ext_list *list, FILE *externals_output_file) {/*TODO: <-- */
	struct ext_list ext_list = list;
	table symbol_t = symbol_table;
	long base_address;
	long offset;

	while(ext_list != NULL){
		fprintf(externals_output_file, "%s BASE ", ext_list->ext_symbol);
		base_address = get_ext_symbol_base(ext_list->ext_symbol, symbol_t); /*TODO: make function*/
		fprintf(externals_output_file, "%ld\n", base_address);
		fprintf(externals_output_file, "%s OFFSET ", ext_list->ext_symbol);
		offset = get_symbol_offset(ext_list->ext_symbol, symbol_t);  /*TODO: make function*/
		fprintf(externals_output_file, "%ld\n", offset);

		ext_list = ext_list->next;
	}

}

void fill_object_output_file(machine_code assembler_table, FILE *object_output_file) {/*TODO: <-- */

}


