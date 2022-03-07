/* Contains general-purposed functions*/
#ifndef _utils_h
#define _utils_h


#define MOVE_TO_NOT_WHITE(string, index) /* moves the index to the next place in string where the char isn't white */
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

void *malloc_with_check(long size); /* Allocates memory in the required size. Exits the program if failed.*/


