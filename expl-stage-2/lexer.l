%{
	#include <stdio.h>
	#include <stdlib.h>

	#include "tree.h"
	#include "y.tab.h"

	int yylex(void);
	int yyerror(char const *s);

	int number;
%}

%%
[0-9]+			{
				number = atoi(yytext);
				yylval.node = createNode(number,0,NULL,const_node,NULL,NULL);
				return NUM;
			}
[a-z]			{
				yylval.node = createNode(0,0,yytext,var_node,NULL,NULL);
				return ID;
			}

"begin"			{ return PBEGIN; }
"end"			{ return PEND; }

"read"			{ return READ; }
"write"			{ return WRITE; }

"+"			{ return PLUS; }
"-"			{ return MINUS; }
"*"			{ return MUL; }
"/"			{ return DIV; }
";"			{ return *yytext; }
"="			{ return *yytext; }

[ \t]			{ /* do nothing, we ignore straight-line whitespace*/ }
[\n]			{ /* do nothing, we ignore straight-line whitespace*/ }

[()]			{return *yytext;}

.			{
				yyerror("unknown character\n");
				exit(1);
			}

%%

int yywrap(void)
{
	return 1;
}
