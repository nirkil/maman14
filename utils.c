#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

	
	skip_white(line.content, i) /* Skip white chars  */
	{ 
        while(string[i]!='\0')
     {
          if(string[i] == ' ')
              i++;
	 }
	}

	bool alphanumeric_str(char *string) { /*check for every char in string if it is non alphanumeric */
	int i;
	for (i = 0; string[i]; i++) {
		if (!isalpha(string[i]) && !isdigit(string[i])) return FALSE;
	}
	return TRUE;
}

    bool is_valid_label(char *name) {/* Checking if is valid label */
	return name[0] && strlen(name) <= 31 && isalpha(name[0]) && alphanumeric_str(name + 1) && !checkExist(table tab, char *key);
}

bool checkExist(table tab, char *key)
{	
	while(tab)
	{
		if(strcmp(tab->key,key) == 0)
			return TRUE;
		tab = tab->next;		
	}
	return FALSE;
}


/* Returns whether an error occurred during the try of parsing the symbol. puts the symbol into the second buffer. */
bool find_label(line_info line, char *symbol_dest) {
	int j, i;
	i = j = 0;

	skip_white(line.content, i)/* Skip white chars at the beginning anyway */
  
	/* Let's allocate some memory to the string needed to be returned */
	while (line.content[i] != ':' && line.content[i] != EOF && i <= 80) 
	{
		symbol_dest[j] = line.content[i];
		i++, j++; 
	}
	 symbol_dest[j] = '\0'; /* End of string */

	/* if it was a try to define label, print errors if needed. */
	if (line.content[i] == ':') {
		if (!is_valid_label_name(symbol_dest)) {
			printf_line_error(line,
			                  "Invalid label name - cannot be longer than 32 chars, may only start with letter be alphanumeric.");
			symbol_dest[0] = '\0';
			return TRUE; /* No valid symbol, and no try to define one */
		}
		return FALSE;
	}
	symbol_dest[0] = '\0';
	return FALSE; /* There was no error */
}

void *malloc_with_check(long size) {
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;

	int printf_error(line_info line, char *message, ...) /* Prints the errors into a file*/
	{ 
	int result;
	va_list p;
	fprintf(stderr,"Error In %s:%ld: ", line.file_name, line.line_num); /* Print error to stderr */
    va_start(p, message);
	result = vfprintf(stderr, message, args);
	va_end(p);
    fprintf(stderr, "\n");
	return result;
} 

struct attributes_lookup_item {
	char *name;
	attributes value;
};

static struct attributes_lookup_item
		attributes_lookup_table[] = {
		{"string", STRING_INST},
		{"data",   DATA_INST},
		{"entry",  ENTRY_INST},
		{"extern", EXTERN_INST},
		{NULL, NONE_ADDR}
};

attributes find_attributes_by_name(char *name) { /* recognize atrribute type*/ 
	struct instruction_lookup_item *curr_item;
	for (curr_item = attributes_lookup_table; curr_item->name != NULL; curr_item++) {
		if (strcmp(curr_item->name, name) == 0) {
			return curr_item->value;
		}
	}
	return NONE_ADDR;
}

bool check_int(char *string) {
	int i = 0;
	if (string[0] == '-' || string[0] == '+') string++; /* if string starts with +/-, it's OK */
	for (; string[i]; i++) { /* make sure that every charcter is a digit until the end */
		if (!isdigit(string[i])) {
			return FALSE;
		}
	}
	return i > 0; /* if i==0 then it was an empty string! */
}
