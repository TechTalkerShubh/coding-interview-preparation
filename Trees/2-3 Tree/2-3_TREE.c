/**
  * Implementation and Basic Operations (Insert, Delete, Search, Display) on 2-3 tree.
  * 2-3 Tree is simply a B-Tree of Order 'p' = 3
  * If p is the order, then every node will have p-1 keys and p children.
  * 
  **/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

/* 2-3 tree node */
struct node {
	int keys[3];			/* array stores at-most 3 keys in sorted fashion */
	int keys_count;			/* holds the no. of keys this node currently holds */
	struct node *p1;		/* first child from the left */
	struct node *p2;		/* second child from the left */
	struct node *p3;		/* third child from the left */
	struct node *p4;		/* right most child */
	struct node *parent;	/* parent node */
};

typedef struct node node;

node *root = NULL;

/** creates a new node */
node *create_new_node(int key) {
	node *mem = (node*)malloc(sizeof(node));
	mem->keys[0] = key;
	mem->keys[1] = mem->keys[2] = INT_MIN;
	mem->p1 = mem->p2 = mem->p3 = mem->p4 = mem->parent = NULL;
	mem->keys_count = 1;
	return mem;
}

void insert(int key) {

}

void delete(int key) {
	
	
}

node *tree_minimum(node *root) {
	if(root->p1)
		return tree_minimum(root->p1);
	return root;
}

node *tree_maximum(node *root) {
	if(root->p3)
		return tree_maximum(root->p3);
	return root;
}

int search(node *root, int key) {
	if(!root)
		return 0;
	if(key < root->keys[0])
		return search(root->p1, key);
	if(root->keys_count == 1 || (key > root->keys[0] && key < root->keys[1]))
		return search(root->p2, key);
	if(key > root->keys[1])
		return search(root->p3, key);
	return 1;
}

void inorder(node *root) {
	if(root) {
		inorder(root->p1);
		printf("%d ", root->keys[0]);
		inorder(root->p2);
		if(root->keys_count == 2)
			printf("%d ", root->keys[1]);
		inorder(root->p3);
	}
}

void preorder(node *root) {
	if(root) {
		printf("%d ", root->keys[0]);
		preorder(root->p1);
		if(root->keys_count == 2)
			printf("%d ", root->keys[1]);
		preorder(root->p2);
		preorder(root->p3);
	}
}

void postorder(node *root) {
	if(root) {
		postorder(root->p1);
		postorder(root->p2);
		postorder(root->p3);
		if(root->keys_count == 2)
			printf("%d ", root->keys[1]);
		printf("%d ", root->keys[0]);
	}
}

int main() {

	while(1) {

		printf("+-------------------------+\n"
				"[1] Insert into 2-3 tree.\n"
				"[2] Delete from 2-3 tree.\n"
				"[3] Search for a key.\n"
				"[4] Display Inorder\n"
				"[5] Display Preorder\n"
				"[6] Display Postorder\n"
				"[7] Exit\n"
				"Enter your choice: ");

		char ch;
		scanf(" %c", &ch);

		switch(ch) {
			case '1':
				printf("Enter the key : ");
				int key;
				scanf("%d", &key);
				if(search(root, key)) {	/* first check if the key already exists */
					printf("%d already exists in 2-3 tree.\n", key);
					break;
				}
				insert(key);
				if(root)
					printf("%d successfully inserted.\n", key);
				else {
					printf("Error occurred.\n");
					exit(1);
				}
				break;

			case '2':
				if(!root) {
					printf("2-3 is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(!search(root, key)) {
					printf("%d is NOT present in the 2-3.\n", key);
					break;
				}
				delete(key);
				if(root)
					printf("Successfully deleted.\n");
				else if(!root)
					printf("2-3 Tree is now empty!\n");
				else {
					printf("Error occurred.\n");
					exit(1);
				}
				break;

			case '3':
				printf("Enter the key : ");
				scanf("%d", &key);
				if(search(root, key))
					printf("%c is present in the 2-3 tree.\n", key);
				else
					printf("%c NOT found in the 2-3 tree.\n", key);
				break;

			case '4':
				if(!root)
					printf("2-3 tree is empty\n");
				else {
					inorder(root);
					printf("\n");
				}
				break;

			case '5':
				if(!root)
					printf("2-3 tree is empty\n");
				else {
					preorder(root);
					printf("\n");
				}
				break;

			case '6':
				if(!root)
					printf("2-3 tree is empty\n");
				else {
					postorder(root);
					printf("\n");
				}
				break;

			case '7':
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
