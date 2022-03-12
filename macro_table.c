

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macro_table.h"
/*
int main(void) {
	char *val = "hello";
	char *word = "123hfhdskaieerhgstr";
	char *get_val;
	int location;
	struct macro_table* t1;
	t1 = create_table(3);
	set_cel_value(t1, 1, 1, val);
	set_cel_value(t1, 1, 2, val);
	set_cel_value(t1, 2, 1, val);
	set_cel_value(t1, 2, 2, val);
	set_cel_value(t1, 3, 1, word);
	set_cel_value(t1, 3, 2, val);
	add_row_to_table(t1);
	set_cel_value(t1, 4, 1, val);
	set_cel_value(t1, 4, 2, val);
	print_table(t1);
	get_val = get_cel_value(t1, 4, 1);
	printf("the cell value is: %s\n", get_val);
	location = search_table_for_value(t1, get_val);
	printf("the value row num: %d\n", location);
	printf("finish\n");
	destroy_table(t1);
	printf("table destroyed");
	print_table(t1);

	return EXIT_SUCCESS;
}
*/
struct macro_table* create_table(int row_ammount) {

	struct macro_table *table = (struct macro_table*) malloc(
			sizeof(struct macro_table));
	table->row_ammount = row_ammount;
	int i;
	i = 1;

	struct table_row *head = (struct table_row*) malloc(
			sizeof(struct table_row));

	head->next = NULL;
	table->head = head;


	while (i < row_ammount) {
		struct table_row *node = (struct table_row*) malloc(
				sizeof(struct table_row));

		if (head->next == NULL) {
			head->next = node;
			node->next = NULL;

		} else {
			struct table_row *current = head;
			while (1) {
				if (current->next == NULL) {
					current->next = node;

					break;
				}
				current = current->next;
			}
		}
		i++;
	}
	return table;
}

void add_row_to_table(struct macro_table *table) {
	struct table_row *new_row = (struct table_row*) malloc(
			sizeof(struct table_row));

	struct table_row *current = table->head;
	while (1) {
		if (current->next == NULL) {
			current->next = new_row;
			table->row_ammount++;
			break;
		}
		current = current->next;
	}
}

char* get_cel_value(struct macro_table *table, int row_num, int col_num) {

	int i;
	i = 1;

	struct table_row *current = table->head;

	while (current->next != NULL) {
		if (i < row_num) {
			current = current->next;
			i++;
		} else {
			break;
		}
	}
	return current->row_structure[col_num-1];
}

void set_cel_value(struct macro_table *table, int row_num, int col_num,
		char *value) {

	int i;
	i = 1;

	if (row_num < 1 || col_num < 0 || row_num > table->row_ammount || col_num > COL_AMMOUNT) {
		printf("error! cell location not found");
		exit(1);
	}

	struct table_row *current = table->head;

	while (current->next != NULL) {
		if (i < row_num) {
			current = current->next;
			i++;
		} else {
			break;
		}
	}
	strcpy(current->row_structure[col_num-1], value);

}

int search_table_for_value(struct macro_table *table, char *value){

	char *word;
	int row_loc;
	row_loc = 1;

	struct table_row *current = table->head;

	while (current->next != NULL) {
		word = strtok(current->row_structure[1], " \t");
		if(strcmp(value, word) == 0){
			return row_loc;
		}
		word = strtok(NULL, " \t");
		current = current->next;
		row_loc++;
	}

	return -1;

}

void destroy_table(struct macro_table *table){
	struct table_row *current = table->head;
	struct table_row *next;

	while (current != NULL) {
		next = current->next;
		free(current);
		current = next;
	}

	table->head = NULL;
}

void print_table(struct macro_table *table){
	struct table_row *current = (struct table_row*) malloc(
			sizeof(struct table_row));
	current = table->head;
	while(current != NULL){
		printf("%s | %s\n", &current->row_structure[0], &current->row_structure[1]);
		current = current->next;
	}

}
