#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "SymbolTable.h"

static bool write_ob(machine_code **code_img, long *data_img, long icf, long dcf, char *filename)
{
	int i,n;
	long val, val_temp;
	FILE *file_desc;
	char *output_filename = addext(filename, ".ob"); /* add extension of file to open */
	file_desc = fopen(output_filename, "w"); /* Try to open the file for writing */
    free(output_filename);
	if (file_desc == NULL) 
	{
		printf("Can't create or rewrite to file %s.", output_filename);
		return FALSE;
	}

 fprintf(file_desc, "%ld %ld", icf - IC_INIT_VALUE, dcf); /* print data/code word count on top */

   for (i = 0; i < icf - IC_INIT_VALUE; i++) 
   {
	  if (code_img[i]->length > 0) 
	   {
		 val = (code_img[i]->code.first->LAST << 19) | (code_img[i]->code.first->ARE << 16) | (code_img[i]->code.first->opcode ) 
	     val_temp = (code_img[i] >> 16);
	     fprintf(file_desc, "\n%d %s%.4lx%s ",i + 100, A, val_temp, -);
		 val_temp = (code_img[i] >> 12);
	     fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, B, val_temp, -);
		 val_temp = (code_img[i] >> 8);
	     fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100,i + 100, C, val_temp, -);
         val_temp = (code_img[i] >> 4);
	     fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, D, val_temp, -);
		 val_temp = code_img[i];
	     fprintf(file_desc, "%.4d %s%.4lx",i + 100, E, val_temp);
	     i++;
	     if ((code_img[i]->code.second->adressing_des)!= none_addr)
	     { 
           val  = (code_img[i]->code.second->LAST << 19) | (code_img[i]->code.second->ARE << 16) |
			      (code_img[i]->code.second->funct << 12) | (code_img[i]->code.second->registesr_src << 8) |
			      (code_img[i]->code.second->adressing_src << 6) | (code_img[i]->code.second->register_des) << 2 |
			      (code_img[i]->code.second->adressing_des);  

		val_temp = (code_img[i] >> 16);
	    fprintf(file_desc, "\n%.4d %s%.4lx%s ",i + 100, A, val_temp, -);
		val_temp = (code_img[i] >> 12);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, B, val_temp, -);
		val_temp = (code_img[i] >> 8);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, C, val_temp, -);
        val_temp = (code_img[i] >> 4);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, D, val_temp, -);
		val_temp = code_img[i];
	    fprintf(file_desc, "%.4d %s%.4lx",i + 100, E, val_temp);
	     } 
		 else
		 i--; 
	    }
	    else 
	   {
			
		val = (code_img[i]->code.data->LAST << 19) | (code_img[i]->code.data->ARE << 16)| (code_img[i]->code.data->data);   
		val_temp = (code_img[i] >> 16);
	    fprintf(file_desc, "\n%.4d %s%.4lx%s ",i + 100, A, val_temp, -);
		val_temp = (code_img[i] >> 12);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, B, val_temp, -);
		val_temp = (code_img[i] >> 8);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, C, val_temp, -);
        val_temp = (code_img[i] >> 4);
	    fprintf(file_desc, "%.4d %s%.4lx%s ",i + 100, D, val_temp, -);
		val_temp = code_img[i];
	    fprintf(file_desc, "%.4d %s%.4lx",i + 100, E, val_temp);
		}
    }
	/* Close the file */
	fclose(file_desc);
	return TRUE;
}

 static bool write_ent(table tab, char *filename)
 {
	FILE *file_desc;
	char *full_filename = addext(filename, .ent);
	file_desc = fopen(full_filename, "w");
	free(full_filename);
	if (file_desc == NULL) 
	{
		printf("Can't create or rewrite to file %s.", full_filename);
		return FALSE;
	}
	if (tab == NULL) return TRUE;

	fprintf(file_desc, "\n%s ,%.ld, %.ld", table->key, table->baseadress, table->offset);
	while ((tab = tab->next) != NULL) 
	{
		fprintf(file_desc, "\n%s ,%.ld, %.ld", table->key, table->baseadress, table->offset);
	}
	fclose(file_desc);
	return TRUE;
 }

static bool write_ext(table tab, char *filename) 
{
	FILE *file_desc;
	char *full_filename = addext(filename, .ext);
	file_desc = fopen(full_filename, "w");
	free(full_filename);
	if (file_desc == NULL) 
	{
		printf("Can't create or rewrite to file %s.", full_filename);
		return FALSE;
	}
	if (tab == NULL) return TRUE;

	fprintf(file_desc, "\n%s "BASE" %.ld", tatableb->key, table->baseadress);
	fprintf(file_desc, "\n%s "OFFSET" %.ld", table->key, table->offset);
	while ((tab = tab->next) != NULL) 
	{
		fprintf(file_desc, "\n%s "BASE" %.ld", table->key, table->baseadress);
	    fprintf(file_desc, "\n%s "OFFSET" %.ld", table->key, table->offset);
	}
	fclose(file_desc);
	return TRUE;
}
    

int write_output_files(machine_code **code_img, long *data_img, long icf, long dcf, char *filename, table symbol_table)
{
	bool result;
	table externals = filter_table_by_type(symbol_table, EXTERNAL_REFERENCE);
	table entries = filter_table_by_type(symbol_table, ENTRY_SYMBOL);
	result = write_ob(code_img, data_img, icf, dcf, filename) && write_ext(externals, filename) && write_ent(entries, filename);
	free_table(externals);
	free_table(entries);
	return result;
}

