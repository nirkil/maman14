#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_table.h"

struct macro_table* create_table() {

	struct macro_table *table = (struct macro_table*) malloc(
			sizeof(struct macro_table));
	table->row_ammount = 0;

	struct table_row *head = (struct table_row*) malloc(
			sizeof(struct table_row));

	head->next = NULL;
	table->head = head;

	return table;
}

void add_row_to_table(struct macro_table *table) {
	struct table_row *new_row = (struct table_row*) malloc(
			sizeof(struct table_row));

	struct table_row *current = table->head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = new_row;
	table->row_ammount++;
}

char* get_cel_value(struct macro_table *table, int row_num, int col_num) {
	int i;

	if (row_num
			< 1|| col_num < 0 || row_num > table->row_ammount || col_num > COL_AMMOUNT) {
		printf("error! cell get location not found");
		exit(1);
	}

	i = 1;

	struct table_row *current = table->head;

	while (current->next != NULL && i <= row_num) {
		current = current->next;
		i++;
	}

	if (current == NULL) {
		printf("error! cell get location not found");
		exit(1);
	}

	return current->row_structure[col_num - 1];
}

void set_cel_value(struct macro_table *table, int row_num, int col_num,
		char *value) {
	int i;

	if (row_num
			< 1|| col_num < 0 || row_num > table->row_ammount || col_num > COL_AMMOUNT) {
		printf("error! cell location not found");
		return;
	}

	struct table_row *current = table->head;
	i = 1;

	while (current->next != NULL && i <= row_num) {
		current = current->next;
		i++;
	}

	if (current == NULL) {
		printf("error! cell get location not found");
		exit(1);
	}

	strcpy(current->row_structure[col_num - 1], value);

}

int search_table_for_value(struct macro_table *table, char *value) {
	int row_num;
	row_num = 1;

	struct table_row *current = table->head;

	while (current->next != NULL) {
		current = current->next;
		if (strcmp(value, current->row_structure[0]) == 0) {
			return row_num;
		}
	}
	row_num++;

	return -1;

}

void destroy_table(struct macro_table *table) {
	struct table_row *current = table->head;
	struct table_row *next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	table->head = NULL;
}
