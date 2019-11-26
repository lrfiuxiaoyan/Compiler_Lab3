#include "translate.h"

void genLABEL(InterCode resCode, char* labelName) {
	resCode->kind = LABEL;
	((resCode->u).label.op)->kind = LABEL;
	(((resCode->u).label.op)->u).labelName = malloc(strlen(labelName)+1);
	strcpy((((resCode->u).label.op)->u).labelName, labelName); 
}
void genFUNCTION(InterCode resCode, char* funName) {
	resCode->kind = FUNCTION;
	((resCode->u).function.op)->kind = FUNCTION;
	(((resCode->u).function.op)->u).funName = malloc(strlen(funName)+1);
	strcpy((((resCode->u).function.op)->u).funName, funName);
}
void genASSIGN(InterCode resCode, char* leftName, char* rightName, int leftType, int rightType) { //leftType:0->x, 1->*x; rightType:0->y, 1->*y, 2->&y
	resCode->kind = ASSIGN;
	switch(leftType) {
		case 0: ((resCode->u).assign.left)->kind = VARIABLE; break;
		case 1: ((resCode->u).assign.left)->kind = VARIABLE_GETVALUE; break;
	}
	(((resCode->u).assign.op1)->u).varName = malloc(strlen(leftName)+1);
	strcpy((((resCode->u).assign.op1)->u).varName, leftName);
	switch(rightType) {
		case 0: ((resCode->u).assign.right)->kind = VARIABLE; break;
		case 1: ((resCode->u).assign.right)->kind = VARIABLE_GETVALUE; break;
		case 2: ((resCode->u).assign.right)->kind = VARIABLE_GETADDRESS; break;
	}
	(((resCode->u).assign.op2)->u).varName = malloc(strlen(rightName)+1);
	strcpy((((resCode->u).assign.op2)->u).varName, rightName);
}
void genBINOP(InterCode resCode, char* op1Name, char* op2Name, char* resultName, int operatorType) { //0:+, 1:-, 2:*, 3:/
	switch(operatorType) {
		case 0: resCode->kind = ADD; break;
		case 1: resCode->kind = SUB; break;
		case 2: resCode->kind = MUL; break;
		case 3: resCode->kind = DIV; break;
	}
	((resCode->u).binop.op1)->kind = VARIABLE;
	(((resCode->u).binop.op1)->u).varName = malloc(strlen(op1Name)+1);
	strcpy((((resCode->u).binop.op1)->u).varName, op1Name);
	((resCode->u).binop.op2)->kind = VARIABLE;
	(((resCode->u).binop.op2)->u).varName = malloc(strlen(op2Name)+1);
	strcpy((((resCode->u).binop.op2)->u).varName, op2Name);
	((resCode->u).binop.result)->kind = VARIABLE;
	(((resCode->u).binop.result)->u).varName = malloc(strlen(resultName)+1);
	strcpy((((resCode->u).binop.result)->u).varName, resultName);
}
void genGOTO(InterCode resCode, char* labelName) {
	resCode->kind = GOTO;
	((resCode->u)._goto.op)->kind = LABEL;
	(((resCode->u)._goto.op)->u).labelName = malloc(strlen(labelName)+1);
	strcpy((((resCode->u)._goto.op)->u).labelName, labelName);
}
void genIF(InterCode resCode, char* op1Name, char* op2Name, char* labelName, int relopType) {
	resCode->kind = IF;
	((resCode->u)._if.op1)->kind = VARIABLE;
	(((resCode->u)._if.op1)->u).varName = malloc(strlen(op1Name)+1);
	strcpy((((resCode->u)._if.op1)->u).varName, op1Name);
	((resCode->u)._if.op2)->kind = VARIABLE;
	(((resCode->u)._if.op2)->u).varName = malloc(strlen(op2Name)+1);
	strcpy((((resCode->u)._if.op2)->u).varName, op2Name);
	((resCode->u)._if.label)->kind = LABEL;
	(((resCode->u)._if.label)->u).labelName = malloc(strlen(labelName)+1);
	strcpy((((resCode->u)._if.label)->u).labelName, labelName);
	(resCode->u)._if.relop = relopType;
}
void genRETURN(InterCode* resCode, char* opName) {
	resCode->kind = RETURN;
	((resCode->u)._return.op)->kind = VARIABLE;
	(((resCode->u)._return.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u)._return.op)->u)->varName, opName);
}
void genDEC(InterCode resCode, char* opName, int size) {
	resCode->kind = DEC;
	((resCode->u).dec.op)->kind = VARIABLE;
	(((resCode->u).dec.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).dec.op)->u)->varName, opName);
	(resCode->u).dec.size = size;
}
void genARG(InterCode resCode, char* opName) {
	resCode->kind = ARG;
	((resCode->u).arg.op)->kind = VARIABLE;
	(((resCode->u).arg.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).arg.op)->u)->varName, opName);
}
void genCALL(InterCode resCode, char* opName, char* funName) {
	resCode->kind = CALL;
	((resCode->u).call.op)->kind = VARIABLE;
	(((resCode->u).call.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).call.op)->u)->varName, opName);
	((resCode->u).call.function)->kind = FUNCTION;
	(((resCode->u).call.function)->u)->funName = malloc(strlen(funName)+1);
	strcpy((((resCode->u).call.function)->u)->funName, funName);
}
void genPARAM(InterCode resCode, char* opName) {
	resCode->kind = PARAM;
	((resCode->u).param.op)->kind = VARIABLE;
	(((resCode->u).param.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).param.op)->u)->varName, opName);
}
void genREAD(InterCode resCode, char* opName) {
	resCode->kind = READ;
	((resCode->u).read.op)->kind = VARIABLE;
	(((resCode->u).read.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).read.op)->u)->varName, opName);

}
void genWRITE(InterCode resCode, char* opName) {
	resCode->kind = WRITE;
	((resCode->u).write.op)->kind = VARIABLE;
	(((resCode->u).write.op)->u)->varName = malloc(strlen(opName)+1);
	strcpy((((resCode->u).write.op)->u)->varName, opName);
}
void printCode(InterCode code);

InterCodes createInterCodesNode(InterCode code) {
	InterCodes node = malloc(sizeof(struct InterCodes_));
	node->code = code;
	node->prev = NULL;
	node->next = NULL;
}

void insertInterCodesNode(InterCodes codeToInsert) {

}
