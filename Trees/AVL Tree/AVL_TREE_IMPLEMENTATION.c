/**
  *	AVL Tree Implementation in C language and Basic Operations on AVL tree (Insert, Delete, Search, Display)
  */

#include<stdio.h>
#include<stdlib.h>

struct node {
	int data;
	int height;			/* to add AVL functionality */
	struct node *left;
	struct node *right;
};

typedef struct node node;

node *root = NULL;

node *create_new_node(int);			/* create a new node with the given key */
node *left_rotate(node*);			/* performs left rotation and returns new root */
node *right_rotate(node*);			/* performs right rotation and returns new root */
node *insert(node*, int);			/* inserts the given key into the AVL tree and returns the new root */
node *delete(node*, int);			/* deletes the given key from the AVL tree and returns the new root */
node *inorder_successor(node*);		/* returns the smallest node in the RST */
node *inorder_predecessor(node*);	/* returns the greatest node in the LST */
int height(node*);					/* returns the height of AVL starting from the given node */
int balance_factor(node*);			/* returns the balance_factor of the given node */
int max(int, int);					/* returns maximum of the two given integers */
int search(node*, int);				/* searches the given key in the AVL tree and returns the status */
void update_height(node*, node*);	/* updates heights of relocated nodes after rotation procedure. */
void display(node*);				/* utitlity method to print the AVL tree */
void inorder(node*);				/* inorder traversal of the AVL tree */
void preorder(node*);				/* preorder traversal of the AVL tree */
void postorder(node*);				/* postorder traversal of the AVL tree */

int main() {

	while(1) {

		printf("+-------------------------+\n"
				"[1] Insert into AVL tree.\n"
				"[2] Delete from AVL tree.\n"
				"[3] Search in AVL tree.\n"
				"[4] Display\n"
				"[5] Exit\n"
				"Enter your choice: ");

		char ch;
		scanf(" %c", &ch);

		switch(ch) {
			case '1':
				printf("Enter the key : ");
				int key;
				scanf("%d", &key);
				if(search(root, key)) {	/* first check if the key already exists */
					printf("%d already exists in AVL tree.\n", key);
					break;
				}
				root = insert(root, key);
				if(root)
					printf("%d successfully inserted.\n", key);
				else {
					printf("Error occured.\n");
					exit(1);
				}
				break;

			case '2':
				if(!root) {
					printf("AVL is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(!search(root,key)) {
					printf("%d is NOT present in the AVL.\n", key);
					break;
				}
				root = delete(root, key);
				if(root)
					printf("%d successfully deleted.\n", key);
				else
					printf("AVL tree is now empty!\n");
				break;

			case '3':
				if(!root) {
					printf("AVL is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				int found = search(root, key);
				if(found)
					printf("%d is present int the AVL tree.\n", key);
				else
					printf("%d NOT found in the AVL tree.\n", key);
				break;

			case '4':
				if(!root)
					printf("AVL tree is empty\n");
				else
					display(root);
				break;

			case '5':
				printf("Enter 'y' to exit : " );
				char ch;
				scanf(" %c", &ch);
				if(ch == 'y') {
					free(root);
					exit(1);
				}
				break;

			default:
				printf("Invalid choice! Try again...\n");
		}
	}

	return 0;
}

node *insert(node *par, int key) {

	/** +--------------------------+
		regular binary search tree
		+--------------------------+ **/

	if(!par)
		return create_new_node(key);

	if(key < par->data)
		par->left = insert(par->left, key);
	else
		par->right = insert(par->right, key);

	/** +------------------------+
	     adding AVL functionality
	    +------------------------+ **/

	/* Update the height of this parent node */
	par->height = 1 + max(height(par->left), height(par->right));

	/* Get the balance factor of the Parent node */
	int balance = balance_factor(par);

	/** if balance is NOT within range -1 <= balance <= 1 then 4 cases arrive */

	/* Left sub tree */
	if(balance > 1) {

		/* Left child */
		if(key < par->left->data) {	/* LL Imbalance */
			printf("Left-Left Imbalance resolved at node: %d.\n", par->data);
			return right_rotate(par);
		}

		/* Right child */
		if(key > par->left->data) {	/* LR Imbalance */
			printf("Left-Right Imbalance resolved at node: %d.\n", par->data);
			par->left = left_rotate(par->left);
			return right_rotate(par);
		}
	}

	/* Right sub tree */
	if(balance < -1) {

		/* Right child */
		if(key > par->right->data) {	/* RR Imbalance */
			printf("Right-Right Imbalance resolved at node: %d.\n", par->data);
			return left_rotate(par);
		}

		/* Left child */
		if(key < par->right->data) {	/* RL Imbalance */
			printf("Right-Left Imbalance resolved at node: %d.\n", par->data);
			par->right = right_rotate(par->right);
			return left_rotate(par);
		}
	}

	return par;
}

node *left_rotate(node *gpar) {

	node *par = gpar->right;
	node *child = par->left;

	par->left = gpar;		/* grand-parent becomes left child of parent */
	gpar->right = child;	/* left child of parent becomes right child of grandparent */

	/* update the heights for relocated nodes */
	update_height(gpar, par);

	/* if grand-parent was the root node,
	   parent becomes new root node */
	return par;
}

node *right_rotate(node *gpar) {

	node *par = gpar->left;
	node *child = par->right;

	par->right = gpar;	/* grand-parent becomes right child of parent */
	gpar->left = child;	/* right child of parent becomes left child of grand-parent */

	/* update the heights for relocated nodes */
	update_height(gpar, par);

	/* if grand-parent was the root node,
	   parent becomes new root node */
	return par;
}

int balance_factor(node *root) {

	if(!root)
		return 0;

	int lst = 0, rst = 0;

	if(root->left)
		lst = root->left->height;	/* height of left sub tree */
	if(root->right)
		rst = root->right->height;	/* height of right sub tree */

	return lst - rst;
}

int height(node *root) {
	if(!root)	/* empty tree */
		return 0;
	return root->height;
}

void update_height(node *gpar, node *par) {
	gpar->height = 1 + max(height(gpar->left), height(gpar->right));
	par->height = 1 + max(height(par->left), height(par->right));
}

int max(int lst, int rst) {
	return lst > rst ? lst : rst;
}

node *create_new_node(int key) {

	node *mem = NULL;
	mem = (node*)malloc(sizeof(node));

	if(!mem)	/* failed to allocate memory */
		return NULL;

	mem->data = key;
	mem->height = 1;
	mem->left = mem->right = NULL;

	return mem;	/* successfully inserted */
}

/* function expects that the root node exists */
node *delete(node *ptr, int key) {

	/** +-------------------------+
		regular binary search tree
		+-------------------------+ **/

	/* Traverse LST if key's value is less than ptr's value */
	if(key < ptr->data)
		ptr->left = delete(ptr->left, key);

	/* Traverse RST if key's value is less than ptr's value */
	else if(key > ptr->data)
		ptr->right = delete(ptr->right, key);

	/* key's value is same as the ptr's value */
	else {
		if(ptr->right) { /* right child exists */

            /* find inorder successor */
            node* in_succ = inorder_successor(ptr->right);

            /* copy the key to the ptr */
            ptr->data = in_succ->data;

            /* find and delete the inorder successor */
            ptr->right = delete(ptr->right, in_succ->data);

		} else if(ptr->left) { /* left child exists */

			/* find inorder predecessor */
			node *in_pred = inorder_predecessor(ptr->left);

			/* copy the key to the ptr */
			ptr->data = in_pred->data;

			/* find and delete the inorder predecessor */
			ptr->left = delete(ptr->left, in_pred->data);

		} else { /* either this is the only node or a leaf node */

			if(ptr == root) {	/* only node */
				free(ptr);
				root = ptr = NULL;
			} else {			/* leaf node */
				free(ptr);
				ptr = NULL;
			}
		}
	}

	/** +-------------------+
		add AVL functionality
		+-------------------+ **/
		
	if(!ptr)	/* if this was a leaf */
		return NULL;

	/* update height of this node */
	ptr->height = 1 + max(height(ptr->left), height(ptr->right));

	/* get the balance factor for this node	*/
	int balance = balance_factor(ptr);

	/** if balance is NOT within range -1 <= balance <= 1 then 4 cases arrive */

	/* Left sub tree */
	if(balance > 1) {

		int left_balance = balance_factor(ptr->left);

		/* Left child */
		if(left_balance >= 0) {		/* LL Imbalance */
			printf("Left-Left Imbalance resolved at node: %d.\n", ptr->data);
			return right_rotate(ptr);
		}

		/* Right child */
		if(left_balance < 0) {		/* LR Imbalance */
			printf("Left-Right Imbalance resolved at node: %d.\n", ptr->data);
			ptr->left = left_rotate(ptr->left);
			return right_rotate(ptr);
		}
	}

	/* Right sub tree */
	if(balance < -1) {

		int right_balance = balance_factor(ptr->right);

		/* Right child */
		if(right_balance <= 0) {	/* RR Imbalance */
			printf("Right-Right Imbalance resolved at node: %d.\n", ptr->data);
			return left_rotate(ptr);
		}

		/* Left child */
		if(right_balance > 0) {		/* RL Imbalance */
			printf("Right-Left Imbalance resolved at node: %d.\n", ptr->data);
			ptr->right = right_rotate(ptr->right);
			return left_rotate(ptr);
		}
	}
    return ptr;
}

node *inorder_successor(node *root) {
	if(root->left)
		return inorder_successor(root->left);
	return root;
}

node *inorder_predecessor(node *root) {
	if(root->right)
		return inorder_predecessor(root->right);
	return root;
}

int search(node *root, int key) {
	if(!root)
		return 0;
	if(key > root->data)
		return search(root->right, key);
	if(key < root->data)
		return search(root->left, key);
	return 1;
}

void display(node *root) {
	int flag = 1;
	while(flag) {
		printf("\t+--------------------+\n"
				"\t[1] Inorder Traversal\n"
				"\t[2] Preorder Traversal\n"
				"\t[3] Postorder Traversal\n"
				"\t[4] Back to previous window\n"
				"\tEnter your choice : ");

		char ch;
		scanf(" %c", &ch);

		switch(ch) {
			case '1':
				printf("\tInorder : ");
				inorder(root);
				printf("\n");
				break;

			case '2':
				printf("\tPreorder : ");
				preorder(root);
				printf("\n");
				break;

			case '3':
				printf("\tPostorder : ");
				postorder(root);
				printf("\n");
				break;

			case '4':
				flag = 0;
				break;

			default:
				printf("\tInvalid choice! Try again...\n");
		}
	}
}

void inorder(node *root) {
	if(root) {
		inorder(root->left);
		printf("%d ", root->data);
		inorder(root->right);
	}
}

void preorder(node *root) {
	if(root) {
		printf("%d ", root->data);
		preorder(root->left);
		preorder(root->right);
	}
}

void postorder(node *root) {
	if(root) {
		postorder(root->left);
		postorder(root->right);
		printf("%d ", root->data);
	}
}