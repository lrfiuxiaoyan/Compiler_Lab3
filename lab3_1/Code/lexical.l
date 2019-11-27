%{
	#include "node.h"
	#include "syntax.tab.h"
	#include <math.h>
	#include <stdlib.h>
	extern int counterError;
	int yycolumn = 1;
	#define YY_USER_ACTION\
	yylloc.first_line = yylloc.last_line = yylineno;\
	yylloc.first_column = yycolumn;\
	yylloc.last_column = yycolumn + yyleng - 1;\
	yycolumn += yyleng;
	
	
%}

%option yylineno



SPACE [ \n\t\r]+
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
EXPFLOAT ({INT}|{FLOAT})[Ee][+-]?{DIGIT}+
EXPFLOATERROR ({INT}|{FLOAT})[Ee][+-]?
DIGIT [0-9]
DIGITP [1-9]
INTERROR [0]{DIGIT}+
INT ({DIGITP}{DIGIT}*)|0
FLOAT {INT}(\.(0|{INT}*))?
ALP [a-zA-Z]
ID ({ALP}|_)({ALP}|{DIGIT}|_)*
IDERROR {DIGIT}({ALP}|{DIGIT}|_)*
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS \-
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}

%%
\n {yycolumn = 1;}
{SPACE} {}
{STRUCT} {/*printf("STRUCT\n");*/ return STRUCT;}
{RETURN} {/*printf("/*return\n");*/return RETURN;}
{IF} {/*printf("IF\n");*/ return IF;}
{ELSE} {/*printf("ELSE\n");*/ return ELSE;}
{WHILE} {/*printf("WHILE\n");*/ return WHILE;}
{EXPFLOAT} {/*printf("EXPFLOAT\n");*/ yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext);return FLOAT;}
{TYPE} {/*printf("TYPE\n");*/ /*insertContent(yytext);*/ yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext); return TYPE;}
{PLUS} {/*printf("PLUS\n");*/ return PLUS;}
{MINUS} {/*printf("MINUS\n");*/  return MINUS;}
{INTERROR} {counterError++;printf("Error type A at Line %d: Illegal int number \'%s\'\n",yylineno,yytext);}
{INT} {/*printf("INT\n"); insertContent(yytext);*/ yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext); return INT;}
{FLOAT} {/*printf("FLOAT\n"); insertContent(yytext);*/yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext);return FLOAT;}
{IDERROR} {counterError++;printf("Error type A at Line %d: Illegal ID \'%s\'\n",yylineno,yytext);}
{ID} {/*printf("ID\n"); insertContent(yytext);*/ yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext);return ID;}
{SEMI} {/*printf("SEMI\n");*/ return SEMI;}
{COMMA} {/*printf("COMMA\n");*/ return COMMA;}
{ASSIGNOP} {/*printf("ASSIGNOP\n");*/  return ASSIGNOP;}
{RELOP} {/*printf("RELOP\n");*/ yylval.tokenNode.content = malloc(strlen(yytext)+1); strcpy(yylval.tokenNode.content, yytext); return RELOP;}
{STAR} {/*printf("STAR\n");*/ return STAR;}
{DIV} {/*printf("DIV\n");*/  return DIV;}
{AND} {/*printf("AND\n");*/  return AND;}
{OR} {/*printf("OR\n");*/ return OR;}
{DOT} {/*printf("DOT\n");*/  return DOT;}
{NOT} {/*printf("NOT\n");*/ return NOT;}
{LP} {/*printf("LP\n");*/  return LP;}
{RP} {/*printf("RP\n");*/ return RP;}
{LB} {/*printf("LB\n");*/  return LB;}
{RB} {/*printf("RB\n");*/ return RB;}
{LC} {/*printf("LC\n");*/  return LC;}
{RC} {/*printf("RC\n");*/ return RC;}
{EXPFLOATERROR} {counterError++;printf("Error type A at Line %d: Illegal Exponential Float number\'%s\'\n",yylineno,yytext);}
. {counterError++; printf("Error type A at Line %d: Mysterious characters \'%s\'\n",yylineno,yytext);}


%%
