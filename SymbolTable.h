/* A dynamically-allocated symbol table */
#ifndef StmbolTable
#define SymbolTable


typedef enum symbol_type
{/* The symbol type */
	code_symbol,
	data_symbol,
	external_symbol,
	external_refernce,/*Address that contains a reference to (a usage of) external symbol */
	entry_symbol
} symbol_type;


typedef struct row* table;/*pointer to table row  */

typedef struct row {/* A single table line */
{
    char *key;/* Key (symbol name) is a string */
    long value;/*Address of the symbol */
    long baseadress; /* Address of the baseadress of the symbol*/
    long offset; /* Address of the offset */
    symbol_type type;	/* Symbol type */
    table next;/* Next row in the table of the symbol*/

} table_row;

void add_table_item(char *key,long value,long baseadress ,long offset, symbol_type type,table *tab); /*Adds an item to the table, keeping it sorted.*/

void free_table(table tab); /*Deallocates all the memory required by the table.*/

table filter_table_by_type(table tab, symbol_type type); /* Returns all the entries by their type in a new table*/


#endif
