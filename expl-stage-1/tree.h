typedef struct tnode
{
	//value of the node, if numerical.
	int val;
	//type of operator, if operator node
	char *op;
	//pointers to left and right child nodes
	struct tnode *left, *right;
}tnode;

// create a numerical leaf node
struct tnode* makeLeafNode(int n);

// create an operator node with left and right child nodes
struct tnode* makeOperatorNode(char c, struct tnode *left, struct tnode *right);

// given a node, generate its PostFix notation
void generatePostfix(struct tnode *t);

// given a node, generate its PreFix notation
void generatePrefix(struct tnode *t);

// given a node, generate its InFix notation
void generateInfix(struct tnode *t);

// obtain the ID of a free register
int getRegister();

// release a register
void freeRegister();

// Given a node of the AST, generate target code for the XSM platform
int codeGen(struct tnode*t);

// Helper method to insert Read/Write code into the target executable
void scall(char *fcode,int arg1,int arg2);
