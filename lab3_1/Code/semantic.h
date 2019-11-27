#include <stdlib.h>

#include "node.h"
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct ArgList_* ArgList;
typedef struct Symbol_* Symbol;
typedef struct HashNode_* HashNode;

struct Type_ {
	enum{BASIC, ARRAY, STRUCTURE} kind;
	union {
		int basic;	//int:0, float:1
		struct {
			Type elem;
			int size;
		} array;
		struct {
			FieldList fieldList;
			char* structName;	
		} structure;
	} u;
};

struct FieldList_ {
	char* name;
	Type type;
	FieldList tail;
};

struct ArgList_ {
	char* name;
	Type type;
	ArgList tail;	
};

struct Symbol_ {
	enum{VAR, FUN, STRUCTDEF} kind;
	char* name;
	Type type;
	union {
		struct {			
			union {
				int intVal;
				float floatVal;
			} initVal;
		} varInfo;
		struct {
			int argNum;
			ArgList args;	
			int state;	//not declared nor defined(0); declared but not defined(1); not declared but defined(2); declared and defined(3)
		} funInfo;
	} symInfo;
};

struct HashNode_ {
	Symbol symbol;
	HashNode next;
};
#define SYBTAB_SIZE 0x3fff
HashNode sybtab[SYBTAB_SIZE];		//the last elem of sybtab is used to store defined struture

HashNode getSybtabNode(char* name);
void initHashTable();
void traverseTree(GrammarTree* root);
void traverseExtDefList(GrammarTree* root);
void traverseExtDef(GrammarTree* root);
void traverseExtDecList(GrammarTree* root, Type varDecType);

void traverseSpecifier(GrammarTree* root, Type res);
void traverseStructSpecifier(GrammarTree* root, FieldList* resFieldList, char** resStructName);

HashNode traverseVarDec(GrammarTree* root, Type varDecType, FieldList* resFieldList, _Bool fillFieldList, ArgList* resArgList, _Bool fillArgList);
HashNode traverseFunDec(GrammarTree* root, Type retType);
void traverseVarList(GrammarTree* root, int* argNum, ArgList* args);
void traverseParamDec(GrammarTree* root, ArgList* args);


void traverseCompSt(GrammarTree* root, HashNode funSybtabNode);
void traverseStmtList(GrammarTree* root, HashNode funSybtabNode);
void traverseStmt(GrammarTree* root, HashNode funSybtabNode);

void traverseDefList(GrammarTree* root, FieldList* resFieldList, _Bool fillFieldList);
void traverseDef(GrammarTree* root, FieldList* resFieldList, _Bool fillFieldList);
void traverseDecList(GrammarTree* root, Type varDefType, FieldList* resFieldList, _Bool fillFieldList);
void traverseDec(GrammarTree* root, Type varDecType, FieldList* resFieldList, _Bool fillFieldList);

int traverseExp(GrammarTree* root, char** resIdName, _Bool* isLeftExp);
void traverseArgs(GrammarTree* root, ArgList* resArgs);

