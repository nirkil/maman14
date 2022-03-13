#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro.h"
#include "macro_table.h"

void check_line_length(FILE *input_file, char *line);

FILE* handle_file(FILE *input_file, FILE *new_file) {

	struct macro_table* macro_t;
	char *word;
	char line[MAX_LINE_LENGTH + ADDED_LINE_LENGTH];
	char line_copy[MAX_LINE_LENGTH + ADDED_LINE_LENGTH];
	char *macro_name;
	char macro_def[MAX_CELL_LENGTH];

	macro_t = create_table(1);

	while (feof(input_file) == 0) {
		if (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {

			check_line_length(input_file, line);

			strcpy(line_copy, line);


			if (line[0] != ';' && line[0] != '\n') { /*ignore comments: ; and empty lines*/
				word = strtok(line_copy, " \t");

				while (word != NULL) {

					if (macro_flag == 0 && is_it_macro_def_open(word)) { /*compare to MACRO_OPEN*/
						macro_def[0] =  '\0';
						macro_flag = 1;
						macro_name = find_macro_name(line); /*returns the next word after macro open*/
						break;
					}

					else if (macro_flag == 0) { /*compare to macro table*/

						if (search_macro_name(macro_t, word) != NULL) {/* return NULL only if word is not a macro name else returns macro's def*/
							strcpy(macro_def, search_macro_name(macro_t, word));
							add_to_new_file(new_file, macro_def);
							line[0] = '\0';
							break;
						}
					}

					if (macro_flag && !(is_it_macro_def_open(word))
							&& strcmp(word, macro_name) != 0) { /**/
						if (is_it_macro_def_close(line)) { /*when macro def ends*/
							add_def_to_macro_table(macro_t, macro_name,
									macro_def);
							add_to_new_file(new_file, macro_def);
							macro_flag = 0;
							break;

						} else {
							strcat(macro_def, line);
						}
					}
					word = strtok(NULL, " \t");

				}
			}
			if (macro_flag == 0 && !is_it_macro_def_close(line)) {
				add_to_new_file(new_file, line);
			}
		}
	}

	return new_file;
}

void check_line_length(FILE *input_file, char *line) {
	char extra;
	char down = '\n';
	char *pt;
	pt = strchr(line, down);
	if (pt == NULL && !feof(input_file)) {
		do {
			extra = getc(input_file);
		} while (extra != down && extra != EOF);
		strcat(line, &down);
	}
}

void add_to_new_file(FILE *new_file, char *s) {
	fprintf(new_file, "%s", s);
}

void add_def_to_macro_table(struct macro_table *macro_t, char *macro_name,
		char *macro_def) {
	int row_num;
	add_row_to_table(macro_t);
	row_num = macro_t->row_ammount;
	set_cel_value(macro_t, row_num, 1, macro_name);
	set_cel_value(macro_t, row_num, 2, macro_def);
}

char* find_macro_name(char *s) {
	int n;
	int i;
	int j;
	char *macro_name = (char*) malloc(sizeof(char));

	n = strlen(s);
	i = 0;

	while (i < n && s[i] != ' ') {
		++i;
	}
	if (i == n) {
		return NULL;
	}
	++i;
	j = 0;
	while (i < n && s[i] != ' ') {
		macro_name[j] = s[i];
		++i;
		++j;
	}
	return macro_name;
}

char* search_macro_name(struct macro_table *macro_t, char *macro_name) {
	char *macro_def;
	int row_num;
	row_num = search_table_for_value(macro_t, macro_name);

	if (row_num == -1) {
		macro_def = NULL;
	} else {
		macro_def = get_cel_value(macro_t, row_num, 2);
	}

	return macro_def;
}

int is_it_macro_def_open(char *word) {
	if (strcmp(word, MACRO_OPEN) == 0) {
		return 1;
	}
	return 0;
}

int is_it_macro_def_close(char *line) {
	char line_copy[MAX_LINE_LENGTH + ADDED_LINE_LENGTH];
	char *word;

	strcpy(line_copy, line);
	word = strtok(line_copy, " \t\n");
	while (word != NULL) {
		if (strcmp(word, MACRO_CLOSE) == 0) {
			return 1;
		}
		word = strtok(NULL, " \t\n");
	}
	return 0;
}



