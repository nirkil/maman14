#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"
#include "globals.h"

bool alphanumeric_str(char *string) 
{ /*check for every char in string if it is non alphanumeric */
  int i;
  for (i = 0; string[i]; i++) 
  {
    if (!isalpha(string[i]) && !isdigit(string[i])) return FALSE;
  }
return TRUE;
}

bool is_valid_label(char *name) 
{ /* Checking if is valid label */
  return name[0] && strlen(name) <= 31 && isalpha(name[0]) && alphanumeric_str(name + 1) && !checkExist(table tab, char *key);
}

bool checkExist(table tab, char *key)/*check if a key exist in the table already*/ 
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
bool find_label(line_info line, char *symbol_dest) 
{
	int j, i;
	i = j = 0;

	SKIP_WHITE(line.content, i)/* Skip white chars at the beginning anyway */
  
	/* Let's allocate some memory to the string needed to be returned */
	for (; line.content[i] && line.content[i] != ':' && line.content[i] != EOF && i <= MAX_LINE_LENGTH; i++, j++) 
	{
	     symbol_dest[j] = line.content[i];
	}
	symbol_dest[j] = '\0'; /* End of string */

	/* if it was a try to define label, print errors if needed. */
	if (line.content[i] == ':')
	{
		if (!is_valid_label(symbol_dest)) 
		{
			printf_error(line, "Invalid label name sorry has to be no longer than 32 chars and may only start with letter be alphanumeric.");
			symbol_dest[0] = '\0';
			return TRUE; /* No valid symbol, and no try to define one */
		}
		return FALSE;
	}
	symbol_dest[0] = '\0';
	return FALSE; /* There was no error */
}


void *malloc_with_check(long size)
{
	void *ptr = malloc(size);
	if (ptr == NULL) 
	{
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;

	int printf_error(line_info line, char *message, ...) /* Prints the errors into a file*/
	
	int result;
	va_list p;
	fprintf(stderr,"Error In %s:%ld: ", line.file_name, line.line_num); /* Print error to stderr */
    va_start(p, message);
	result = vfprintf(stderr, message, args);
	va_end(p);
    fprintf(stderr, "\n");
	return result;
} 

bool check_int(char *string) 
{
	int i = 0;
	if (string[0] == '-' || string[0] == '+') string++; /* if string starts with +/-, it's OK */
	for (; string[i]; i++) 
	{ /* make sure that every charcter is a digit until the end */
		if (!isdigit(string[i])) 
		{
			return FALSE;
		}
	}
	return i > 0; /* if i==0 then it was an empty string! */
}

char *addext(char *s0, char* s1)
{ 
	char *str = (char *)malloc_with_check(strlen(s0) + strlen(s1) + 1);
	strcpy(str, s0);
	strcat(str, s1);
	return str;
}


struct attribute_lookup_item 
{
    char *name;
	attribute value;
};

static struct attributes_lookup_item
		attributes_lookup_table[] = 
{
		{"string", STRING_ATTR},
		{"data",   DATA_ATTR},
		{"entry",  ENTRY_ATTR},
		{"extern", EXTERN_ATTR},
		{NULL, NONE_ATTR}
};

attribute find_attribute_by_name(char *name) 
{ /* recognize atrribute type*/ 
	struct attribute_lookup_item *curr_item;
	for (curr_item = attributes_lookup_table; curr_item->name != NULL; curr_item++) 
	{
		if (strcmp(curr_item->name, name) == 0) 
		{
			return curr_item->value;
		}
	}
	return NONE_ATTR;
}

void free_code_image(machine_code **code_image, long fic) 
{
	long i;
	/* for each not-null cell (we might have some "holes", so we won't stop on first null) not completly done!!*/
	for (i = 0; i < fic; i++) 
	{
		machine_code *curr_code = code_image[i];
		if (curr_code != NULL) {
			
			if (curr_code->length > 0) {
				free(curr_code->code.first);
				free(curr_code->code.second);
				free(curr_code->code.extra);
			} else {
				free(curr_code->code.data);
			}
			/* free the pointer to the union */
			free(curr_code);
			code_image[i] = NULL;
		}
	}
}

