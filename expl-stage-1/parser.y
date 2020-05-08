%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "tree.h"

	FILE *fp,*yyin;
	int yyerror(char const *s);
	int yylex(void);

%}
%union	{
		struct tnode *no;
	}
%token NUM PLUS MINUS MUL DIV END
%type <no> expr NUM program END

%left PLUS MINUS
%left MUL DIV

%%

program : expr END		{
					printf("Abstract Syntax Tree generated. Infix, Postfix and Prefix notations below:\n");
					printf("Infix: \t");
					generateInfix($$);
					printf("\n");
					printf("Postfix: \t");
					generatePostfix($$);
					printf("\n");
					printf("Prefix: \t");
					generatePrefix($$);
					printf("\n");
				   	int r = codeGen($$);
					freeRegister();
					scall("Write",-2,4096);
					scall("Exit",0,0);
					return 0;
				}
	;

expr    : expr PLUS expr 	{ $$ = makeOperatorNode('+', $1, $3); }
        | expr MINUS expr 	{ $$ = makeOperatorNode('-',$1,$3); }
        | expr MUL expr 	{ $$ = makeOperatorNode('*',$1,$3); }
        | expr DIV expr 	{ $$ = makeOperatorNode('/',$1,$3); }
	| '(' expr ')' 		{ $$ = $2; }
	| NUM		        { $$ = $1; }
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s",s);
}

int main() 
{
	yyin = fopen("input.txt","r");
	fp = fopen("target.xsm","w");

	// standard code to insert the header for the XSM executable
	fprintf(fp,"%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0);
	fprintf(fp,"MOV SP,4096\n");
	fclose(fp);

	yyparse();

	return 0;
}
