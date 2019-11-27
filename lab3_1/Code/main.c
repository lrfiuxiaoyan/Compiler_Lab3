#include<stdio.h>
#include<stdlib.h>
//#include"node.h"
//#include "semantic.h"
#include "translate.h"
GrammarTree* root;
InterCodes head, tail;
extern FILE* yyin;
int counterError = 0;
extern int yyrestart(FILE*);
extern void yyparse();
extern int yylex();
extern int yylineno;



int main(int argc, char** argv)
{
	if(argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	if(!f){
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);

	//yylineno = 1;
	yyparse();
	if(counterError == 0){
//		printNodes(root, 0);
		initHashTable();
		traverseTree(root);
		head = NULL;
		tail = head;
		test_gen(argv[2]);
//		generateIR()
	}

	return 0;

}

