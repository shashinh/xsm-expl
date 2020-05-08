%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "tree.h"

	FILE *fp,*yyin;
	int yyerror(char const*);
	int yylex(void);
	int yywrap();

	int r;
	int i;
%}

%union
{
	struct tnode *node;
}

%type <node> program NUM ID expr AssgStmt OutputStmt InputStmt Stmt Slist
%token PBEGIN PEND NUM ID PLUS MINUS MUL DIV READ WRITE
%left PLUS MINUS
%left MUL DIV

%%

program : PBEGIN Slist PEND ';'		{
						$$ = $2;
						printf("Abstract Syntax Tree generated, Preorder representation below:\n");
						traversePreorder($$);
						r = codeGen($$);
						i = scall("Exit", 0, 0);
						return 0;
					}
	| PBEGIN PEND ';'		{ $$ = NULL; }
	;

Slist	: Slist Stmt			{ $$ = createNode(0, 0, NULL, conn_node, $1, $2); }
	| Stmt				{ $$ = $1; }
	;

Stmt	: InputStmt			{ $$ = $1; }
	| OutputStmt			{ $$ = $1; }
	| AssgStmt			{ $$ = $1; }
	;

InputStmt : READ'('ID')'';'		{ $$ = createNode(0, 0, NULL, read_node, $3, NULL); }
	  ;

OutputStmt : WRITE'('expr')'';'		{ $$ = createNode(0, 0, NULL, write_node, $3, NULL); }
	   ;

AssgStmt	: ID'='expr';'		{ $$ = createNode(0, 0, "=", assg_node, $1, $3); }
		;

expr	: expr PLUS expr		{ $$ = createNode(0, 0, "+", op_node, $1, $3);	}
	| expr MINUS expr		{ $$ = createNode(0, 0, "-", op_node, $1, $3);	}
	| expr MUL expr			{ $$ = createNode(0, 0, "*", op_node, $1, $3);	}
	| expr DIV expr			{ $$ = createNode(0, 0, "/", op_node, $1, $3); }
	| '('expr')'			{ $$ = $2; }
	| NUM				{ $$ = $1; }
	| ID				{ $$ = $1; }
	;

%%

int yyerror(char const *s)
{
	printf("yyerror %s\n",s);
}

int main()
{
	yyin = fopen("input.txt","r");
	fp = fopen("target.xsm","w");

	// standard code to insert the header for the XSM executable
	fprintf(fp, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",0,2056,0,0,0,0,0,0);
	fprintf(fp, "BRKP\n");
	/* the stack pointer now starts from 4121 instead of 4096,
		 since first 26 memory locations are reserved for the variables
	*/
	fprintf(fp, "MOV SP,4121\n");
	fclose(fp);

	yyparse();

	return 0;
}
