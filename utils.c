#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "utils.h"

	
	MOVE_TO_NOT_WHITE(line.content, i) /* Skip white chars at the beginning anyway */

	/* Let's allocate some memory to the string needed to be returned */
	for (; line.content[i] && line.content[i] != ':' && line.content[i] != EOF && i <= 81; i++, j++) {
		symbol_dest[j] = line.content[i];
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