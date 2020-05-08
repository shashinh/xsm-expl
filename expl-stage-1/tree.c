#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
int yyerror(char const *s);
int regIndex =-1;

struct tnode* makeLeafNode(int n)
{
	struct tnode *temp;
	temp = (struct tnode*)malloc(sizeof(struct tnode));
	temp->op = NULL;
	temp->val = n;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

struct tnode* makeOperatorNode(char c, struct tnode *left, struct tnode *right)
{
	struct tnode *temp;
	temp = (struct tnode*)malloc(sizeof(struct tnode));
	temp->op = malloc(sizeof(char));
	*(temp->op) = c;
	temp->left = left;
	temp->right = right;
	return temp;
}

void generatePostfix(struct tnode *t)
{
	if(t->left!=NULL)
		generatePostfix(t->left);
	if(t->right!=NULL)
		generatePostfix(t->right);
        if(t->op==NULL)
		printf("%d ",t->val);
	else
		printf("%c ",*(t->op));
}

void generatePrefix(struct tnode *t)
{
	if(t->op==NULL)
		printf("%d ",t->val);
	else
		printf("%c ",*(t->op));
	if(t->left!=NULL)
		generatePrefix(t->left);
	if(t->right!=NULL)
		generatePrefix(t->right);
}

void generateInfix(struct tnode *t)
{
	if(t->left!=NULL)
		generateInfix(t->left);
        if(t->op==NULL)
		printf("%d",t->val);
	else
		printf("%c",*(t->op));
	if(t->right!=NULL)
		generateInfix(t->right);
}

int getRegister()
{
	  regIndex++;
	  if(regIndex>20)
	    yyerror("Error: No free registers for the operation\n");
	  else
	     return regIndex;
}

void freeRegister()
{
 	regIndex--;
}

int codeGen(struct tnode* t)
{
	FILE *fp =fopen("target.xsm","a");
	int reg, i, j;
	// handle the event that the node is a numerical node (a leaf)
	if(t->op == NULL)
	{
			reg = getRegister();
			// simply move the value of the node to a free register
			fprintf(fp,"MOV R%d,%d\n", reg, t->val);
			fclose(fp);
			return reg;
	}
	// else handle the operator node
	else
	{
	i = codeGen(t->left);
	j = codeGen(t->right);
	// handle each type of operator
	switch(*(t->op))
	{
	case '+':
		fprintf(fp,"ADD R%d,R%d\n",i,j);
	   	break;
	case '-':
	  	fprintf(fp,"SUB R%d,R%d\n",i,j);
		break;
	case '*':
		fprintf(fp,"MUL R%d,R%d\n",i,j);
		break;
	case '/':
		fprintf(fp,"DIV R%d,R%d\n",i,j);
		break;
	}

	/* the result of the operation will be stored in the first register
	 so go ahead and release the higher register */
	freeRegister();
	fclose(fp);
	return i;
	}
}


// boiler-plate code to insert Write/Read operations into the executable
void scall(char *fcode, int arg1, int arg2)
{
  FILE *fp=fopen("target.xsm","a");
  int r,i,j;

  if(strcmp("Write",fcode)==0)
   {
     regIndex = getRegister();
     fprintf(fp,"MOV [%d],R%d\n",arg2,r);
     fprintf(fp,"MOV R%d,\"%s\"\n",r,fcode);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"MOV R%d,%d\n",r,arg1);
     fprintf(fp,"PUSH R%d\n",r);
     i=getRegister();
     fprintf(fp,"MOV R%d,[%d]\n",i,arg2);
     fprintf(fp,"PUSH R%d\n",i);
     freeRegister();
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"CALL 0\n");
     i=getRegister();
     fprintf(fp,"POP R%d\n",i);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     freeRegister();
     freeRegister();
     fclose(fp);
    }
   else if(strcmp("Read",fcode)==0)
   {
     r=getRegister();
     fprintf(fp,"MOV R%d,\"%s\"\n",r,fcode);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"MOV R%d,%d\n",r,arg1);
     fprintf(fp,"PUSH R%d\n",r);
     i=getRegister();
     fprintf(fp,"MOV R%d,%d\n",i,arg2);
     fprintf(fp,"PUSH R%d\n",i);
     freeRegister();
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"CALL 0\n");
     i=getRegister();
     fprintf(fp,"POP R%d\n",i);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     freeRegister();
     freeRegister();
     fclose(fp);
    }
   else if(strcmp("Exit",fcode)==0)
   {
     r=getRegister();
     fprintf(fp,"MOV R%d,\"%s\"\n",r,fcode);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"PUSH R%d\n",r);
     fprintf(fp,"CALL 0\n");
     i=getRegister();
     fprintf(fp,"POP R%d\n",i);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     fprintf(fp,"POP R%d\n",r);
     freeRegister();
     freeRegister();
     fclose(fp);
    }
 }
