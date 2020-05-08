#include <iostream>
#include <fstream>
#include "tree.h"
#include "codegen.h"

#define regIndex int;

int getReg();
void freeReg();
int buildTarget(struct node* t, std::ofstream *of);

int regCount = 0;
int regMax = 0;

using namespace std;

void initializeExecutable(std::ofstream *of){

  //Initialize the headers of the executable
  *of << 0 << "\n" << 2056 << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n" << 0 << "\n";
  *of << "BRKP\n";
}
int codeGen(struct node* t, std::ofstream *of){

  //ofstream's copy constructor is marked for deletion, so use a pointer to the of object instead
  initializeExecutable(of);

  int result = buildTarget(t, of);

  std::cout << "\ntarget.xsm successfully written\n" << regMax << " registers used\nresult in register R" << result << endl;

  *of << "BRKP\n";

  (*of).close();
  return 1;
}

int buildTarget(struct node* t, std::ofstream *of){

  if(t->op == NULL){

    int reg = getReg();
    if((reg+1) > regMax){
        regMax = reg+1;
    }
    //std::cout << "reached leaf node, assigning R" << reg << " to " << t->val << "\n";
    *of << "MOV R" << reg << "," << t->val << "\n";

    return reg;

  } else {

    int i, j;
    switch(*(t->op)){
        case '+' :
                  //std::cout << "reached case +\n";
                  i = buildTarget(t->left, of);
                  j = buildTarget(t->right, of);
                  *of << "ADD " << "R" << i << "," << "R" << j <<"\n";
                  freeReg();
                  return i;
                  break;
        case '-' :
                  //std::cout << "reached case -\n";
                  i = buildTarget(t->left, of);
                  j = buildTarget(t->right, of);
                  *of << "SUB " << "R" << i << "," << "R" << j <<"\n";
                  freeReg();
                  return i;
                  break;
        case '*' :
                  //std::cout << "reached case *\n";
                  i = buildTarget(t->left, of);
                  j = buildTarget(t->right, of);
                  *of << "MUL " << "R" << i << "," << "R" << j <<"\n";
                  freeReg();
                  return i;
                  break;
        case '/' :
                  //std::cout << "reached case /\n";
                  i = buildTarget(t->left, of);
                  j = buildTarget(t->right, of);
                  *of << "DIV " << "R" << i << "," << "R" << j <<"\n";
                  freeReg();
                  return i;
                  break;
    }


  }

}


int getReg(){
  //handle an error if register count exceeds 19 (i.e. 20 total registers)
  return regCount++;
}

void freeReg(){
  --regCount;
}
