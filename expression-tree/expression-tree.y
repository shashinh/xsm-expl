%{
    #include <iostream>
    #include <fstream>
    using namespace std;
    #include "tree.h"
    #include "codegen.h"
    extern int yylex();
    extern int yyerror(char const* s);
    void generateTarget(struct node* t);
%}

%union{
        struct node* node;
}

%type <node> expr NUMBER program END
%token NUMBER PLUS MINUS MUL DIV END
%left PLUS MINUS
%left MUL DIV

%%

program : expr END  {
                        generateTarget ($1);
                        exit(1);
                    }
        ;

expr    : expr PLUS expr    {$$ = makeOperatorNode('+',$1,$3);}
        | expr MINUS expr   {$$ = makeOperatorNode('-',$1,$3);}
        | expr MUL expr     {$$ = makeOperatorNode('*',$1,$3);}
        | expr DIV expr     {$$ = makeOperatorNode('/',$1,$3);}
        | '(' expr ')'      {$$ = $2;}
        | NUMBER            {$$ = $1;}
        ;

%%

int yyerror(char const* s){
    std::cout << "yyerror : " << s << endl;
    return 1;
}

void generateTarget(struct node* t){
    std::ofstream of("target.xsm", std::ofstream::out);
    codeGen(t, &of);
}

int main(){
    yyparse();
    return 0;
}
