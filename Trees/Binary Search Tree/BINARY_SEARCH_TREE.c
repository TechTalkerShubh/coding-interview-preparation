/** 
  * Implementation of Binary Search Tree and basic operations on Binary Search Tree
  */

#include<stdio.h>
#include<stdlib.h>

struct node {
	int data;
	struct node *left;
	struct node *right;
};

typedef struct node node;

node *root = NULL;

node *create_new_node(int);			/* create a new node with the given key */
int search(node*, int);				/* searches the given key in the BST and returns status */
node *insert(node*, int);			/* inserts the given key into the BST and returns the new root */
node *delete(node*, int);			/* deletes the given key from the BST and returns the new root */
node *inorder_successor(node*);		/* returns the inorder successor from the given root node */
node *inorder_predecessor(node*);	/* returns the inorder predecessor from the given root node */
void display(node*);				/* utitlity method to print the BST */
void inorder(node*);				/* inorder traversal of the BST */
void preorder(node*);				/* preorder traversal of the BST */
void postorder(node*);				/* postorder traversal of the BST */

int main() {
	
	while(1) {
		
		printf("+---------------------+\n"
				"[1] Insert into BST.\n"
				"[2] Delete from BST.\n"
				"[3] Search in BST.\n"
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
				if(search(root, key)) {
					printf("%d already exists in the BST\n", key);
					break;
				}
				root = insert(root, key);
				if(root)
					printf("%d successfully inserted.\n", key);
				else {
					printf("Error occured.\n", key);
					exit(1);
				}
				break;
			
			case '2':
				if(!root) {
					printf("BST is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(!search(root, key)) {
					printf("%d does not exist in the BST!\n", key);
					break;
				}
				root = delete(root, key);
				if(root)
					printf("%d successfully deleted.\n", key);
				else
					printf("BST tree is now empty!\n");
				break;
				
			case '3':
				if(!root) {
					printf("BST is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(search(root, key))
					printf("%d is present int the BST.\n", key);
				else
					printf("%d NOT found in the BST.\n", key);
				break;
				
			case '4':
				if(!root)
					printf("BST is empty\n");
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

node *insert(node *root, int key) {
	
	if(!root)
		return create_new_node(key);
	
	if(key > root->data)
		root->right = insert(root->right, key);		/* move right */
	else
		root->left = insert(root->left, key);		/* move left */
	
	return root;
}

node *create_new_node(int key) {
	
	node *mem = NULL;
	mem = (node*)malloc(sizeof(node));
	
	if(!mem)	/* failed to allocate memory */
		return NULL;
	
	mem->data = key;
	mem->left = mem->right = NULL;
	
	return mem;	/* successfully inserted */
}

/* function expects that the root node exists */
node *delete(node *ptr, int key) {
	
	if(key > ptr->data)
		ptr->right = delete(ptr->right, key);	/* move right */
	
	else if(key < ptr->data)
		ptr->left = delete(ptr->left, key);		/* move left */
	
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
	return ptr;
}

/* extracts smallest node from the Right sub tree */
node *inorder_successor(node *root) {
	if(root->left)
		return inorder_successor(root->left);
	return root;
}

/* extracts largest node from the Left Sub Tree */
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
