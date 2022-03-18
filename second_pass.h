

#ifndef SEC_PASS
#define SEC_PASS

/*operates the second pass: gets input files and creates necessary output files*/
void second_pass_handle(FILE *input_file);

/*adds the word entry in given symbol line of symbol table*/
void add_entry_to_symbol_table(table symbol_table, char *symbol);

/*fills in the empty assembler table line after the first pass*/
void complete_assembler_table(machine_code assembler_table, table symbol_table);

/*fills entry output file*/
void fill_entries_output_file(table symbol_table, FILE *entries_output_file);

/*fills externals output file*/
void fill_externals_output_file(machine_code assembler_table, FILE *externals_output_file);

/*fills object output file*/
void fill_object_output_file(machine_code assembler_table, FILE *object_output_file);

#endif
