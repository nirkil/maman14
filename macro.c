/*
 ============================================================================
 Name        : macro.c
 Author      : Noga
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "table.c"

void check_line_length(FILE *input_file, char *line);

FILE* handle_file(FILE *input_file, FILE *new_file) {

	struct table* macro_table;
	char *word;
	char line[MAX_LINE_LENGTH + ADDED_LINE_LENGTH];
	char *macro_name;
	char *macro_def;

	*macro_table = create_table(0, 2);

	while (!feof(input_file)) {
		if (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {

			check_line_length(input_file, line);

			if (line[0] != ';' && line[0] != '\n') { /*ignore comments: ; and empty lines*/
				word = strtok(line, " \t");

				while (word != NULL) {
					if (macro_flag == 0) { /*compare to macro table*/
						macro_def = search_macro_name(macro_table, word); /* macro_def = NULL only if word is not a macro name*/
						if (macro_def != NULL) {
							add_to_new_file(new_file, macro_def);
							continue;
						}
					}

					else if (macro_flag == 0 && is_it_macro_def_open(word)) { /*compare to MACRO_OPEN*/
						macro_flag = 1;
						macro_name = find_macro_name(line); /*returns the next word after macro open*/
						macro_def = "";
						continue;
					}

					if (macro_flag) { /**/
						if (is_it_macro_def_close(line)) { /*when macro def ends*/
							macro_flag = 0;
							add_def_to_macro_table(macro_table, macro_name,
									macro_def);
							add_to_new_file(new_file, macro_def);
							continue;
						}

						strcat(macro_def, line);
					}
					word = strtok(NULL, " \t");
				}
			}
			if (macro_flag == 0) {
				add_to_new_file(new_file, line);
			}
		}
	}
	return new_file;
}

void check_line_length(FILE *input_file, char *line) {
	char *unnecessary;
	if (strchr(line, "\n") == NULL && !feof(input_file)) {
		do {
			unnecessary = getc(input_file);
		} while (unnecessary != "\n" && unnecessary != EOF);
		strcat(line, "\n");
	}
}

void add_to_new_file(FILE *new_file, char *macro_def) {
	fprintf(new_file, "s\n", macro_def);
}

void add_def_to_macro_table(struct table *macro_table, char *macro_name, char *macro_def) {
	int row_num;
	add_row_to_table(macro_table);
	row_num = get_row_count(macro_table);
	set_cel_value(macro_table, row_num, 0, macro_name);
	set_cel_value(macro_table, row_num, 1, macro_def);
}

char* find_macro_name(char *line) {
	char *macro_name;
	strtok(line, " ");
	macro_name = strtok(line, " ");

	return macro_name;
}

char* search_macro_name(struct table *macro_table, char *macro_name) {
	char * macro_def;
	int row_num;
	row_num = search_for_value(macro_table, macro_name);
	if (row_num == -1) {
		macro_def = NULL;
	} else {
		macro_def = get_cel_value(macro_table, row_num, 1);
	}
	return macro_def;
}

int is_it_macro_def_open(char *word) {
	if (strcmp(word, MACRO_OPEN)) {
		return 1;
	}
	return 0;
}

int is_it_macro_def_close(char *line) {
	char *word;
	word = strtok(line, " \t\n");
	while (word != NULL) {
		if (strcmp(word, MACRO_CLOSE)) {
			return 1;
		}
		word = strtok(NULL, " \t\n");
	}
	return 0;
}



