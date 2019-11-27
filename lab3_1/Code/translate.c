#include "translate.h"

void genLABEL(InterCode resCode, char* labelName) {
	resCode->kind = LABEL;
	(resCode->u).label.op = malloc(sizeof(struct Operand_));
	((resCode->u).label.op)->kind = LABEL;
	strcpy((((resCode->u).label.op)->u).labelName, labelName); 
}
void genFUNCTION(InterCode resCode, char* funName) {
	resCode->kind = FUNCTION;
	(resCode->u).function.op = malloc(sizeof(struct Operand_));
	((resCode->u).function.op)->kind = FUNCTION;
	strcpy((((resCode->u).function.op)->u).funName, funName);
}
void genASSIGN(InterCode resCode, char* leftName, int leftType, char* rightName, int rightType) { //leftType:0->x, 1->*x; rightType:0->y, 1->*y, 2->&y, 3->constant
	resCode->kind = ASSIGN;
	(resCode->u).assign.left = malloc(sizeof(struct Operand_));
	(resCode->u).assign.right = malloc(sizeof(struct Operand_));
	switch(leftType) {
		case 0: ((resCode->u).assign.left)->kind = VARIABLE; break;
		case 1: ((resCode->u).assign.left)->kind = VAR_GETVALUE; break;
	}
	strcpy((((resCode->u).assign.left)->u).varName, leftName);
	switch(rightType) {
		case 0: {((resCode->u).assign.right)->kind = VARIABLE; strcpy((((resCode->u).assign.right)->u).varName, rightName);}break;
		case 1: {((resCode->u).assign.right)->kind = VAR_GETVALUE; strcpy((((resCode->u).assign.right)->u).varName, rightName);}break;
		case 2: {((resCode->u).assign.right)->kind = VAR_GETADDRESS; strcpy((((resCode->u).assign.right)->u).varName, rightName);}break;
		case 3: {((resCode->u).assign.right)->kind = CONSTANT; strcpy((((resCode->u).assign.right)->u).value, rightName);}break;
	}
	
}
void genBINOP(InterCode resCode, char* op1Name, int op1Type, char* op2Name, int op2Type, char* resultName, int operatorType) { //operatorType-0:+, 1:-, 2:*, 3:/	opType-0:var, 1:*var, 2:&var, 3:cons
	switch(operatorType) {
		case 0: resCode->kind = ADD; break;
		case 1: resCode->kind = SUB; break;
		case 2: resCode->kind = MUL; break;
		case 3: resCode->kind = DIV; break;
	}
	(resCode->u).binop.op1 = malloc(sizeof(struct Operand_));
	(resCode->u).binop.op2 = malloc(sizeof(struct Operand_));
	(resCode->u).binop.result = malloc(sizeof(struct Operand_));
	switch(op1Type) {
		case 0: {((resCode->u).binop.op1)->kind = VARIABLE; strcpy((((resCode->u).binop.op1)->u).varName, op1Name);} break;
		case 1: {((resCode->u).binop.op1)->kind = VAR_GETVALUE; strcpy((((resCode->u).binop.op1)->u).varName, op1Name);}break;
		case 2: {((resCode->u).binop.op1)->kind = VAR_GETADDRESS; strcpy((((resCode->u).binop.op1)->u).varName, op1Name);}break;
		case 3: {((resCode->u).binop.op1)->kind = CONSTANT; strcpy((((resCode->u).binop.op1)->u).value, op1Name);}break;
	}
	switch(op2Type) {
		case 0: {((resCode->u).binop.op2)->kind = VARIABLE; strcpy((((resCode->u).binop.op2)->u).varName, op2Name);} break;
		case 1: {((resCode->u).binop.op2)->kind = VAR_GETVALUE; strcpy((((resCode->u).binop.op2)->u).varName, op2Name);}break;
		case 2: {((resCode->u).binop.op2)->kind = VAR_GETADDRESS; strcpy((((resCode->u).binop.op2)->u).varName, op2Name);}break;
		case 3: {((resCode->u).binop.op2)->kind = CONSTANT; strcpy((((resCode->u).binop.op2)->u).value, op2Name);}break;
	}
	((resCode->u).binop.result)->kind = VARIABLE;
	strcpy((((resCode->u).binop.result)->u).varName, resultName);
}
void genGOTO(InterCode resCode, char* labelName) {
	resCode->kind = GOTO;
	(resCode->u)._goto.op = malloc(sizeof(struct Operand_));
	((resCode->u)._goto.op)->kind = LABEL;
	strcpy((((resCode->u)._goto.op)->u).labelName, labelName);
}
void genIF(InterCode resCode, char* op1Name, int op1Type, char* op2Name, int op2Type, char* labelName, int relopType) {
	resCode->kind = IF;
	(resCode->u)._if.op1 = malloc(sizeof(struct Operand_));
	(resCode->u)._if.op2 = malloc(sizeof(struct Operand_));
	(resCode->u)._if.label = malloc(sizeof(struct Operand_));
	switch(op1Type) {
		case 0: {((resCode->u)._if.op1)->kind = VARIABLE; strcpy((((resCode->u)._if.op1)->u).varName, op1Name);} break;
		case 1: {((resCode->u)._if.op1)->kind = VAR_GETVALUE; strcpy((((resCode->u)._if.op1)->u).varName, op1Name);}break;
		case 2: {((resCode->u)._if.op1)->kind = VAR_GETADDRESS; strcpy((((resCode->u)._if.op1)->u).varName, op1Name);}break;
		case 3: {((resCode->u)._if.op1)->kind = CONSTANT; strcpy((((resCode->u)._if.op1)->u).value, op1Name);}break;
	}
	switch(op2Type) {
		case 0: {((resCode->u)._if.op2)->kind = VARIABLE; strcpy((((resCode->u)._if.op2)->u).varName, op2Name);} break;
		case 1: {((resCode->u)._if.op2)->kind = VAR_GETVALUE; strcpy((((resCode->u)._if.op2)->u).varName, op2Name);}break;
		case 2: {((resCode->u)._if.op2)->kind = VAR_GETADDRESS; strcpy((((resCode->u)._if.op2)->u).varName, op2Name);}break;
		case 3: {((resCode->u)._if.op2)->kind = CONSTANT; strcpy((((resCode->u)._if.op2)->u).value, op2Name);}break;
	}
	((resCode->u)._if.label)->kind = LABEL;
	strcpy((((resCode->u)._if.label)->u).labelName, labelName);
	(resCode->u)._if.relop = relopType;
}
void genRETURN(InterCode resCode, char* opName, int opType) {
	resCode->kind = RETURN;
	(resCode->u)._return.op = malloc(sizeof(struct Operand_));
	switch(opType) {
		case 0: {((resCode->u)._return.op)->kind = VARIABLE; strcpy((((resCode->u)._return.op)->u).varName, opName);} break;
		case 1: {((resCode->u)._return.op)->kind = VAR_GETVALUE; strcpy((((resCode->u)._return.op)->u).varName, opName);}break;
		case 2: {((resCode->u)._return.op)->kind = VAR_GETADDRESS; strcpy((((resCode->u)._return.op)->u).varName, opName);}break;
		case 3: {((resCode->u)._return.op)->kind = CONSTANT; strcpy((((resCode->u)._return.op)->u).value, opName);}break;
	}
}
void genDEC(InterCode resCode, char* opName, char* size) {
	resCode->kind = DEC;
	(resCode->u).dec.op = malloc(sizeof(struct Operand_));
	((resCode->u).dec.op)->kind = VARIABLE;
	strcpy((((resCode->u).dec.op)->u).varName, opName);
	strcpy((resCode->u).dec.size, size);
}
void genARG(InterCode resCode, char* opName, int opType) {
	resCode->kind = ARG;
	(resCode->u).arg.op = malloc(sizeof(struct Operand_));
	switch(opType) {
		case 0: {((resCode->u).arg.op)->kind = VARIABLE; strcpy((((resCode->u).arg.op)->u).varName, opName);} break;
		case 1: {((resCode->u).arg.op)->kind = VAR_GETVALUE; strcpy((((resCode->u).arg.op)->u).varName, opName);}break;
		case 2: {((resCode->u).arg.op)->kind = VAR_GETADDRESS; strcpy((((resCode->u).arg.op)->u).varName, opName);}break;
		case 3: {((resCode->u).arg.op)->kind = CONSTANT; strcpy((((resCode->u).arg.op)->u).value, opName);}break;
	}
}
void genCALL(InterCode resCode, char* opName, char* funName) {
	resCode->kind = CALL;
	(resCode->u).call.op = malloc(sizeof(struct Operand_));
	(resCode->u).call.function = malloc(sizeof(struct Operand_));
	((resCode->u).call.op)->kind = VARIABLE;
	strcpy((((resCode->u).call.op)->u).varName, opName);
	((resCode->u).call.function)->kind = FUNCTION;
	strcpy((((resCode->u).call.function)->u).funName, funName);
}
void genPARAM(InterCode resCode, char* opName) {
	resCode->kind = PARAM;
	(resCode->u).param.op = malloc(sizeof(struct Operand_));
	((resCode->u).param.op)->kind = VARIABLE;
	strcpy((((resCode->u).param.op)->u).varName, opName);
}
void genREAD(InterCode resCode, char* opName) {
	resCode->kind = READ;
	(resCode->u).read.op = malloc(sizeof(struct Operand_));
	((resCode->u).read.op)->kind = VARIABLE;
	strcpy((((resCode->u).read.op)->u).varName, opName);
}
void genWRITE(InterCode resCode, char* opName, int opType) {
	resCode->kind = WRITE;
	(resCode->u).write.op = malloc(sizeof(struct Operand_));
	switch(opType) {
		case 0: {((resCode->u).write.op)->kind = VARIABLE; strcpy((((resCode->u).write.op)->u).varName, opName);} break;
		case 3: {((resCode->u).write.op)->kind = CONSTANT; strcpy((((resCode->u).write.op)->u).value, opName);}break;
	}
}

void printCodes(InterCodes head, char* fileName) {
	FILE *fp = fopen(fileName, "w");
	while(head) {
		switch(head->code->kind) {
			case LABEL: { char t[7] = "LABEL "; fputs(t, fp); fputs((((head->code->u).label.op)->u).labelName, fp); strcpy(t, " :"); fputs(t, fp);} break;
			case FUNCTION: { char t[10] = "FUNCTION "; fputs(t, fp); fputs((((head->code->u).function.op)->u).funName, fp); strcpy(t, " :"); fputs(t, fp);} break;
			case ASSIGN: {
				switch(((head->code->u).assign.left)->kind) {
					case VARIABLE: { fputs((((head->code->u).assign.left)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).assign.left)->u).varName, fp);} break;
				}
				char t[5] = " := "; fputs(t, fp);
				switch(((head->code->u).assign.right)->kind) {
					case VARIABLE: { fputs((((head->code->u).assign.right)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).assign.right)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).assign.right)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).assign.right)->u).value, fp);} break;
				}
				
			} break;
			case ADD: { 
				switch(((head->code->u).binop.result)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.result)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.result)->u).varName, fp);} break;
				}
				char t[5] = " := "; fputs(t, fp);
				switch(((head->code->u).binop.op1)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op1)->u).value, fp);} break;
				}
				strcpy(t, " + "); fputs(t, fp);
				switch(((head->code->u).binop.op2)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op2)->u).value, fp);} break;
				}
			} break;
			case SUB: { 
				switch(((head->code->u).binop.result)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.result)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.result)->u).varName, fp);} break;
				}
				char t[5] = " := "; fputs(t, fp);
				switch(((head->code->u).binop.op1)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op1)->u).value, fp);} break;
				}
				strcpy(t, " - "); fputs(t, fp);
				switch(((head->code->u).binop.op2)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op2)->u).value, fp);} break;
				}
			} break;
			case MUL: { 
				switch(((head->code->u).binop.result)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.result)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.result)->u).varName, fp);} break;
				}
				char t[5] = " := "; fputs(t, fp);
				switch(((head->code->u).binop.op1)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op1)->u).value, fp);} break;
				}
				strcpy(t, " * "); fputs(t, fp);
				switch(((head->code->u).binop.op2)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op2)->u).value, fp);} break;
				}
			} break;
			case DIV: { 
				switch(((head->code->u).binop.result)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.result)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.result)->u).varName, fp);} break;
				}
				char t[5] = " := "; fputs(t, fp);
				switch(((head->code->u).binop.op1)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op1)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op1)->u).value, fp);} break;
				}
				strcpy(t, " / "); fputs(t, fp);
				switch(((head->code->u).binop.op2)->kind) {
					case VARIABLE: { fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).binop.op2)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).binop.op2)->u).value, fp);} break;
				}
			} break;
			case GOTO: { char t[6] = "GOTO "; fputs(t, fp); fputs((((head->code->u)._goto.op)->u).labelName, fp);} break;
			case IF: { 
				char t[7] = "IF "; fputs(t, fp); 
				switch(((head->code->u)._if.op1)->kind) {
					case VARIABLE: { fputs((((head->code->u)._if.op1)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u)._if.op1)->u).value, fp);} break;
				}
				switch((head->code->u)._if.relop) {
					case 0: strcpy(t, " > "); break;
					case 1: strcpy(t, " == "); break;
					case 2: strcpy(t, " < "); break;
					case 3: strcpy(t, " <= "); break;
					case 4: strcpy(t, " >= "); break;
					case 5: strcpy(t, " != "); break;
				}
				fputs(t, fp);
				switch(((head->code->u)._if.op2)->kind) {
					case VARIABLE: { fputs((((head->code->u)._if.op2)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u)._if.op2)->u).value, fp);} break;
				}
				strcpy(t, " GOTO "); fputs(t, fp);
				fputs((((head->code->u)._if.label)->u).labelName, fp);
			} break;
			case RETURN: { 
				char t[8] = "RETURN "; fputs(t, fp); 
				switch(((head->code->u)._return.op)->kind) {
					case VARIABLE: { fputs((((head->code->u)._return.op)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u)._return.op)->u).value, fp);} break;
				}
			} break;
			case DEC: { char t[5] = "DEC "; fputs(t, fp); fputs((((head->code->u).dec.op)->u).varName, fp); fputc(' ', fp); fputs((head->code->u).dec.size, fp);} break;
			case ARG: { 
				char t[5] = "ARG "; fputs(t, fp); 
				switch(((head->code->u).arg.op)->kind) {
					case VARIABLE: { fputs((((head->code->u).arg.op)->u).varName, fp);} break;
					case VAR_GETVALUE: { fputc('*', fp); fputs((((head->code->u).arg.op)->u).varName, fp);} break;
					case VAR_GETADDRESS: { fputc('&', fp); fputs((((head->code->u).arg.op)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).arg.op)->u).value, fp);} break;
				} break;
			} break;
			case CALL: { fputs((((head->code->u).call.op)->u).varName, fp); char t[10] = " := CALL "; fputs(t, fp); fputs((((head->code->u).call.function)->u).funName, fp);} break;
			case PARAM: { char t[7] = "PARAM "; fputs(t, fp); fputs((((head->code->u).param.op)->u).varName, fp);} break;
			case READ: { char t[6] = "READ "; fputs(t, fp); fputs((((head->code->u).read.op)->u).varName, fp);} break;
			case WRITE: { 
				char t[7] = "WRITE "; fputs(t, fp); 
				switch(((head->code->u).write.op)->kind) {
					case VARIABLE: { fputs((((head->code->u).write.op)->u).varName, fp);} break;
					case CONSTANT: { fputc('#', fp); fputs((((head->code->u).write.op)->u).value, fp);} break;
				}
			} break;
		}
		fputc('\n', fp);
		head = head->next;
	}
	fclose(fp);
}

InterCodes createInterCodesNode(InterCode code) {
	InterCodes node = malloc(sizeof(struct InterCodes_));
	node->code = code;
	node->prev = NULL;
	node->next = NULL;
}

void insertInterCodesNodes(InterCodes* head, InterCodes* tail, InterCodes nodesToInsert) {
	if((*head)==(*tail)&&(*head)==NULL) {
		(*head) = nodesToInsert;
		(*tail) = (*head);
		while((*tail)->next) (*tail) = (*tail)->next;
	}
	else {
		(*tail)->next = nodesToInsert;
		nodesToInsert->prev = (*tail);
		while((*tail)->next) (*tail) = (*tail)->next;
	}
}

void test_gen(char* fileName) {
	InterCode resCode = malloc(sizeof(struct InterCode_));
	char name1[32] = "12";
	char name2[32] = "func1";
	char name3[32] = "label4";
	genWRITE(resCode, name1, 3);
	InterCodes node = createInterCodesNode(resCode);
	insertInterCodesNodes(&head, &tail, node);

	InterCode resCode1 = malloc(sizeof(struct InterCode_));
	strcpy(name3, "2");
	genRETURN(resCode1, name3, 3);
	InterCodes node1 = createInterCodesNode(resCode1);
	insertInterCodesNodes(&head, &tail, node1);

	printCodes(head, fileName);
printf("%s\n", name2);
}
