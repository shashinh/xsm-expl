%{
    #include <iostream>
    #include "tree.h"
    #include "y.tab.h"
    using namespace std;

    extern int yyerror(char const* s);

    int number;
%}


%%

[0-9]+  {
            number = atoi(yytext);
            yylval.node = makeLeafNode(number);
            return NUMBER;
        }

"+"     {
            return PLUS;
        }

"-"     {
            return MINUS;
        }

"*"     {
            return MUL;
        }

"/"     {
            return DIV;
        }

[\t]    {}

[()]    {
            return *yytext;
        }

[\n]    {
            return END;
        }

.       {
            yyerror("unrecognized character\n");
            exit(1);
        }



%%

int yywrap(){
      return 1;
}
