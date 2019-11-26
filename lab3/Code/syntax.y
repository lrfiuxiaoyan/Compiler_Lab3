%{
	#include<stdlib.h>
	#include<stdio.h>
	#include "lex.yy.c"
	//#include "grammarTree.h"
	extern GrammarTree* root;
	extern int counterError;
	extern int yylex();
	extern void yyerror(char *msg);
	extern int yylineno;
	int line;

%}

%union{
	int type_int;
	float type_float;
	double type_double;
	char* type_string;
	GrammarTree* node;
	struct {
		GrammarTree* node;
		char* content;
	} tokenNode;
}

/*declared tokens*/

%token <tokenNode> TYPE
%token <tokenNode> INT
%token <tokenNode> FLOAT
%token <tokenNode> ID
%token <tokenNode> RELOP

%token <node> STRUCT
%token <node> RETURN
%token <node> IF
%token <node> ELSE
%token <node> WHILE
%token <node> PLUS
%token <node> MINUS
%token <node> SEMI
%token <node> COMMA
%token <node> ASSIGNOP
%token <node> STAR
%token <node> DIV
%token <node> AND
%token <node> OR
%token <node> DOT
%token <node> NOT
%token <node> LP
%token <node> RP
%token <node> LB
%token <node> RB
%token <node> LC
%token <node> RC

/*------associativity of operator------*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left DOT LP RP LB RB

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args
%%

/*--------High-level Definitions-------*/

Program		: ExtDefList		{$$ = createNode("Program", NULL, @$.first_line, 0, NULL);insertNodes($$, $1); root = $$;}
		;
ExtDefList	: ExtDef ExtDefList	{$$ = createNode("ExtDefList", NULL, @$.first_line, 0, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| /*empty*/		{$$ = NULL;}
		;
ExtDef		: Specifier ExtDecList SEMI	{$$ = createNode("ExtDef",  NULL, @$.first_line, 0, NULL);  $3 = createNode("SEMI", NULL, @3.first_line, 1, NULL); insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Specifier SEMI	{$$ = createNode("ExtDef", NULL, @$.first_line, 0, NULL); $2 = createNode("SEMI",  NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| Specifier FunDec CompSt	{$$ = createNode("ExtDef",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
//		| Specifier FunDec SEMI {$$ = createNode("ExtDef", NULL, @$.first_line, 0, NULL); $3 = createNode("SEMI", NULL, @3.first_line, 1, NULL); insertNodes($$, $1); insertNodes($$, $2); insertNodes($$, $3);}
		| Specifier FunDec error SEMI {counterError++;printf("Error type B at Line %d: Incomplete definition of function.\n", yylineno);}
		| Specifier FunDec error {counterError++;printf("Error type B at Line %d: Incomplete definition of function.\n", yylineno);}
		| error SEMI		{counterError++;printf("Error type B at Line %d: Wrong Definition\n", yylineno);}
		;
ExtDecList	: VarDec		{$$ = createNode("ExtDecList", NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		| VarDec COMMA ExtDecList	{$$ = createNode("ExtDecList",  NULL, @$.first_line, 0, NULL); $2 = createNode("COMMA",  NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		;
/*------------Specifiers---------------*/
Specifier	: TYPE			{$$ = createNode("Specifier",  NULL, @$.first_line, 0, NULL); $1.node = createNode("TYPE", NULL, @1.first_line, 2, $1.content); insertNodes($$, $1.node);}
		| StructSpecifier	{$$ = createNode("Specifier",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		;
StructSpecifier	: STRUCT OptTag LC DefList RC{$$ =  createNode("StructSpecifier",  NULL, @$.first_line, 0, NULL);$1 =  createNode("STRUCT",  NULL, @1.first_line, 1, NULL);$3 =  createNode("LC",  NULL, @3.first_line, 1, NULL);$5 =  createNode("RC",  NULL, @5.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);
						insertNodes($$, $4);insertNodes($$, $5);}
		| STRUCT Tag		{$$ = createNode("StructSpecifier",  NULL, @$.first_line, 0, NULL);$1 =  createNode("STRUCT",  NULL, @1.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		;
OptTag		: ID			{$$ = createNode("OptTag",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		| /*empty*/		{$$ = NULL;}
		;
Tag		: ID			{$$ = createNode("Tag",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		;

/*------------Declarators-----------*/
VarDec		: ID			{$$ = createNode("VarDec",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		| VarDec LB INT RB	{$$ = createNode("VarDec", NULL, @$.first_line, 0, NULL);$2 = createNode("LB", NULL, @2.first_line, 1, NULL);$3.node = createNode("INT", NULL, @3.first_line, 2, $3.content);$4 = createNode("RB", NULL, @4.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3.node);insertNodes($$, $4);}
		| VarDec LB error RB	{counterError++;printf("Error type B at Line %d: Missing ']'\n", yylineno);}
		;
FunDec		: ID LP VarList RP	{$$ = createNode("FunDec",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$4 = createNode("RP", NULL, @4.first_line, 1, NULL);insertNodes($$, $1.node);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);}
		| ID LP RP		{$$ = createNode("FunDec",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$3 = createNode("RP", NULL, @3.first_line, 1, NULL);insertNodes($$, $1.node);insertNodes($$, $2);insertNodes($$, $3);}
		| error RP		{counterError++;printf("Error type B at Line %d: Function Definition Error\n", yylineno);}
		;
VarList		: ParamDec COMMA VarList	{$$ = createNode("VarList",  NULL, @$.first_line, 0, NULL);$2 = createNode("COMMA", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| ParamDec		{$$ = createNode("VarList",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		;
ParamDec	: Specifier VarDec	{$$ = createNode("ParamDec",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| error RP		{counterError++;printf("Error type B at Line %d: Missing ')'\n",yylineno);}
		| error COMMA		{counterError++;printf("Error type B at Line %d: Missing ';'\n", yylineno);}
		;
/*---------------Statements------------*/
CompSt		: LC DefList StmtList RC	{$$ = createNode("CompSt",  NULL, @$.first_line, 0, NULL);$1 = createNode("LC", NULL, @1.first_line, 1, NULL);$4 = createNode("RC", NULL, @4.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);}
		| LC error RC		{counterError++;printf("Error type B at Line %d: Statement block error\n", yylineno);}
		;
StmtList	: Stmt StmtList		{$$ = createNode("StmtList",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| /*empty*/		{$$ = NULL;}
		;
Stmt		: Exp SEMI		{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);$2 = createNode("SEMI", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| CompSt		{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		| RETURN Exp SEMI	{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);$1 = createNode("RETURN", NULL, @1.first_line, 1, NULL);$3 = createNode("SEMI", NULL, @3.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| IF LP Exp RP Stmt	{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);$1 = createNode("IF", NULL, @1.first_line, 1, NULL);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$4 = createNode("RP", NULL, @4.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);insertNodes($$, $5);}
		| IF LP Exp RP Stmt ELSE Stmt	{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);$1 = createNode("IF", NULL, @1.first_line, 1, NULL);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$4 = createNode("RP", NULL, @4.first_line, 1, NULL);$6 = createNode("ELSE", NULL, @6.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);insertNodes($$, $5);insertNodes($$, $6);insertNodes($$, $7);}
		| WHILE LP Exp RP Stmt	{$$ = createNode("Stmt",  NULL, @$.first_line, 0, NULL);$1 = createNode("WHILE", NULL, @1.first_line, 1, NULL);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$4 = createNode("RP", NULL, @4.first_line, 1, NULL); insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);insertNodes($$, $5);}
		| IF LP Exp RP error ELSE Stmt	{counterError++;printf("Error type B at Line %d: Missing ';'\n", yylineno);}
		;
/*------------Local Definitions-------*/
DefList		: Def DefList		{$$ = createNode("DefList",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| /*empty*/		{$$ = NULL;}
		;
Def		: Specifier DecList SEMI	{$$ = createNode("Def",  NULL, @$.first_line, 0, NULL);$3 = createNode("SEMI", NULL, @3.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| error SEMI		{counterError++;printf("Error type B at Line %d: Wrong Local Definition\n",yylineno);}
		;
DecList		: Dec			{$$ = createNode("DecList",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		| Dec COMMA DecList	{$$ = createNode("DecList",  NULL, @$.first_line, 0, NULL);$2 = createNode("COMMA", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		;
Dec		: VarDec		{$$ = createNode("Dec",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		| VarDec ASSIGNOP Exp	{$$ = createNode("Dec",  NULL, @$.first_line, 0, NULL);$2 = createNode("ASSIGNOP", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		;
/*------------Expressions------------*/
Exp		: Exp ASSIGNOP Exp	{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("ASSIGNOP", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp AND Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("AND", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp OR Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("OR", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp RELOP Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2.node = createNode("RELOP", NULL, @2.first_line, 2, $2.content);insertNodes($$, $1);insertNodes($$, $2.node);insertNodes($$, $3);}
		| Exp PLUS Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("PLUS", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp MINUS Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("MINUS", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp STAR Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("STAR", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp DIV Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("DIV", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| LP Exp RP		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1 = createNode("LP", NULL, @1.first_line, 1, NULL);$3 = createNode("RP", NULL, @3.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| MINUS Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1 = createNode("MINUS", NULL, @1.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);}	
		| NOT Exp		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1 = createNode("NOT", NULL, @1.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);}
		| ID LP Args RP		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$4 = createNode("RP", NULL, @4.first_line, 1, NULL);insertNodes($$, $1.node);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);}
		| ID LP RP		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);$2 = createNode("LP", NULL, @2.first_line, 1, NULL);$3 = createNode("RP", NULL, @3.first_line, 1, NULL);insertNodes($$, $1.node);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp LB Exp RB		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("LB", NULL, @2.first_line, 1, NULL);$4 = createNode("RB", NULL, @4.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);insertNodes($$, $4);}
		| Exp DOT ID		{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$2 = createNode("DOT", NULL, @2.first_line, 1, NULL);$3.node = createNode("ID", NULL, @3.first_line, 2, $3.content);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3.node);}
		| ID			{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1.node = createNode("ID", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		| INT			{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1.node = createNode("INT", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		| FLOAT			{$$ = createNode("Exp",  NULL, @$.first_line, 0, NULL);$1.node = createNode("FLOAT", NULL, @1.first_line, 2, $1.content);insertNodes($$, $1.node);}
		| Exp LB error RB	{counterError++;printf("Error type B at Line %d: Missing ']'\n", yylineno);}
		;
Args		: Exp COMMA Args	{$$ = createNode("Args",  NULL, @$.first_line, 0, NULL);$2 = createNode("COMMA", NULL, @2.first_line, 1, NULL);insertNodes($$, $1);insertNodes($$, $2);insertNodes($$, $3);}
		| Exp			{$$ = createNode("Args",  NULL, @$.first_line, 0, NULL);insertNodes($$, $1);}
		;

		
%%

void yyerror(char* ch){}
