#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "SymbolTable.h"
#include "utils.h"

void add_table_item(char *key,long value,long baseadress ,long offset, symbol_type type,table *tab);
{  
   char *temp_key;
   table prev_entry, curr_entry, new_entry;
   
   new_entry = (table) malloc_with_check(sizeof(table_row)); /* allocate memory for new entry */
   temp_key = (char *) malloc_with_check(strlen(key) + 1);/* Prevent "Aliasing" of pointers.*/
   strcpy(temp_key, key);

    new_entry->key = temp_key;
    new_entry->value = value;
    new_entry->baseadress = baseadress;
    new_entry->offset = offset;
    new_entry->type = type;

    if ((*tab) == NULL ) /* if the table's null, set the new entry as the head. */
	{ 
		new_entry->next = (*tab);
		(*tab) = new_entry;
		return;

	}

    curr_entry = (*tab)->next; /* Insert the new table entry, keeping it sorted */
	prev_entry = *tab;
	while (curr_entry != NULL && curr_entry->value < value)
   {
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
	}

	new_entry->next = curr_entry;
	prev_entry->next = new_entry;
}

table filter_table_by_type(table tab, symbol_type type) 
{ /* For each entry divsion insert to the new table. */
	table new_table = NULL;
	do 
	{
		if (tab->type == type) 
		{
			add_table_item(tab->key,tab->value,tab->baseadress ,tab->offset, tab->type, &new_table);
		}
	} while ((tab = tab->next) != NULL);
	
	return new_table; /* It holds a pointer to the first entry*/
}

void free_table(table tab) /*Deallocates all the memory required by the table.*/
{
	table prev_entry, curr_entry = tab;
	while (curr_entry != NULL) 
	{
		prev_entry = curr_entry;
		curr_entry = curr_entry->next;
		free(prev_entry->key); 
		free(prev_entry);
	}
}




	
	

