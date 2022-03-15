#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "globals.h"

/* Returns the first attributes from the specified index and if not returns NONE */
attribute find_attribute_from_index(line_info line, int *index) 
{
	char temp[MAX_LINE_LENGTH];
	int j;
	attribute result;

	SKIP_WHITE(line.content, *index) 
	if (line.content[*index] != '.') return NONE_ATTR;

	for (j = 0; line.content[*index] && line.content[*index] != '\t' && line.content[*index] != ' '; (*index)++, j++) 
	{
		temp[j] = line.content[*index];
	}
	temp[j] = '\0'; /* End of string */

	/* if invalid instruction but starts with ., return error */
	if ((result = find_attribute_by_name(temp+1)) != NONE_ATTR return result;
	printf_error(line, "Invalid instruction name: %s", temp);
	return ERROR_ATTR; /* starts with '.' but not a valid instruction! */
}  

bool process_string_attribute(line_info line, int index, long *data_img, long *dc)/* Processes a string instruction from index of source line*/
{
	char temp_str[MAX_LINE_LENGTH];
	char *last_quote_location = strrchr(line.content, '"');
	SKIP_WHITE(line.content, index)

	if (line.content[index] != '"') /* checking if the string isn't surrounded with "" */
    {
		printf_error(line, "Missing opening quote of string");
		return FALSE;
    } 
   else if (&line.content[index] == last_quote_location) 
   { 
		printf_error(line, "Missing closing quote of string");
		return FALSE;
	} 
  else 
   {
		int i;
		for (i = 0;line.content[index] && line.content[index] != '\n' &&  line.content[index] != EOF; index++,i++) 
        {/* Copy the string */
				temp_str[i] = line.content[index];
		}
		temp_str[last_quote_location - line.content] = '\0'; 

		for(i = 1;temp_str[i] && temp_str[i] != '"'; i++) 
        { /* kind of strcpy function but with dc increment */
			data_img[*dc] = temp_str[i];
			(*dc)++;
		}
		data_img[*dc] = '\0';
		(*dc)++;
	}
	return TRUE;
}
      /* Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.*/
bool process_data_attribute(line_info line, int index, long *data_img, long *dc) 
{
	char temp[80], *temp_ptr;
	long value;
	int i;
	SKIP_WHITE(line.content, index)
	if (line.content[index] == ',') 
	{
		printf_error(line, "Unexpected comma after .data attributes");
	}
	do 
	{
	  for (i = 0;line.content[index] && line.content[index] != EOF && line.content[index] != '\t' &&
		     line.content[index] != ' ' && line.content[index] != ',' &&
		     line.content[index] != '\n'; index++, i++) 
	   {
			temp[i] = line.content[index];
		}
		temp[i] = '\0'; /* End of string */

		if (!check_int(temp)) 
		{
			printf_error(line, "Expected integer for .data attributes (got '%s')", temp);
			return FALSE;
		}
		value = strtol(temp, &temp_ptr, 10); /* convert into integer number */

		data_img[*dc] = value;
        (*dc)++; /* a word was written right now */

		SKIP_WHITE(line.content, index)
		if (line.content[index] == ',') index++;
		else if (!line.content[index] || line.content[index] == '\n' || line.content[index] == EOF)
			break; /* nothing to process anymore */

		/* check if there a in writting way */
		SKIP_WHITE(line.content, index)
		if (line.content[index] == ',') 
		{
			printf_error(line, "Multiple consecutive commas.");
			return FALSE;
		} else if (line.content[index] == EOF || line.content[index] == '\n' || !line.content[index]) 
		{
			printf_error(line, "Missing data after comma");
			return FALSE;
		}
	} while (line.content[index] != '\n' && line.content[index] != EOF);
	return TRUE;
}

