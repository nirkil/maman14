#ifndef _GLOBALS_
#define _GLOBALS_

#define MAX_LINE_LENGTH 81 /* Maximum length of a single source line  */

#define IC_INT_VALUE 100 /*Initial IC value */

typedef enum booleans {/* oolean T/F definition */
	FALSE = 0, TRUE = 1
} bool;

typedef enum opcodes { /* Commands opcode */
	
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


typedef enum funct { /* Commands funct */
	
	ADD_FUNCT = 10,
	SUB_FUNCT = 11,

	
	CLR_FUNCT = 10,
	NOT_FUNCT = 11,
	INC_FUNCT = 12,
	DEC_FUNCT = 13,

	
	JMP_FUNCT = 10,
	BNE_FUNCT = 11,
	JSR_FUNCT = 12,

	NONE_FUNCT = 0 /** Default (No need/Error) */
} funct;


typedef enum registers {/* Registers */
	R0 = 0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
	NONE_REG = -1
} reg;


typedef enum addressing_types { /* Operand addressing type */
	
	IMMEDIATE_ADDR = 0,/** Immediate addressing (0) */
	
	DIRECT_ADDR = 1,/** Direct addressing (1) */
	
	INDEX_ADDR = 2,/** index addressing (2) */

	REGISTER_ADDR = 3, 	/** Register addressing */
	
	NONE_ADDR = -1 /** Failed/Not detected addressing */
} addressing_type;

