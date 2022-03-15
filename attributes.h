/* fuction to the attributes part all the diffrenet types of instruction */
#ifndef _ATTRIBUTES_H
#define _ATTRIBUTES_H
#include "globals.h"


attribute find_attribute_from_index(line_info line, int *index)/* Returns the first attributes from the specified index and if not returns NONE */

bool process_string_attribute(line_info line, int index, long *data_img, long *dc)/* Processes a string instruction from index of source line*/

/* Parses a .data instruction. copies each number value to data_img by dc position, and returns the amount of processed data.*/
bool process_data_attribute(line_info line, int index, long *data_img, long *dc) 

#endif
