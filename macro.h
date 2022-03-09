
#define MACRO_OPEN "macro"
#define MACRO_CLOSE "endm"
#define MAX_LINE_LENGTH 8
#define ADDED_LINE_LENGTH 2
#define COL_AMMOUNT 2

int macro_flag;


/*gets input file and empty new file. Fills new file according to the instructions*/
FILE* handle_file(FILE *input_file, FILE *new_file);

/* compares given word to macro table and checks if it's there.
 * returns the macro definition of the macro name word
 * or returns NULL if the word is not a macro name  */
char* search_macro_name(struct table *macro_table, char *macro_name);

/*compares given word to MACRO_OPEN*/
int is_it_macro_def_open(char *word);

/*searches line and compares each word to MACRO_CLOSE*/
int is_it_macro_def_close(char *line);

/*adds new macro name & def to a new macro table line*/
void add_def_to_macro_table(struct table *macro_table, char *macro_name, char *macro_def);

/*copies wanted text to the new file*/
void add_to_new_file(FILE *new_file, char *macro_def);

char* find_macro_name(char *line);






