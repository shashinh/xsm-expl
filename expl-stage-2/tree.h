
// numerical constants to define each AST node type
#define op_node    1
#define assg_node  2
#define conn_node  3
#define const_node 4
#define read_node  5
#define var_node   6
#define write_node 7

typedef struct tnode
{
	//value of the node, if numerical.
	int val;
	//type of node
	int type;
	// name of variable, in case of user defined node
	char *varName;
	// indicates the type of the AST node (operator, variable, assignment, etc)
	int nodeType;
	//pointers to children nodes
	struct tnode *left,*middle,*right;
} tnode;

// Creates an AST node, given the type and other information
struct tnode* createNode (int val,
				int type,
				char *varName,
				int nodeType,
				struct tnode *l,
				struct tnode *r);


// Given a node, traverse it and all of its children in Inorder
void traverseInorder(struct tnode *t);
// Given a node, traverse it and all of its children in Preorder

void traversePreorder(struct tnode *t);
void printNode(struct tnode *t);

int codeGen(struct tnode *t);
int scall(char *fcode,int arg1, int arg2);
