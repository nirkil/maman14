#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool process_file(char *filename) {
	
	int temp_c;
	long ic = IC_INIT_VALUE, dc = 0, icf, dcf; /* Memory address counters */

	bool checking_success = TRUE; /* for chechking later if the pass are correct*/
    
	char *input_filename;
	char temp_line[MAX_LINE_LENGTH + 1]; /* temporary string for storing line */

	FILE *fd; /* Current assembly file descriptor to process after the first pass of macros*/

	long data_img[8191]; /* Contains an image of the machine code ???*/
	machine_בםגק *code_img[8191];
	
	table symbol_table = NULL; /* The symbol table */
	line_info curr_line_info;
 
	fd = fopen(input_filename, "r"); /*  Open file, after macros */

	if (fd == NULL) { /* if file couldn't be opened, write to stderr */
		fprintf(stderr,"cannot open file \n");
		free(input_filename); 
		return FALSE;
    }
}
 
/* start second pass*/

	curr_line_info.file_name = input_filename;
	curr_line_info.content = temp_line; /* We use temp_line to read from the file, but it stays at same location. */
	
    /* loop which running over the line's file */
	for (curr_line_info.line_numb = 1;fgets(temp_line, MAX_LINE_LENGTH + 1, fd) != NULL; curr_line_info.line_num++) 
    {
		/* if line too long, the buffer doesn't include the '\n' char OR the file isn't on end. */
		if (strchr(temp_line, '\n') == NULL && !feof(fd)) 
        {
            printf_error(curr_line_info, "Line too long to process,the maximum line length should be %d.",80);
			checking_success = FALSE; /* Print error message  nd prevent further line processing */
			do {
				temp_c = fgetc(fd);
			} while (temp_c != '\n' && temp_c != EOF);

		} else {
			if (!process_line_fpass(curr_line_info, &ic, &dc, code_img, data_img, &symbol_table)) {
				if (is_success) {
					/*free_code_image(code_img, ic_before);*/
					icf = -1;
					is_success = FALSE;
				}
			}
		}
