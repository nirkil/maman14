/* Output files related functions */
#ifndef _OUTTOFILE_H
#define _OUTTOFILE_H
#include "globals.h"
#include "SymbolTable.h"

/* Writes the output files of a single assembled file */
int write_output_files(machine_word **code_img, long *data_img, long icf, long dcf, char *filename, table symbol_table);

#endif
