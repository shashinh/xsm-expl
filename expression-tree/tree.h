typedef struct node{
  //contains the numerical value of the node
  int val;
  //if operator node, contains the operator
  char *op;
  //pointers to left and right nodes
  struct node *left, *right;
} node;

//create a leaf node and assign to it, its numerical value
struct node* makeLeafNode(int n);

//create an operator node, assign its operator and link its left and right children
struct node* makeOperatorNode(char c, struct node* left, struct node* right);

//accept a pointer to the root of the tree and evaluate it
int evaluate(struct node* tree);
