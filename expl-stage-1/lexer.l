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

[0-9]+ 		{
			number = atoi(yytext);
			yylval.no = makeLeafNode(number);
        		return NUM;
		}
"+" 		{ return PLUS; }
"-" 		{ return MINUS; }
"*" 		{ return MUL; }
"/" 		{ return DIV; }
[ \t] 		{ /* do nothing, we ignore straight-line whitespace*/ }
[()] 		{ return *yytext; }
[\n] 		{ return END; }
(.) 		{
			yyerror("unknown character\n");
			exit(1);
		}

%%

int yywrap(void)
{
   return 1;
}
