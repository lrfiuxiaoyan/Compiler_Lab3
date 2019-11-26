#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include<string.h>

#define MAX_LEN 32
extern int yylineno;
enum treeType{NonTerminator, Terminator, HaveValue};

typedef struct GrammarTree{
//int id;
	char idName[MAX_LEN];
	char content[MAX_LEN];
	enum treeType type;
	int lineNum;
	struct GrammarTree *childNode;
	struct GrammarTree *siblingNodes;
}GrammarTree;

extern GrammarTree *root;
GrammarTree* createNode(char* idname,char* value, int line, enum treeType type, char* content);
void insertNodes(GrammarTree* parent,GrammarTree* child);
void printNodes(GrammarTree* node, int blankNum);


typedef struct Content{
	char content[MAX_LEN];
	struct Content *next;
}Content;

void insertContent(char*content);
