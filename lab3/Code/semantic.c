#include <string.h>
#include "semantic.h"

int anonymousStructCnt = 0;
void printType(Type type) {
	switch(type->kind) {
		case BASIC: {
			if((type->u).basic==0)
				printf("int");
			else if((type->u).basic==1)
				printf("float");
		}break;
		case ARRAY: {
			printType((type->u).array.elem);
			printf("[%d]", (type->u).array.size);
		}break;
		case STRUCTURE: {
			printf("struct %s", (type->u).structure.structName);
		}break;
	}
}
void typecpy(Type dst, Type src) {
	if(!src) return ;
	dst->kind = src->kind;
	switch(src->kind) {
		case BASIC: {
			(dst->u).basic = (src->u).basic;
		}break;
		case ARRAY: {
			if(!((dst->u).array.elem)) {
				(dst->u).array.elem = malloc(sizeof(struct Type_));
				typecpy((dst->u).array.elem, (src->u).array.elem);
				(dst->u).array.size = (src->u).array.size;
			}
		}break;
		case STRUCTURE: {
			(dst->u).structure.structName = malloc(strlen((src->u).structure.structName)+1);
			strcpy((dst->u).structure.structName, (src->u).structure.structName);

			FieldList* tmpSrcFl = &((src->u).structure.fieldList);
			FieldList* tmpDstFl = &((dst->u).structure.fieldList);
			while(*tmpSrcFl) {
				*tmpDstFl = malloc((sizeof(struct FieldList_)));
				(*tmpDstFl)->name = malloc(strlen((*tmpSrcFl)->name)+1);
				strcpy((*tmpDstFl)->name, (*tmpSrcFl)->name);
				(*tmpDstFl)->type = malloc((sizeof(struct Type_)));
				typecpy((*tmpDstFl)->type, (*tmpSrcFl)->type);
				(*tmpDstFl)->tail = NULL;
				
				tmpSrcFl = &((*tmpSrcFl)->tail);
				tmpDstFl = &((*tmpDstFl)->tail);
			}
		}break;
	}
}

int typecmp(Type dst, Type src) {
	if(dst->kind!=src->kind) return 1;
	switch(dst->kind) {
		case BASIC: {
			if((dst->u).basic!=(src->u).basic) return 1;
		}break;
		case ARRAY:{
			if(typecmp((dst->u).array.elem, (src->u).array.elem)!=0) return 1;
		}break;
		case STRUCTURE: {
			if(strcmp((dst->u).structure.structName, (src->u).structure.structName)!=0) return 1;
		}break;
	}
	return 0;
}

unsigned int hash_pjw(char* name) {
	unsigned int val = 0, i;
	for(; *name; ++name) {
		val = (val<<2)+*name;
		if(i = val & ~0x3fff)
			val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}

HashNode getSybtabNode(char* name) {
	unsigned int sybtabId = hash_pjw(name);
	HashNode sybtabNode = sybtab[sybtabId];
	while(sybtabNode&&strcmp((sybtabNode->symbol)->name, name)!=0) {
		sybtabNode = sybtabNode->next;
	}
	return sybtabNode;
}

_Bool isInHashTable(char* name) {
	unsigned int idSybtabId = hash_pjw(name);
	HashNode tmpHashNode = sybtab[idSybtabId];
	if(!tmpHashNode) return false;

	while(strcmp(tmpHashNode->symbol->name, name)!=0) {
		tmpHashNode = tmpHashNode->next;
	}
	return (tmpHashNode!=NULL);
}

void initHashTable() {
	int i = 0;
	for(i = 0; i < SYBTAB_SIZE; ++i) {
		sybtab[i] = NULL;
	}
}

HashNode addSybtabNode(char* name, int kind, Type type, int argNum, ArgList args) {//kind:0->VAR, 1->FUN, 2->STRUCTDEF
	unsigned int sybtabId = hash_pjw(name);
	HashNode* tmpHashNode = &sybtab[sybtabId];
	while(*tmpHashNode) {						//check if there is repetition of var names
			*tmpHashNode = (*tmpHashNode)->next;
	}
	*tmpHashNode = malloc(sizeof(struct HashNode_));
	(*tmpHashNode)->symbol = malloc(sizeof(struct Symbol_));
	(*tmpHashNode)->next = NULL;

	(*tmpHashNode)->symbol->name = malloc(strlen(name)+1);
	strcpy((*tmpHashNode)->symbol->name, name);

	(*tmpHashNode)->symbol->type = malloc(sizeof(struct Type_));
	memset((void*)((*tmpHashNode)->symbol->type), 0, sizeof(struct Type_));
	typecpy((*tmpHashNode)->symbol->type, type);
	
	if(kind==0) {
		(*tmpHashNode)->symbol->kind = VAR;
	}
	else if(kind==1) {
		(*tmpHashNode)->symbol->kind = FUN;
		((*tmpHashNode)->symbol->symInfo).funInfo.argNum = argNum;
		ArgList* tmpArgList = &((((*tmpHashNode)->symbol->symInfo).funInfo).args);
		while(args) {
			*tmpArgList = malloc(sizeof(struct ArgList_));
			if(args->name) {
				(*tmpArgList)->name = malloc(strlen(args->name)+1);
				strcpy((*tmpArgList)->name, args->name);
			}
			(*tmpArgList)->type = malloc(sizeof(struct Type_));
			typecpy((*tmpArgList)->type, args->type);
			args = args->tail;
			tmpArgList = &((*tmpArgList)->tail);
		}
	}
	else if(kind==2) {
		(*tmpHashNode)->symbol->kind = STRUCTDEF;
		(*tmpHashNode)->symbol->type = malloc(sizeof(struct Type_));
		typecpy((*tmpHashNode)->symbol->type, type);
	}
	return (*tmpHashNode);
}

void traverseTree(GrammarTree* root) {
	char *funName1 = malloc(strlen("read")+1), *funName2 = malloc(strlen("write")+1);
	strcpy(funName1, "read"); 
	strcpy(funName2, "write");
	Type retType1 = malloc(sizeof(struct Type_));
	retType1->kind = BASIC;
	(retType1->u).basic = 0;
	Type retType2 = malloc(sizeof(struct Type_));
	retType2->kind = BASIC;
	(retType2->u).basic = 0;
	ArgList args2 = malloc(sizeof(struct ArgList_));
	args2->type = malloc(sizeof(struct Type_));
	args2->name = NULL;
	args2->type->kind = BASIC;
	(args2->type->u).basic = 0;
	addSybtabNode(funName1, 1, retType1, 0, NULL);
	addSybtabNode(funName2, 1, NULL, 1, args2);

	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "ExtDefList"
	if(tmpRoot)				//"ExtDefList" is possible to be NULL
		traverseExtDefList(tmpRoot);
}

void traverseExtDefList(GrammarTree* root) {	
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "ExtDef"
	traverseExtDef(tmpRoot);
	tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "ExtDefList"
	if(tmpRoot) {					//"ExtDefList" is possible to be NULL
		traverseExtDefList(tmpRoot);
	}
}

void traverseExtDef(GrammarTree* root) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "Specifier"

	Type specifier = malloc(sizeof(struct Type_));
	memset((void*)specifier, 0, sizeof(struct Type_));
	traverseSpecifier(tmpRoot, specifier);

	tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "ExtDecList"/"SEMI"/"FunDec"
	if(strcmp(tmpRoot->idName, "ExtDecList")==0) {		//'tmpRoot' points to "ExtDecList"
		traverseExtDecList(tmpRoot, specifier);
	}
	else if(strcmp(tmpRoot->idName, "FunDec")==0) {		//'tmpRoot' points to "FunDec"
		HashNode funSybtabNode = traverseFunDec(tmpRoot, specifier);
		if(funSybtabNode) {				//function defined successfully in symbol table
			tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to "CompSt"
			traverseCompSt(tmpRoot, funSybtabNode);			//pass the funSybtabNode to check if the return type in 'CompSt' is correct
		}
	}
	else if(strcmp(tmpRoot->idName, "SEMI")==0) {		//define a structure
		if((specifier->u).structure.structName) {
			addSybtabNode((specifier->u).structure.structName, 2, specifier, 0, NULL);
		}
	}
}

void traverseExtDecList(GrammarTree* root, Type varDecType) {
	GrammarTree* tmpRoot = root->childNode;	//'tmpRoot' points to "VarDec"
	traverseVarDec(tmpRoot, varDecType, NULL, false, NULL, false);
	tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to "COMMA"/NULL
	if(tmpRoot) {
		tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to "ExtDecList"
		traverseExtDecList(tmpRoot, varDecType);
	}
}

void traverseSpecifier(GrammarTree* root, Type res) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "TYPE"/"StructSpecifier"
	if(strcmp(tmpRoot->idName, "TYPE")==0) {
		res->kind = BASIC;
		if(strcmp(tmpRoot->content, "int")==0) {
			(res->u).basic = 0;
		}
		else if(strcmp(tmpRoot->content, "float")==0) {
			(res->u).basic = 1;
		}
	}
	else if(strcmp(tmpRoot->idName, "StructSpecifier")==0) {
		res->kind = STRUCTURE;
		(res->u).structure.fieldList = NULL;
		(res->u).structure.structName = NULL;
		traverseStructSpecifier(tmpRoot, &((res->u).structure.fieldList), &((res->u).structure.structName));
	}
}

void traverseStructSpecifier(GrammarTree* root, FieldList* resFieldList, char** resStructName) {
	GrammarTree* tmpRoot = root->childNode->siblingNodes;		//'tmpRoot' points to "OptTag"/"LC"/"Tag"
	if(strcmp(tmpRoot->idName, "OptTag")==0) {
		GrammarTree* tmpTmpRoot = tmpRoot->childNode;		//'tmpRoot' points to "ID"
		if(isInHashTable(tmpTmpRoot->content)) {
			printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", tmpRoot->lineNum, tmpTmpRoot->content);
			*resFieldList = NULL;
			*resStructName = NULL;
			return ;
		}
		*resStructName = malloc(strlen(tmpTmpRoot->content)+1);
		strcpy(*resStructName, tmpTmpRoot->content);
		tmpRoot = tmpRoot->siblingNodes->siblingNodes;	//'tmpRoot' points to "DefList"
		traverseDefList(tmpRoot, resFieldList, true);	//get structure's fields
	}
	else if(strcmp(tmpRoot->idName, "LC")==0) {		//anonymous struct
		char* anonymousStructCnt_str = malloc(6);
		sprintf(anonymousStructCnt_str, "%d", anonymousStructCnt);
		++anonymousStructCnt;
		*resStructName = malloc(strlen("@struct")+strlen(anonymousStructCnt_str)+1);
		strcpy(*resStructName, "@struct");
		strcat(*resStructName, anonymousStructCnt_str);
		tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "DefList"/"RC"
		if(strcmp(tmpRoot->idName, "DefList")==0) {
			traverseDefList(tmpRoot, resFieldList, true);
		}
	}
	else if(strcmp(tmpRoot->idName, "Tag")==0) {
		tmpRoot = tmpRoot->childNode;		//'tmpRoot' points to "ID"
		if(isInHashTable(tmpRoot->content)) {
			*resStructName = malloc(strlen(tmpRoot->content)+1);
			strcpy(*resStructName, tmpRoot->content);
			unsigned int structSybtabId = hash_pjw(tmpRoot->content);
			HashNode tmpHashNode = sybtab[structSybtabId];
			while(strcmp(tmpHashNode->symbol->name, tmpRoot->content)!=0) {
				tmpHashNode = tmpHashNode->next;
			}
			FieldList sybtabFl = (tmpHashNode->symbol->type->u).structure.fieldList;
			while(sybtabFl) {
				*resFieldList = malloc(sizeof(struct FieldList_));
				(*resFieldList)->name = malloc(strlen(sybtabFl->name)+1);
				strcpy((*resFieldList)->name, sybtabFl->name);
				(*resFieldList)->type = malloc(sizeof(struct Type_));
				typecpy((*resFieldList)->type, sybtabFl->type);
				(*resFieldList)->tail = NULL;

				resFieldList = &((*resFieldList)->tail);
				sybtabFl = sybtabFl->tail;
			}
		}
		else {
			printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);
			*resFieldList = NULL;
			*resStructName = NULL;
		}
	}
}

HashNode traverseVarDec(GrammarTree* root, Type varDecType, FieldList* resFieldList, _Bool fillFieldList, ArgList* resArgList, _Bool fillArgList) {		//add variable's sybtab record here
	GrammarTree* tmpRoot = root->childNode;
	if(strcmp(tmpRoot->idName, "ID")==0) {
		unsigned int varSybtabId = hash_pjw(tmpRoot->content);
		HashNode* tmpHashNode = &sybtab[varSybtabId];
		while(*tmpHashNode) {						//check if there is repetition of var names
			if(strcmp((*tmpHashNode)->symbol->name, tmpRoot->content)==0) {
				if(fillFieldList) {printf("Error type 15 at Line %d: Redefined field \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);}
				else {printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);}
				return NULL;
			}
			*tmpHashNode = (*tmpHashNode)->next;
		}
		if(fillFieldList) {
			FieldList* tmpFl = resFieldList;
			while((*tmpFl)) {tmpFl = &((*tmpFl)->tail);}
			(*tmpFl) = malloc(sizeof(struct FieldList_));
			(*tmpFl)->name = malloc(strlen(tmpRoot->content)+1);
			strcpy((*tmpFl)->name, tmpRoot->content);
			(*tmpFl)->type = malloc(sizeof(struct Type_));
			memset((void*)((*tmpFl)->type), 0, sizeof(struct Type_));
			typecpy((*tmpFl)->type, varDecType);
			(*tmpFl)->tail = NULL;
		}
		if(fillArgList) {
			ArgList* tmpAl = resArgList;
			while((*tmpAl)) {tmpAl = &((*tmpAl)->tail);}
			(*tmpAl) = malloc(sizeof(struct ArgList_));
			(*tmpAl)->name = malloc(strlen(tmpRoot->content)+1);
			strcpy((*tmpAl)->name, tmpRoot->content);
			(*tmpAl)->type = malloc(sizeof(struct Type_));
			memset((void*)((*tmpAl)->type), 0, sizeof(struct Type_));
			typecpy((*tmpAl)->type, varDecType);
			(*tmpAl)->tail = NULL;
		}
		return addSybtabNode(tmpRoot->content, 0, varDecType, 0, NULL);
	}
	else if(strcmp(tmpRoot->idName, "VarDec")==0) {
		Type tmpVarDecType = malloc(sizeof(struct Type_));
		memset((void*)tmpVarDecType, 0, sizeof(struct Type_));
		tmpVarDecType->kind = ARRAY;
		(tmpVarDecType->u).array.elem = malloc(sizeof(struct Type_));
		typecpy((tmpVarDecType->u).array.elem, varDecType);
		(tmpVarDecType->u).array.size = atoi(tmpRoot->siblingNodes->siblingNodes->content);
		return traverseVarDec(tmpRoot, tmpVarDecType, resFieldList, fillFieldList, resArgList, fillArgList);
	}
}

HashNode traverseFunDec(GrammarTree* root, Type retType) {			//add function's sybtab record here TODO:change the ret type of this function to HashNode
	GrammarTree* tmpRoot = root->childNode;			//'tmpRoot' points to "ID"
	unsigned int funSybtabId = hash_pjw(tmpRoot->content);
	HashNode* tmpHashNode = &sybtab[funSybtabId];
	while(*tmpHashNode) {						//check if there is repetition of var/fun names
		if(strcmp((*tmpHashNode)->symbol->name, tmpRoot->content)==0) {
			printf("Error type 4 at Line %d: Redefined function \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);
			return NULL;
		}
		*tmpHashNode = (*tmpHashNode)->next;
	}
	char* funName = malloc(strlen(tmpRoot->content)+1);
	strcpy(funName, tmpRoot->content);
	int argNum = 0;
	ArgList args = NULL;

	tmpRoot = tmpRoot->siblingNodes->siblingNodes;		//'tmpRoot' points to "VarList"/"RP"
	if(strcmp(tmpRoot->idName, "VarList")==0) {
		traverseVarList(tmpRoot, &argNum, &args);
	}
	return addSybtabNode(funName, 1, retType, argNum, args);
}

void traverseVarList(GrammarTree* root, int* argNum, ArgList* args) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "ParamDec"
	traverseParamDec(tmpRoot, args);
	++(*argNum);

	tmpRoot = tmpRoot->siblingNodes;
	if(tmpRoot) {
		tmpRoot = tmpRoot->siblingNodes;
		traverseParamDec(tmpRoot, args);
		++(*argNum);
	}
}

void traverseParamDec(GrammarTree* root, ArgList* args) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "Specifier"
	Type specifier = malloc(sizeof(struct Type_));
	traverseSpecifier(tmpRoot, specifier);

	tmpRoot = tmpRoot->siblingNodes;
	traverseVarDec(tmpRoot, specifier, NULL, false, args, true);
}

void traverseCompSt(GrammarTree* root, HashNode funSybtabNode) {
	GrammarTree* tmpRoot = root->childNode->siblingNodes;		//'tmpRoot' points to "DefList"/"StmtList"/"RC"
	if(strcmp(tmpRoot->idName, "DefList")==0) {
		traverseDefList(tmpRoot, NULL, false);
		tmpRoot = tmpRoot->siblingNodes;			//'tmpRoot' points to "StmtList"/"RC"
		if(strcmp(tmpRoot->idName, "StmtList")==0) {
			traverseStmtList(tmpRoot, funSybtabNode);
		}
	}
	else if(strcmp(tmpRoot->idName, "StmtList")==0) {
		traverseStmtList(tmpRoot, funSybtabNode);
	}
}

void traverseStmtList(GrammarTree* root, HashNode funSybtabNode) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "Stmt"
	traverseStmt(tmpRoot, funSybtabNode);
	tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "StmtList"/NULL
	if(tmpRoot) {
		traverseStmtList(tmpRoot, funSybtabNode);
	}
}

void traverseStmt(GrammarTree* root, HashNode funSybtabNode) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to ""
	if(strcmp(tmpRoot->idName, "Exp")==0) {
		traverseExp(tmpRoot, NULL, NULL);
	}
	else if(strcmp(tmpRoot->idName, "CompSt")==0) {
		traverseCompSt(tmpRoot, funSybtabNode);
	}
	else if(strcmp(tmpRoot->idName, "RETURN")==0) {
		tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to "Exp"
		char* tmpIdName;
		int expType = traverseExp(tmpRoot, &tmpIdName, NULL);
		if(expType == 2) {
			expType = (getSybtabNode(tmpIdName)->symbol->type->u).basic;
		}
		if(expType!=((funSybtabNode->symbol->type)->u).basic) {
			printf("Error type 8 at Line %d: Type mismatched for return.\n", tmpRoot->lineNum);
		}
	}
	else if(strcmp(tmpRoot->idName, "IF")==0) {
		tmpRoot = tmpRoot->siblingNodes->siblingNodes;	//'tmpRoot' points to "Exp"
		char* tmpIdName;
		int expType = traverseExp(tmpRoot, &tmpIdName, NULL);
		if(expType==2) {expType = (getSybtabNode(tmpIdName)->symbol->type->u).basic;}
		if(expType > 0) {		//expType is not int
			printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
		}
		tmpRoot = tmpRoot->siblingNodes->siblingNodes;	//'tmpRoot' points to "Stmt"
		traverseStmt(tmpRoot, funSybtabNode);
		tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to NULL/"ELSE"
		if(tmpRoot) {
			tmpRoot = tmpRoot->siblingNodes;
			traverseStmt(tmpRoot, funSybtabNode);
		}
		
	}
	else if(strcmp(tmpRoot->idName, "WHILE")==0) {
		tmpRoot = tmpRoot->siblingNodes->siblingNodes;	//'tmpRoot' points to "Exp"
		char* tmpIdName;
		int expType = traverseExp(tmpRoot, &tmpIdName, NULL);
		if(expType==2) {expType = (getSybtabNode(tmpIdName)->symbol->type->u).basic;}
		if(expType > 0) {		//expType is not int
			printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
		}
		tmpRoot = tmpRoot->siblingNodes->siblingNodes;	//'tmpRoot' points to "Stmt"
		traverseStmt(tmpRoot, funSybtabNode);
	}
}

void traverseDefList(GrammarTree* root, FieldList* resFieldList, _Bool fillFieldList) {
	GrammarTree* tmpRoot = root->childNode;
	traverseDef(tmpRoot, resFieldList, fillFieldList);
	tmpRoot = tmpRoot->siblingNodes;
	if(tmpRoot) {
		traverseDefList(tmpRoot, resFieldList, fillFieldList);
	}
}

void traverseDef(GrammarTree* root, FieldList* resFieldList, _Bool fillFieldList) {
	GrammarTree* tmpRoot = root->childNode;			//'tmpRoot' points to "Specifier"
	Type varDefType = malloc(sizeof(struct Type_));
	traverseSpecifier(tmpRoot, varDefType);
	if(varDefType->kind==STRUCTURE&&(!((varDefType->u).structure.structName))) {	//Undefined structure
		return ;
	}
	tmpRoot = tmpRoot->siblingNodes;
	traverseDecList(tmpRoot, varDefType, resFieldList, fillFieldList);
}

void traverseDecList(GrammarTree* root, Type varDefType, FieldList* resFieldList, _Bool fillFieldList) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "Dec"
	traverseDec(tmpRoot, varDefType, resFieldList, fillFieldList);
	tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "COMMA"/NULL
	if(tmpRoot) {		//'tmpRoot' points to "COMMA"
		tmpRoot	= tmpRoot->siblingNodes;
		traverseDecList(tmpRoot, varDefType, resFieldList, fillFieldList);
	}
}

void traverseDec(GrammarTree* root, Type varDecType, FieldList* resFieldList, _Bool fillFieldList) {
	GrammarTree* tmpRoot = root->childNode;		//'tmpRoot' points to "VarDec"
	traverseVarDec(tmpRoot, varDecType, resFieldList, fillFieldList, NULL, false);
	tmpRoot = tmpRoot->siblingNodes;
	if(tmpRoot) {		//'tmpRoot' points to "ASSIGNOP"
		tmpRoot = tmpRoot->siblingNodes;	//'tmpRoot' points to "Exp"
		char* tmpIdName;
		int basicType = traverseExp(tmpRoot, &tmpIdName, NULL);
		if(basicType != (varDecType->u).basic) {
			printf("error: initial value doesn't match variable type\n");
		}
	}
}

int traverseExp(GrammarTree* root, char** resIdName, _Bool* isLeftExp) {		//return "Exp"'s type: 0->int; 1->float; 2->ID
	GrammarTree* tmpRoot = root->childNode;
	if(strcmp(tmpRoot->idName, "INT")==0) {
		if(isLeftExp) *isLeftExp = false;
		*resIdName = malloc(strlen(tmpRoot->content)+1);
		strcpy(*resIdName, tmpRoot->content);
		return 0;
	}
	else if(strcmp(tmpRoot->idName, "FLOAT")==0) {
		if(isLeftExp) *isLeftExp = false;
		*resIdName = malloc(strlen(tmpRoot->content)+1);
		strcpy(*resIdName, tmpRoot->content);
		return 1;
	}
	else if(strcmp(tmpRoot->idName, "ID")==0) {
		if(!isInHashTable(tmpRoot->content)) {
			if(!(tmpRoot->siblingNodes))	{printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);}
			else				{printf("Error type 2 at Line %d: Undefined function \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);}
			if(isLeftExp) *isLeftExp = false;
//if(strcmp(tmpRoot->content, "temp")==0)
//	printf("temp\n");
			return -1;
		}
		if(!(tmpRoot->siblingNodes)) {
			if(isLeftExp) *isLeftExp = true;
			*resIdName = malloc(strlen(tmpRoot->content)+1);
			strcpy(*resIdName, tmpRoot->content);
			return 2;
		}
		else {		//function call
			if(isLeftExp) *isLeftExp = false;
			char* tmpFunName;
			tmpFunName = malloc(strlen(tmpRoot->content)+1);
			strcpy(tmpFunName, tmpRoot->content);
			HashNode tmpHashNode = getSybtabNode(tmpFunName);
			if(tmpHashNode->symbol->kind==FUN) {
				tmpRoot = tmpRoot->siblingNodes->siblingNodes;	
				if(strcmp(tmpRoot->idName, "Args")==0) {		//'tmpRoot' points to "Args"	
					ArgList actualArgList;
					traverseArgs(tmpRoot, &actualArgList);

					ArgList tmpActAl = actualArgList;
					ArgList tmpSupAl = (tmpHashNode->symbol->symInfo).funInfo.args;		//supposed arglist
					_Bool matchF = true;
					while(tmpActAl&&tmpSupAl) {
						if(typecmp(tmpActAl->type, tmpSupAl->type)!=0) {matchF = false;}
						tmpActAl = tmpActAl->tail;
						tmpSupAl = tmpSupAl->tail;
					}
					if(tmpActAl||tmpSupAl)
						matchF = false;
					if(!matchF) {			//print Error Information
						printf("Error type 9 at Line %d: Function \"%s(", tmpRoot->lineNum, tmpFunName);
						tmpSupAl = (tmpHashNode->symbol->symInfo).funInfo.args;
						if(tmpSupAl) {
							printType(tmpSupAl->type);
							tmpSupAl = tmpSupAl->tail;
						}
						while(tmpSupAl) {
							printf(", "); printType(tmpSupAl->type);	
							tmpSupAl = tmpSupAl->tail;
						}
						printf(")\" is not applicable for arguments \"(");
						tmpActAl = actualArgList;
						if(tmpActAl) {
							printType(tmpActAl->type);
							tmpActAl = tmpActAl->tail;
						}
						while(tmpActAl) {
							printf(", "); printType(tmpActAl->type);	
							tmpActAl = tmpActAl->tail;
						}
						printf(")\".\n");
						return -1;
					}	
				}
				else {
					if((tmpHashNode->symbol->symInfo).funInfo.argNum>0) {
						printf("error: too few args!\n");
						return -1;
					}
				}
				return (tmpHashNode->symbol->type->u).basic;
			}
			else {
				printf("Error type 11 at Line %d: \"%s\" is not a function.\n", tmpRoot->lineNum, tmpFunName);
				return -1;
			}		
		}
	}
	else if(strcmp(tmpRoot->idName, "LP")==0) {
		tmpRoot = tmpRoot->siblingNodes;		
		return traverseExp(tmpRoot, resIdName, isLeftExp);
	}
	else if(strcmp(tmpRoot->idName, "MINUS")==0) {
		tmpRoot = tmpRoot->siblingNodes;	
		if(isLeftExp) *isLeftExp = false;	
		return traverseExp(tmpRoot, resIdName, NULL);
	}
	else if(strcmp(tmpRoot->idName, "NOT")==0) {
		tmpRoot = tmpRoot->siblingNodes;
		char* tmpIdName;
		int expType = traverseExp(tmpRoot, &tmpIdName, NULL);
		if(expType==1) {
			printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
			return -1;
		}
		else if(expType==2) {
			unsigned int idSybtabId = hash_pjw(tmpIdName);
			HashNode tmpHashNode = sybtab[idSybtabId];
			while(strcmp(tmpHashNode->symbol->name, tmpIdName)!=0) {
				tmpHashNode = tmpHashNode->next;
			}
			if((tmpHashNode->symbol->type->u).basic==1) {
				printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
				return -1;
			}
		}
		if(isLeftExp) *isLeftExp = false;
		return 0;
	}
	else if(strcmp(tmpRoot->idName, "Exp")==0) {
		char* tmpIdName1, *tmpIdName2;
		_Bool isLeftExp1, isLeftExp2;
		int exp1Type = traverseExp(tmpRoot, &tmpIdName1, &isLeftExp1), exp2Type;

		if(strcmp(tmpRoot->siblingNodes->siblingNodes->idName, "Exp")==0) {	//tmpRoot->siblingNodes->siblingNodes: "Exp"
			exp2Type = traverseExp(tmpRoot->siblingNodes->siblingNodes, &tmpIdName2, &isLeftExp2);	
		} 
		else {									//tmpRoot->siblingNodes->siblingNodes: "ID"
			exp2Type = 2;
			tmpIdName2 = malloc(strlen(tmpRoot->siblingNodes->siblingNodes->content)+1);
			strcpy(tmpIdName2, tmpRoot->siblingNodes->siblingNodes->content);
		}
		tmpRoot = tmpRoot->siblingNodes;
		if(strcmp(tmpRoot->idName, "ASSIGNOP")==0) {
			if(isLeftExp) *isLeftExp = false;
			if(exp1Type==2) {exp1Type = (getSybtabNode(tmpIdName1)->symbol->type->u).basic;}
			if(exp2Type==2) {exp2Type = (getSybtabNode(tmpIdName2)->symbol->type->u).basic;}

			if(exp1Type>=0 && exp2Type>=0) {
				if(exp1Type!=exp2Type) {
					printf("Error type 5 at Line %d: Type mismatched for assignment.\n", tmpRoot->lineNum);
					return -1;
				}
				if(!isLeftExp1) {
					printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", tmpRoot->lineNum);
					return -1;
				}
			}

			return exp1Type;
		}
		else if(strcmp(tmpRoot->idName, "RELOP")==0) {
			if(isLeftExp) *isLeftExp = false;
			if(exp1Type==2) {exp1Type = (getSybtabNode(tmpIdName1)->symbol->type->u).basic;}
			if(exp2Type==2) {exp2Type = (getSybtabNode(tmpIdName2)->symbol->type->u).basic;}
			if(exp1Type>=0 && exp2Type>=0) {
				if(exp1Type!=exp2Type) {
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
					return -1;
				}
				return 0;
			}
			else return -1;	
		}
		else if(strcmp(tmpRoot->idName, "LB")==0) {
			if(isLeftExp) *isLeftExp = true;
			if(exp2Type==2) {exp2Type = (getSybtabNode(tmpIdName2)->symbol->type->u).basic;}
			if(exp2Type!=0) {
				printf("Error type 12 at Line %d: \"%s\" is not an integer.\n", tmpRoot->lineNum, tmpIdName2);
				return -1;
			}
			if(exp1Type==2) {
				if(getSybtabNode(tmpIdName1)->symbol->type->kind!=ARRAY) {
					printf("Error type 10 at Line %d: \"%s\" is not an array.\n", tmpRoot->lineNum, tmpIdName1);
					return -1;
				}
				else {
					HashNode tmpHashNode = getSybtabNode(tmpIdName1);
					if((tmpHashNode->symbol->type->u).array.elem->kind==BASIC) {return ((tmpHashNode->symbol->type->u).array.elem->u).basic;}
					else {return 2;}			//TODO:what if the array.elem is an array/struct? not demanded->not processed
				}
			}
			
		}
		else if(strcmp(tmpRoot->idName, "DOT")==0) {
			if(isLeftExp) *isLeftExp = true;
			if(exp1Type==2) {
				if(getSybtabNode(tmpIdName1)->symbol->type->kind!=STRUCTURE) {
					printf("Error type 13 at Line %d: Illegal use of \".\".\n", tmpRoot->lineNum);
					return -1;
				}
				tmpRoot = tmpRoot->siblingNodes;		//'tmpRoot' points to "ID"
				HashNode tmpHashNode = getSybtabNode(tmpIdName1);
				FieldList tmpFl = (tmpHashNode->symbol->type->u).structure.fieldList;
				while(tmpFl&&strcmp(tmpFl->name, tmpRoot->content)!=0) {tmpFl = tmpFl->tail;}
				if(!tmpFl) {
					printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", tmpRoot->lineNum, tmpRoot->content);
					return -1;
				}
				else {
					*resIdName = malloc(strlen(tmpRoot->content)+1);
					strcpy(*resIdName, tmpRoot->content);
					return 2;
				}
			}
			else {
				printf("Error type 13 at Line %d: Illegal use of \".\".\n", tmpRoot->lineNum);
				return -1;
			}
		}
		else if(strcmp(tmpRoot->idName, "AND")==0||strcmp(tmpRoot->idName, "OR")==0) {
			if(isLeftExp) *isLeftExp = false;
			if(exp1Type==2) {exp1Type = (getSybtabNode(tmpIdName1)->symbol->type->u).basic;}
			if(exp2Type==2) {exp2Type = (getSybtabNode(tmpIdName2)->symbol->type->u).basic;}
			if(exp1Type>=0 && exp2Type>=0) {
				if(exp1Type!=exp2Type) {
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum); 
					return -1;
				}
				else if(exp1Type==1) {
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum); 
					return -1;
				}
				return exp1Type;
			}
			else return -1;
		}
		else {				//"PLUS"/"MINUS"/"STAR"/"DIV"
			if(isLeftExp) *isLeftExp = false;
			if(exp1Type==2) {exp1Type = (getSybtabNode(tmpIdName1)->symbol->type->u).basic;}
			if(exp2Type==2) {exp2Type = (getSybtabNode(tmpIdName2)->symbol->type->u).basic;}
			if(exp1Type>=0 && exp2Type>=0) {
				if(exp1Type!=exp2Type) {
					printf("Error type 7 at Line %d: Type mismatched for operands.\n", tmpRoot->lineNum);
					return -1;
				}
				return exp1Type;
			}
			else return -1;	
		}
	}
}

void traverseArgs(GrammarTree* root, ArgList* resArgs) {
	GrammarTree* tmpRoot = root->childNode;		//"Exp"
	*resArgs = malloc(sizeof(struct ArgList_));
	(*resArgs)->type = malloc(sizeof(struct Type_));
	char* tmpIdName;
	int expType = traverseExp(tmpRoot, &tmpIdName, NULL);
	if(expType==2) {
		(*resArgs)->name = malloc(strlen(tmpIdName)+1);
		strcpy((*resArgs)->name, tmpIdName);
		HashNode tmpHashNode = getSybtabNode(tmpIdName);
		typecpy((*resArgs)->type, tmpHashNode->symbol->type);
	}
	else {
		(*resArgs)->type->kind = BASIC;
		((*resArgs)->type->u).basic = expType;
	}
	(*resArgs)->tail = NULL;
	tmpRoot = tmpRoot->siblingNodes;
	if(tmpRoot) {
		tmpRoot = tmpRoot->siblingNodes;
		traverseArgs(tmpRoot, &((*resArgs)->tail));
	}
}
