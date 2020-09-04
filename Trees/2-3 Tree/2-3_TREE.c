/**
  * ADSA Lab Assignment 3.1
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

/** fixes the overflow for the node */
void insert_fixup(node *y, int key) {

	while(y && y->keys_count > 2) {	/* until we have a parent which does not overflow */

		printf("Overflow at [%d, %d, %d] occurred.\n", y->keys[0], y->keys[1], y->keys[2]);

		/* get median */
		int median = y->keys[1];

		/* split this node */
		node *w = create_new_node(y->keys[2]);
		w->p2 = y->p4;
		w->p1 = y->p3;

		/* fix child links for the split node */
		if(w->p2)
			w->p2->parent = w;
		if(w->p1)
			w->p1->parent = w;

		/* reset */
		y->keys[1] = y->keys[2] = INT_MIN;
		y->p3 = y->p4 = NULL;
		y->keys_count = 1;

		/* get parent for this node */
		node *p = y->parent;

		/* pull the median up to the parent */
		if(!p) {

			/* create new root node */
			root = p = create_new_node(median);

			/* set parent for both the splits */
			y->parent = w->parent = p;

			/* fix child links for the parent */
			p->p2 = w;
			p->p1 = y;

			break;

		} else {

			if(y == p->p1) {		/* this is the left child */

				/* sort keys of the parent */
				p->keys[2] = p->keys[1];
				p->keys[1] = p->keys[0];
				p->keys[0] = median;

				/* increment key count */
				p->keys_count++;

				/* set parent for the new split */
				w->parent = p;

				/* fix child links for the parent */
				p->p4 = p->p3;
				p->p3 = p->p2;
				p->p2 = w;

				/* move to parent */
				y = p;
			}
			else if(y == p->p2) {	/* this is the middle child */

				/* sort keys of the parent */
				p->keys[2] = p->keys[1];
				p->keys[1] = median;

				/* increment key count */
				p->keys_count++;

				/* set parent for the new split */
				w->parent = p;

				/* fix child links for the parent */
				p->p4 = p->p3;
				p->p3 = w;

				/* move to parent */
				y = p;
			}
			else {					/* this is the right child */

				p->keys[2] = median;

				/* increment key count */
				p->keys_count++;

				/* set parent for the new split */
				w->parent = p;

				/* fix child link for the new split */
				p->p4 = w;

				/* move to parent */
				y = p;
			}
		}
	}
}

/** Inserts the given key into the 2-3 tree */
void insert(int key) {

	node *y = NULL;
	node *x = root;

	/* search where to put the key */
	while(x) {
		y = x;				/* hold the parent node */
		if(key < x->keys[0])
			x = x->p1;
		else if(x->keys_count == 1)
			x = x->p2;
		else
			x = x->p3;
	}

	if(!y)
		root = create_new_node(key);

	else if(y->keys_count == 1) {
		if(key < y->keys[0]) {
			y->keys[1] = y->keys[0];
			y->keys[0] = key;
		} else {
			y->keys[1] = key;
		}
		y->keys_count++;
	}
	else {
		/** overflow condition */

		if(key < y->keys[0]) {
			/* sort the keys */
			y->keys[2] = y->keys[1];
			y->keys[1] = y->keys[0];	/* median */
			y->keys[0] = key;

		} else if(key > y->keys[0] && key < y->keys[1]) {
			/* sort the keys */
			y->keys[2] = y->keys[1];
			y->keys[1] = key;			/* median */

		} else
			y->keys[2] = key;

		y->keys_count++;

		insert_fixup(y, key);
	}
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
