#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
int yyerror(char const *s);
int regIndex =-1;

struct tnode* createNode(int val,
			int type,
			char*varName,
			int nodeType,
			struct tnode *left,
			struct tnode *right)
{
	struct tnode *temp;
	temp=(struct tnode*)malloc(sizeof(struct tnode));
	temp->val=val;
	temp->type=type;
	if(varName!=NULL)
	{
		temp->varName=(char*)malloc(sizeof(char));
		temp->varName=varName;
	}
	else
	{
		temp->varName=NULL;
	}
	temp->nodeType=nodeType;
	temp->left=left;
	temp->right=right;

	return temp;
}

void printNode(struct tnode *t)
{
	printf("[val:%d type:%d ",t->val,t->type);
	if(t->varName!=NULL)
	printf("varName:%c ",*t->varName);
	switch(t->nodeType)
	{
		case 1:
		    	printf("op_node ");
		    	break;
		case 2:
		    	printf("assg_node ");
	   		break;
		case 3:
		    	printf("conn_node ");
		    	break;
		case 4:
		    	printf("const_node ");
	   		break;
		case 5:
		  	printf("read_node ");
		  	break;
		case 6:
			printf("var_node ");
			break;
		case 7:
		    	printf("write_node ");
		    	break;
	}

	printf("]\n");
}

void traverseInorder(struct tnode *t)
{
	if(t==NULL)
		return;

	traverseInorder(t->left);
	printNode(t);
	traverseInorder(t->right);

}
void traversePreorder(struct tnode *t)
{
	if(t==NULL)
		return;

	printNode(t);

	traversePreorder(t->left);
	traversePreorder(t->right);

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

int codeGen(struct tnode *t)
{
	FILE *fp;
	fp=fopen("target.xsm","a");
	int l, r, i, j, buff, data, val;
	switch(t->nodeType)
	{

		case conn_node:
				l = codeGen(t->left);
				//l = codeGen(t->right);
				r = codeGen(t->right);
				fclose(fp);
				return l;
				break;

		case read_node:
				buff = 4096 + *(t->left->varName) - 'a';
				i = scall("Read", -1, buff);
				fclose(fp);
				return i;
				break;

		case assg_node:
				r = codeGen(t->right);
				val = 4096 + *(t->left->varName) - 'a';
				fprintf(fp, "MOV [%d],R%d\n", val, r);
				fclose(fp);
				freeRegister();
				printf("%d\n", regIndex);
				return r;
				break;

		case var_node:
				val = 4096 + *(t->varName) - 'a';
				i = getRegister();
				fprintf(fp, "MOV R%d,[%d]\n", i, val);
				fclose(fp);
				return i;
				break;

		case const_node:
				i = getRegister();
				fprintf(fp, "MOV R%d,%d\n", i, t->val);
				fclose(fp);
				return i;
				break;

		case write_node:
				r = codeGen(t->left);
				freeRegister();
				i = scall("Write", -2, 4121);
				fclose(fp);
				return i;
				break;

		case op_node:
				l = codeGen(t->left);
				r = codeGen(t->right);
				switch(*(t->varName))
				{
					case '+':
						fprintf(fp, "ADD R%d,R%d\n", l, r);
						fclose(fp);
						freeRegister();
						break;
					case '-':
						fprintf(fp, "SUB R%d,R%d\n", l, r);
						fclose(fp);
						freeRegister();
						break;
					case '*':
						fprintf(fp, "MUL R%d,R%d\n", l, r);
						fclose(fp);
						freeRegister();
						break;
					case '/':
						fprintf(fp, "DIV R%d,R%d\n", l, r);
						fclose(fp);
						freeRegister();
						break;
				}

				return l;
				break;
	}

}

// boiler-plate code to insert Write/Read operations into the executable
int scall(char *fcode, int arg1, int arg2)
{
	FILE *fp=fopen("target.xsm","a");
 	int r,i,j;

  if(strcmp("Write",fcode)==0)
   {
	i = getRegister();
	fprintf(fp,"MOV [%d],R%d\n",arg2,i);
     	fprintf(fp,"MOV R%d,\"%s\"\n",i,fcode);
     	fprintf(fp,"PUSH R%d\n",i);

     	fprintf(fp,"MOV R%d,%d\n",i,arg1);
     	fprintf(fp,"PUSH R%d\n",i);

     	fprintf(fp,"MOV R%d,[%d]\n",i,arg2);
     	fprintf(fp,"PUSH R%d\n",i);

	j=getRegister();
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"CALL 0\n");
	freeRegister();

	fprintf(fp,"POP R%d\n",i);

	j=getRegister();
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	freeRegister();

	freeRegister();
	fclose(fp);
	return i;
    }
   else if(strcmp("Read",fcode)==0)
   {
     	i=getRegister();
     	fprintf(fp,"MOV R%d,\"%s\"\n",i,fcode);
     	fprintf(fp,"PUSH R%d\n",i);

     	fprintf(fp,"MOV R%d,%d\n",i,arg1);
     	fprintf(fp,"PUSH R%d\n",i);

     	fprintf(fp,"MOV R%d,%d\n",i,arg2);
     	fprintf(fp,"PUSH R%d\n",i);

	j=getRegister();
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"CALL 0\n");
	freeRegister();

     	fprintf(fp,"POP R%d\n",i);

	j=getRegister();
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	freeRegister();

     	freeRegister();
     	fclose(fp);
	return i;
    }
   else if(strcmp("Exit",fcode)==0)
   {
     	i=getRegister();
     	fprintf(fp,"MOV R%d,\"%s\"\n",i,fcode);
	fprintf(fp,"PUSH R%d\n",i);
     	j=getRegister();
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"PUSH R%d\n",j);
     	fprintf(fp,"CALL 0\n");
     	freeRegister();

     	fprintf(fp,"POP R%d\n",i);
     	j=getRegister();
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	fprintf(fp,"POP R%d\n",j);
     	freeRegister();

     	freeRegister();
     	fclose(fp);
     	return i;
    }


}
