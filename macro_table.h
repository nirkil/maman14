
#ifndef COL_AMMOUNT || MAX_CELL_LENGTH

#define COL_AMMOUNT 2
#define MAX_CELL_LENGTH 401


struct table_row{
	char row_structure[COL_AMMOUNT][MAX_CELL_LENGTH];
	struct table_row *next;
};

struct macro_table{
	int row_ammount;
	struct table_row *head;
};



/*creates table and returns pointer to an empty table*/
struct macro_table* create_table();

/*adds new empty row to given table*/
void add_row_to_table(struct macro_table *table);

/*returns the value of given cell location of given table*/
char* get_cel_value(struct macro_table *table, int row_num, int col_num);

/*sets value in given cell location of given table*/
void set_cel_value(struct macro_table *table, int row_num, int col_num, char *value);

/*searches a given value in the given table returns val's row number or -1 if not found*/
int search_table_for_value(struct macro_table *table, char *value);

/*deletes table*/
void destroy_table(struct macro_table *table);


#endif
