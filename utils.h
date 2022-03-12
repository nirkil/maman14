/* Contains general-purposed functions*/
#ifndef _Utils_h
#define _Utils_h

#define SKIP_WHITE(line.content, i) /* Skip white chars  */
	{ 
        while(string[i]!='\0')
     {
          if(string[i] == ' ')
              i++;
	 }
	}

void *malloc_with_check(long size); /* Allocates memory in the required size. Exits the program if failed.*/

int printf_error(line_info line, char *message, ...);

bool checkExist(table tab, char *key)/*check if a key exist in the table already*/  

bool check_int(char *string) /*checking if the char is digitl*/

bool alphanumeric_str(char *string) { /*check for every char in string if it is non alphanumeric */

bool is_valid_label(char *name) {/* Checking if is valid label */

bool find_label(line_info line, char *symbol_dest)/* Returns whether an error occurred during the try of parsing the symbol. puts the symbol into the second buffer. */


