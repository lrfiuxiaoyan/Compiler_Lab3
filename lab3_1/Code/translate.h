#include "semantic.h"
typedef struct Operand_* Operand;
struct Operand_ {
	enum {
		VARIABLE, VAR_GETADDRESS, VAR_GETVALUE, CONSTANT, ADDRESS, LABEL_NAME, FUNCTION_NAME
	} kind;
	union {
		char varName[32];
		char funName[32];
		char value[32];
		char labelName[32];
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
		struct { Operand op; char size[10]; } dec;
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
	InterCodes prev;
	InterCodes next;
};

extern InterCodes head, tail;


void genLABEL(InterCode resCode, char* labelName);
void genFUNCTION(InterCode resCode, char* funName);
void genASSIGN(InterCode resCode, char* leftName, int leftType, char* rightName, int rightType); //leftType:0->x, 1->*x; rightType:0->y, 1->*y, 2->&y
void genBINOP(InterCode resCode, char* op1Name, int op1Type, char* op2Name, int op2Type, char* resultName, int operatorType); //0:+, 1:-, 2:*, 3:/
void genGOTO(InterCode resCode, char* labelName);
void genIF(InterCode resCode, char* op1Name, int op1Type, char* op2Name, int op2Type, char* labelName, int relopType);
void genRETURN(InterCode resCode, char* opName, int opType);
void genDEC(InterCode resCode, char* opName, char* size);
void genARG(InterCode resCode, char* opName, int opType);
void genCALL(InterCode resCode, char* opName, char* funName);
void genPARAM(InterCode resCode, char* opName);
void genREAD(InterCode resCode, char* opName);
void genWRITE(InterCode resCode, char* opName, int opType);

void printCodes(InterCodes head, char* fileName);

InterCodes createInterCodesNode(InterCode code);
void insertInterCodesNode(InterCodes codeToInsert);
void test_gen(char* fileName);
