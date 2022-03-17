#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static bool process_file(char *filename) {
	
	int temp_c;
	long ic = IC_VALUE, dc = 0, icf, dcf; /* Memory address counters */

	bool checking_success = TRUE; /* for chechking later if the pass are correct*/
    
	char *input_filename;
	char temp_line[MAX_LINE_LENGTH + 1]; /* temporary string for storing line */

	FILE *fd; /* Current assembly file descriptor to process after the first pass of macros*/

	long data_img[8191]; /* Contains an image of the machine code ???*/
	machine_code *code_img[8191];
	
	table symbol_table = NULL; /* The symbol table */
	line_info curr_line_info;
    
	input_filename = addext(filename, ".as");

	fd = fopen(input_filename, "r"); /*  Open file, after macros */

	if (fd == NULL) { /* if file couldn't be opened, write to stderr */
		fprintf(stderr,"cannot open file \n");
		free(input_filename); 
		return FALSE;
    }
    
     /* start first pass*/

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
			if (!process_line_fpass(curr_line_info, &ic, &dc, code_img, data_img, &symbol_table)) 
			{ /* steps 1.01 -1.17 in algo */
				if (is_success) 
				{
					/*free_code_image(code_img, ic_before);*/
					icf = -1;
					is_success = FALSE;
				}
			}
		}
    /* Save ICF & DCF step 1.18 in the algo */
	icf = ic;
	dcf = dc;

	/* if first pass didn't fail, start the second pass */
	if (is_success) {

	ic = IC_INIT_VALUE;

	/* step 1.19 in the algo */
	add_value_to_type(symbol_table, icf, DATA_SYMBOL);
	  
   /*second pass */

	/* Write files if second pass succeeded */
	if (is_success) 
	{
		/* Everything was done. Write to *filename.ob/.ext/.ent */
		is_success = write_output_files(code_img, data_img, icf, dcf, filename, symbol_table);
	}
	
	free(input_filename);
	free_table(symbol_table);
	free_code_image(code_img, icf);

	return is_success;
}
