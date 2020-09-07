/**
  * B Tree Implementation and Basic Operations (Insert, Delete, Search, Display) on B tree.
  *
  * If T is the order, then every node must have:
  * 	at least T-1 keys, and
  * 	at least T children (except Root node)
  * In general,
  * T   <= # of child < 2 * T
  * T-1 <= # of keys  < 2 * T - 1
  *
  **/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int T;		/* order of the BTree */
int MAX;	/* # of keys should be strictly less than the max value
 	 	 	 * # of child should be less than or equal to the max value */

/* B tree node */
struct node {
	int keys[100];			/* array stores at-most 2T-1 keys in sorted fashion */
	int keys_count;			/* holds the no. of keys this node currently holds */
	struct node *p[100+1];	/* holds the child nodes */
	struct node *parent;	/* parent node */
};

typedef struct node node;

node *root = NULL;

/** creates a new node */
node *create_new_node(int key) {
	node *mem = (node*)malloc(sizeof(node));
	mem->keys[0] = key;
	for(int i=1; i < MAX; i++)
		mem->keys[i] = INT_MAX;
	for(int i=0; i <= MAX; i++)
		mem->p[i] = NULL;
	mem->parent = NULL;
	mem->keys_count = 1;
	return mem;
}

/** checks if the given node overflows */
int is_overflow(node *y) {
	return y && y->keys_count == MAX;
}

/** fixes the overflow for the node */
void insert_fixup(node *y) {

	while(is_overflow(y)) {	/* until we have a parent which does not overflow */

		printf("Overflow at [%d", y->keys[0]);
		for(int i=1; i < MAX; i++)
			printf(", %d", y->keys[i]);
		printf("] occurred\n");

		/* get median */
		int median = y->keys[MAX/2];

		/* split this node */
		node *w = create_new_node(y->keys[MAX/2 + 1]);
		for(int i = MAX/2 + 2, j = 1; i < MAX; i++, j++) {	/* copy values */
			w->keys[j] = y->keys[i];
			w->keys_count++;
		}
		for(int i=MAX/2 + 1, j=0; i <= MAX; i++, j++)	/* copy children */
			w->p[j] = y->p[i];

		/* fix child links for the split node */
		for(int i=0; i <= w->keys_count; i++)
			if(w->p[i])
				w->p[i]->parent = w;

		/* reset */
		for(int i=MAX/2; i < MAX; i++) {
			y->keys[i] = INT_MAX;
			y->p[i + 1] = NULL;
			y->keys_count--;
		}

		/* get parent for this node */
		node *p = y->parent;

		/* pull the median up to the parent */
		if(!p) {

			/* create new root node */
			root = create_new_node(median);

			/* set parent for both the splits */
			y->parent = w->parent = root;

			/* fix child links for the new root */
			root->p[0] = y;	/* left child */
			root->p[1] = w;	/* right child */

			break;

		} else {

			/* set parent for the new split */
			w->parent = p;

			/* find where the node y exists */
			int index = -1;
			for(int i=0; i <= p->keys_count; i++) {
				if(y == p->p[i]) {
					index = i;
					break;
				}
			}

			/* shift rest values of parent to right */
			for(int i = p->keys_count; i > index; i--)
				p->keys[i] = p->keys[i-1];

			/* shift rest child nodes of parent to the right */
			for(int i = p->keys_count + 1; i > index; i--)
				p->p[i] = p->p[i-1];

			/* copy median to the parent here */
			p->keys[index] = median;

			/* attach w to this parent */
			p->p[index+1] = w;

			/* increment key count */
			p->keys_count++;
		}
		/* move to parent */
		y = p;
	}
}

/** Inserts the given key into the B tree */
void insert(int key) {

	node *y = NULL;
	node *x = root;

	/* search where to put the key */
	while(x) {

		y = x;	/* hold the parent node */

		int found = 0;
		for(int i=0; i < x->keys_count; i++) {
			if(key < x->keys[i]) {
				x = x->p[i];
				found = 1;
				break;
			}
		}
		if(!found)
			x = x->p[x->keys_count];
	}

	if(!y)
		root = create_new_node(key);

	else {
		/* find where to keep this value */
		int index = 0;
		while(index < y->keys_count && y->keys[index] < key)
			index++;

		for(int i = y->keys_count; i > index; i--)	/* shift keys to the right */
			y->keys[i] = y->keys[i-1];

		for(int i = y->keys_count + 1; i > index; i--)	/* shift children to the right */
			y->p[i] = y->p[i-1];

		y->keys[index] = key;	/* insert the key */

		y->keys_count++;	/* increment key count */

		if(is_overflow(y))	/* violates BTree property */
			insert_fixup(y);
	}
}

/** checks if the given node underflows */
int is_underflow(node *y) {
	return y && y->keys_count < T - 1;
}

/** checks if the given node has enough keys to donate */
int has_enough_keys(node *y) {
	return y && y->keys_count > T - 1;
}

/** checks if the given node is empty */
int is_empty(node *y) {
	return y->keys_count == 0;
}

/** fixes the underflow for the node */
void delete_fixup(node *y) {
	
}

/** searches the key in the B Tree and
 *  returns the node containing that key */
node *search(node *root, int key) {

	if(!root)
		return NULL;

	/* search and recur if not found */
	for(int i=0; i < root->keys_count; i++) {
		if(key < root->keys[i])
			return search(root->p[i], key);
		if(key > root->keys[i] && key < root->keys[i+1])
			return search(root->p[i + 1], key);
	} if(key > root->keys[root->keys_count - 1])
		return search(root->p[root->keys_count], key);

	return root;	/* found */
}

node *tree_minimum(node *root) {
	if(root->p[0])
		return tree_minimum(root->p[0]);
	return root;
}

node *tree_maximum(node *root) {
	for(int i = root->keys_count; i >= 0; i--)
		if(root->p[i])
			return tree_maximum(root->p[i]);
	return root;
}

/** checks if the given node is a leaf node */
int is_leaf(node *x) {
	return x->p[0] == NULL && x->p[1] == NULL;
}

/** checks if the given node is a root node */
int is_root(node *x) {
	return root == x;
}

void delete(int key) {
	
}

void inorder(node *root) {
	if(root) {
		for(int i=0; i < root->keys_count; i++) {
			inorder(root->p[i]);				/* left */
			printf("%d ", root->keys[i]);		/* root */
		}
		inorder(root->p[root->keys_count]);		/* right */
	}
}

void preorder(node *root) {
	if(root) {
		for(int i=0; i < root->keys_count; i++) {
			printf("%d ", root->keys[i]);		/* root */
			preorder(root->p[i]);				/* left */
		}
		preorder(root->p[root->keys_count]);	/* right */
	}
}

int main() {

	printf("Enter the order of the BTree: ");
	scanf("%d", &T);

	while(T < 2) {
		printf("Minimum order for BTree is 2\n");
		printf("Enter the order of the BTree: ");
		scanf("%d", &T);
	}

	MAX = 2*T - 1;
	printf("\nMaximum keys per node should be strictly less than: %d\n"
			"If # keys in a node become == %d then we split.\n", MAX, MAX);
	
	while(1) {

		printf("\n+-------------------------+\n"
				"[1] Insert into B tree.\n"
				"[2] Delete from B tree.\n"
				"[3] Search for a key.\n"
				"[4] Display Inorder\n"
				"[5] Display Preorder\n"
				"[6] Exit\n"
				"Enter your choice: ");

		char ch;
		scanf(" %c", &ch);

		switch(ch) {
			case '1':
				printf("Enter the key : ");
				int key;
				scanf("%d", &key);
				if(search(root, key)) {	/* first check if the key already exists */
					printf("%d already exists in B tree.\n", key);
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
					printf("B is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(!search(root, key)) {
					printf("%d is NOT present in the B tree.\n", key);
					break;
				}
				delete(key);
				if(root)
					printf("%d successfully deleted.\n", key);
				else if(!root)
					printf("B Tree is now empty!\n");
				else {
					printf("Error occurred.\n");
					exit(1);
				}
				break;

			case '3':
				printf("Enter the key : ");
				scanf("%d", &key);
				if(search(root, key))
					printf("%d is present in the B tree.\n", key);
				else
					printf("%d NOT FOUND in the B tree.\n", key);
				break;

			case '4':
				if(!root)
					printf("B tree is empty\n");
				else {
					inorder(root);
					printf("\n");
				}
				break;

			case '5':
				if(!root)
					printf("B tree is empty\n");
				else {
					preorder(root);
					printf("\n");
				}
				break;

			case '6':
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
