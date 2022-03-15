#ifndef _Utils_h
#define _Utils_h
#include "globals.h"


#define SKIP_WHITE(line.content, i) /* Skip white chars  */
	 for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)));

void *malloc_with_check(long size); /* Allocates memory in the required size. Exits the program if failed.*/

int printf_error(line_info line, char *message, ...);

bool checkExist(table tab, char *key)/*check if a key exist in the table already*/  

bool check_int(char *string) /*checking if the char is digitl*/

bool alphanumeric_str(char *string) { /*check for every char in string if it is non alphanumeric */

bool is_valid_label(char *name) {/* Checking if is valid label */

bool find_label(line_info line, char *symbol_dest)/* Returns whether an error occurred during the try of parsing the symbol. puts the symbol into the second buffer. */

char *addext(char *s0, char* s1) /* add strings to each other*/ 

attribute find_attribute_by_name(char *name) { /* recognize atrribute type and return it*/ 

void free_code_image(machine_code **code_image, long IC); /* Frees all the dynamically-allocated memory for the code image.*/

#endif

