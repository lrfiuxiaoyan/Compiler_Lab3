#include "semantic.h"
typedef struct Operand_* Operand;
struct Operand_ {
	enum {
		VARIABLE, VARIABLE_GETADDRESS, VARIABLE_GETVALUE, CONSTANT, ADDRESS, LABEL, FUNCTION, ARRAY, STRUCTURE
	} kind;
	union {
		char* varName;
		char* funName;
		int value;
		char* labelName;
	} u;
};

typedef struct InterCode_* InterCode;
//0: >, 1: ==, 2: <, 3: >=, 4: <=, 5: !=
struct InterCode_ {
	enum { ASSIGN, ADD, SUB, MUL, DIV, LABEL, FUNCTION, GOTO, IF, RETURN, DEC, ARG, CALL, PARAM, READ, WRITE } kind;
	union {
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { Operand op; } label;
		struct { Operand op; } function;
		struct { Operand op; } _goto;
		struct { Operand op1, op2, label; int relop; } _if;
		struct { Operand op; } _return;
		struct { Operand op; int size; } dec;
		struct { Operand op; } arg;
		struct { Operand op, function; } call;
		struct { Operand op; } param;
		struct { Operand op; } read;
		struct { Operand op; } write;
	} u;
};

typedef struct InterCodes_* InterCodes;
struct InterCodes_ {
	InterCode code;
	struct InterCodes* prev;
	struct InterCodes* next;
};

extern InterCodes head, tail;


void genLABEL(InterCode resCode, char* labelName);
void genFUNCTION(InterCode resCode, char* funName);
void genASSIGN(InterCode resCode, char* leftName, char* rightName, int leftType, int rightType); //leftType:0->x, 1->*x; rightType:0->y, 1->*y, 2->&y
void genBINOP(InterCode resCode, char* op1Name, char* op2Name, char* resultName, int operatorType); //0:+, 1:-, 2:*, 3:/
void genGOTO(InterCode resCode, char* labelName);
void genIF(InterCode resCode, char* op1Name, char* op2Name, char* labelName, int relopType);
void genRETURN(InterCode* resCode, char* opName);
void genDEC(InterCode resCode, char* opName, int size);
void genARG(InterCode resCode, char* opName);
void genCALL(InterCode resCode, char* opName, char* funName);
void genPARAM(InterCode resCode, char* opName);
void genREAD(InterCode resCode, char* opName);
void genWRITE(InterCode resCode, char* opName);

void printCode(InterCode code);

InterCodes createInterCodesNode(InterCode code);
void insertInterCodesNode(InterCodes codeToInsert);
