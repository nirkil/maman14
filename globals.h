#ifndef _GLOBALS_
#define _GLOBALS_

#define MAX_LINE_LENGTH 81 /* Maximum length of a single source line  */

#define IC_INT_VALUE 100 /*Initial IC value */

typedef enum booleans 
{/* oolean T/F definition */
	FALSE = 0, TRUE = 1
} bool;

typedef enum opcodes 
{ /* Commands opcode */
	
	MOV_OP = 0, /* First Group */
	CMP_OP = 1,
    ADD_OP = 2,
	SUB_OP = 2,
    LEA_OP = 4, /* END First Group */

	CLR_OP = 5, /* Second Group */
	NOT_OP = 5,
	INC_OP = 5,
	DEC_OP = 5,
    JMP_OP = 9,
	BNE_OP = 9,
	JSR_OP = 9,
    RED_OP = 12,
	PRN_OP = 13, /* END Second Group */
	
    RTS_OP = 14, /* Third Group */
	STOP_OP = 15, /* END Third Group */
	
    NONE_OP = -1 /* Failed/Error */
} opcode;

typedef enum funct 
{ /* Commands funct */
	
	ADD_FUNCT = 10,
	SUB_FUNCT = 11,

	
	CLR_FUNCT = 10,
	NOT_FUNCT = 11,
	INC_FUNCT = 12,
	DEC_FUNCT = 13,

	
	JMP_FUNCT = 10,
	BNE_FUNCT = 11,
	JSR_FUNCT = 12,
} funct; 


typedef enum reg 
{ /* Registers */
	r0 = 0,
	r1,
	r2,
	r3,
	r4,
	r5,
	r6,
	r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
	NONE_REG = -1
} reg;


typedef enum addressing_types 
{ /* Operand addressing type */
	
	Immediate_addr = 0,/* Immediate addressing (0) */
	
	Direct_addr = 1,/* Direct addressing (1) */
	
	index_addr= 2,/* index addressing (2) */

	Register_addr = 3, 	/* Register addressing */
	
	none_addr = -1 /* Failed/Not detected addressing */
} addressing_type;

/* attributes type */
typedef enum attribute 
{
	DATA_ATTR, 
	EXTERN_ATTR,
	ENTRY_ATTR,
	STRING_ATTR,
	NONE_ATTR, /* Not found */
	ERROR_ATTR /* Parsing/syntax error */
} attribute;

typedef struct line_info 
{ /*struct of single line with all is needed information*/
	char *file_name; /* the name of the file*/
    char *content; /* the content in the line */
	long line_num; /* the line number part of the file */	
} line_info;

/* Represents the first binary in the machine code */
typedef struct first_line_code
{
	
	unsigned int opcode: 16;
    unsigned int ARE: 3;
	unsigned int LAST: 1;

} first_line_code;

/* Represents the second binary in the machine code */
typedef struct second_line_code 
{
	unsigned int adressing_des: 2;
	unsigned int register_des: 4;
	unsigned int adressing_src: 2;
	unsigned int registesr_src : 4;
	unsigned int funct: 4;
	unsigned int ARE: 3;
	unsigned int LAST: 1;

} second_line_code;
  
/* Represents the extra binary in the machine code */
typedef struct extra_line_code 
{
	unsigned int base_offset_addr: 16;
    unsigned int ARE: 3;
	unsigned int LAST: 1;

} extra_line_code;

/* Represents a single data digit. */
typedef struct data_digit 
{
	unsigned int data;
	unsigned int ARE: 3;
	unsigned int LAST: 1;
} data_digit;

/* Represents a general machine code  */
typedef struct machine_code 
{ /* if it represents code (not additional data), this field contains the total length required by the code. if it's data, this field is 0. */
	short length;
	/* The content can be one of this 3 strcuts */
	union code 
	{
		first_line_code *first;
		second_line_code *second; 
        extra_line_code *extra;
        data_digit *data; 
	} code;

} machine_code;

#endif
