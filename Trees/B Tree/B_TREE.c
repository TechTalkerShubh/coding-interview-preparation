/**
 * B Tree Implementation and Basic Operations (Insert, Delete, Search, Display) on B tree.
 *
 * If T is the order, then every node must have:
 * 	at least T-1 keys (except Root node), and
 * 	at least T children
 * In general,
 * T   <= # of child < 2 * T
 * T-1 <= # of keys  < 2 * T - 1
 *
 **/

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

int T; /* order of the BTree */
int MAX; /* # of keys should be strictly less than the max value
 	 	  * # of child should be less than or equal to the max value */

/* B tree node */
struct node {
	int keys[100]; /* array stores at-most 2T-1 keys in sorted fashion */
	int keys_count; /* holds the no. of keys this node currently holds */
	struct node *p[100 + 1]; /* holds the child nodes */
	struct node *parent; /* parent node */
};

typedef struct node node;

node *root = NULL;

/** creates a new node */
node* create_new_node(int key) {
	node *mem = (node*) malloc(sizeof(node));
	mem->keys[0] = key;
	for (int i = 1; i < MAX; i++)
		mem->keys[i] = INT_MAX;
	for (int i = 0; i <= MAX; i++)
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

	while (is_overflow(y)) { /* until we have a parent which does not overflow */

		printf("Split at [%d", y->keys[0]);
		for (int i = 1; i < MAX; i++)
			printf(", %d", y->keys[i]);
		printf("] occurred\n");

		/* get median */
		int median = y->keys[MAX / 2];

		/* split this node */
		node *w = create_new_node(y->keys[MAX / 2 + 1]);
		for (int i = MAX / 2 + 2, j = 1; i < MAX; i++, j++) { /* copy values */
			w->keys[j] = y->keys[i];
			w->keys_count++;
		}
		for (int i = MAX / 2 + 1, j = 0; i <= MAX; i++, j++) /* copy children */
			w->p[j] = y->p[i];

		/* fix child links for the split node */
		for (int i = 0; i <= w->keys_count; i++)
			if (w->p[i])
				w->p[i]->parent = w;

		/* reset */
		for (int i = MAX / 2; i < MAX; i++) {
			y->keys[i] = INT_MAX;
			y->p[i + 1] = NULL;
			y->keys_count--;
		}

		/* get parent for this node */
		node *p = y->parent;

		/* pull the median up to the parent */
		if (!p) {

			/* create new root node */
			root = create_new_node(median);

			/* set parent for both the splits */
			y->parent = w->parent = root;

			/* fix child links for the new root */
			root->p[0] = y; /* left child */
			root->p[1] = w; /* right child */

			break;

		} else {

			/* set parent for the new split */
			w->parent = p;

			/* find where the node y exists */
			int index = -1;
			for (int i = 0; i <= p->keys_count; i++) {
				if (y == p->p[i]) {
					index = i;
					break;
				}
			}

			/* shift rest values of parent to right */
			for (int i = p->keys_count; i > index; i--)
				p->keys[i] = p->keys[i - 1];

			/* shift rest child nodes of parent to the right */
			for (int i = p->keys_count + 1; i > index; i--)
				p->p[i] = p->p[i - 1];

			/* copy median to the parent here */
			p->keys[index] = median;

			/* attach w to this parent */
			p->p[index + 1] = w;

			/* increment key count */
			p->keys_count++;
		}
		/* move to parent */
		y = p;
	}
	printf("Overflow resolved!\n");
}

/** Inserts the given key into the B tree */
void insert(int key) {

	node *y = NULL;
	node *x = root;

	/* search where to put the key */
	while (x) {

		y = x; /* hold the parent node */

		int found = 0;
		for (int i = 0; i < x->keys_count; i++) {
			if (key < x->keys[i]) {
				x = x->p[i];
				found = 1;
				break;
			}
		}
		if (!found)
			x = x->p[x->keys_count];
	}

	if (!y)
		root = create_new_node(key);

	else {
		/* find where to keep this value */
		int index = 0;
		while (index < y->keys_count && y->keys[index] < key)
			index++;

		for (int i = y->keys_count; i > index; i--) /* shift keys to the right */
			y->keys[i] = y->keys[i - 1];

		for (int i = y->keys_count + 1; i > index; i--) /* shift children to the right */
			y->p[i] = y->p[i - 1];

		y->keys[index] = key; /* insert the key */

		y->keys_count++; /* increment key count */

		if (is_overflow(y)) /* violates BTree property */
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

/** checks if the given node is a leaf node */
int is_leaf(node *x) {
	return x->p[0] == NULL && x->p[1] == NULL;
}

/** checks if the given node is a root node */
int is_root(node *x) {
	return root == x;
}

/** fixes the underflow for the node */
void delete_fixup(node *y) {

	while (!is_root(y) && is_underflow(y)) { /* until we have a node which violates the BTree property */

		/* visualizer */
		printf("Underflow at [");
		if (!is_empty(y))
			printf("%d", y->keys[0]);
		for (int i = 1; i < y->keys_count; i++)
			printf(", %d", y->keys[i]);
		printf("]\n");

		/* ask the parent for help */
		node *p = y->parent;

		int index = -1;

		/* get y's index in p */
		for (int i = 0; i <= p->keys_count; i++) {
			if (y == p->p[i]) {
				index = i;
				break;
			}
		}

		node *x = NULL; /* left sibling */
		node *z = NULL; /* right sibling */

		if (index > 0)
			x = p->p[index - 1];
		if (index < p->keys_count)
			z = p->p[index + 1];

		if (has_enough_keys(z)) { /* z exists and has enough ( > T-1 ) keys to donate */

			/* exchange the keys between y, p and z */
			y->keys[y->keys_count] = p->keys[index];
			p->keys[index] = z->keys[0]; /* first key in z */

			/* first child of z becomes last child of y  */
			y->p[y->keys_count + 1] = z->p[0];
			if (y->p[y->keys_count + 1])
				y->p[y->keys_count + 1]->parent = y;

			/* update / reset z */
			for (int i = 0; i < z->keys_count - 1; i++) /* left shift */
				z->keys[i] = z->keys[i + 1];
			z->keys[z->keys_count - 1] = INT_MAX;

			/* fix links for z */
			for (int i = 0; i < z->keys_count; i++)
				z->p[i] = z->p[i + 1];
			z->p[z->keys_count] = NULL;

			/* update key counts */
			y->keys_count++;
			z->keys_count--;

			printf("Keys Left Rotated via Parent\n");

		} else if (has_enough_keys(x)) { /* x has enough ( > T-1 ) keys to donate */

			/* shift 'y' keys to the right */
			for (int i = y->keys_count; i > 0; i--)
				y->keys[i] = y->keys[i - 1];

			/* shift 'y' child nodes to the right */
			for (int i = y->keys_count + 1; i > 0; i--)
				y->p[i] = y->p[i - 1];

			/* exchange keys between x, y and p */
			y->keys[0] = p->keys[index - 1];
			p->keys[index - 1] = x->keys[x->keys_count - 1]; /* last key in x */

			/* last child of x becomes first child of y */
			y->p[0] = x->p[x->keys_count];
			if (y->p[0])
				y->p[0]->parent = y;

			/* update x */
			x->keys[x->keys_count - 1] = INT_MAX;
			x->p[x->keys_count] = NULL;

			/* update key counts */
			y->keys_count++;
			x->keys_count--;

			printf("Keys Right Rotated via Parent\n");

		} else {

			/*********************
			 *  MERGE PROCEDURE
			 *********************/

			if (z) { /* shift p's key down and move 'z' to 'y' */

				/* visualizer */
				printf("Merging [");
				if (!is_empty(y))
					printf("%d", y->keys[0]);
				for (int i = 1; i < y->keys_count; i++)
					printf(", %d", y->keys[i]);
				printf("], [%d], and [%d", p->keys[index], z->keys[0]);
				for (int i = 1; i < z->keys_count; i++)
					printf(", %d", z->keys[i]);
				printf("]\n");

				/* shift parent key down */
				y->keys[y->keys_count++] = p->keys[index];

				/* copy z keys to y */
				for (int i = 0; i < z->keys_count; i++)
					y->keys[y->keys_count + i] = z->keys[i];

				/* copy z child nodes to y */
				for (int i = 0; i <= z->keys_count; i++) {
					y->p[y->keys_count + i] = z->p[i];
					if (y->p[y->keys_count + i])
						y->p[y->keys_count + i]->parent = y;
				}

				/* update key count */
				y->keys_count += z->keys_count;

				/* update / reset parent */
				for (int i = index; i < p->keys_count; i++) { /* shift left */
					p->keys[i] = p->keys[i + 1];
					p->p[i + 1] = p->p[i + 2];
				}
				p->keys[p->keys_count - 1] = INT_MAX;
				p->p[p->keys_count] = NULL;
				p->keys_count--;

				/* here there may be a chance that p is now empty,
				 * so if p was the root AND is now empty
				 * y becomes the new root */
				if (is_root(p) && is_empty(p)) {
					root = y;
					free(p);
					p = NULL;
					y->parent = NULL;
				}

				free(z);

				printf("Merge successful!\n");

			} else { /* shift p's key down and move 'y' to 'x' */

				/* visualizer */
				printf("Merging [%d", x->keys[0]);
				for (int i = 1; i < x->keys_count; i++)
					printf(", %d", x->keys[i]);
				printf("], [%d], and [", p->keys[index - 1]);
				if (!is_empty(y))
					printf("%d", y->keys[0]);
				for (int i = 1; i < y->keys_count; i++)
					printf(", %d", y->keys[i]);
				printf("]\n");

				/* shift parent key down */
				x->keys[x->keys_count++] = p->keys[index - 1];

				/* copy y keys to x */
				for (int i = 0; i < y->keys_count; i++) {
					x->keys[x->keys_count + i] = y->keys[i];
				}

				/* copy y child nodes to x */
				for (int i = 0; i <= y->keys_count; i++) {
					x->p[x->keys_count + i] = y->p[i];
					if (x->p[x->keys_count + i])
						x->p[x->keys_count + i]->parent = x;
				}

				/* update key count */
				x->keys_count += y->keys_count;

				/* update / reset parent */
				for (int i = index - 1; i < p->keys_count; i++) { /* shift left */
					p->keys[i] = p->keys[i + 1];
					p->p[i + 1] = p->p[i + 2];
				}
				p->keys[p->keys_count - 1] = INT_MAX;
				p->p[p->keys_count] = NULL;
				p->keys_count--;

				/* here there may be a chance that p is now empty,
				 * so if p was the root AND is now empty
				 * x becomes the new root */
				if (is_root(p) && is_empty(p)) {
					root = x;
					free(p);
					p = NULL;
					x->parent = NULL;
				}

				free(y);

				printf("Merge successful!\n");
			}
		}
		/* move to the parent */
		y = p;
	}
	printf("Underflow resolved!\n");
}

/** searches the key in the B Tree and
 *  returns the node containing that key */
node* search(node *root, int key) {
	if (!root) /* not found */
		return NULL;
	for (int i = 0; i < root->keys_count; i++) { /* search and recur if no match */
		if (key == root->keys[i])
			return root; /* found */
		if (key < root->keys[i])
			return search(root->p[i], key);
	}
	return search(root->p[root->keys_count], key);
}

node* tree_minimum(node *root) {
	if (root->p[0])
		return tree_minimum(root->p[0]);
	return root;
}

node* tree_maximum(node *root) {
	for (int i = root->keys_count; i >= 0; i--)
		if (root->p[i])
			return tree_maximum(root->p[i]);
	return root;
}

/** deletes the specified key from the BTree */
void delete(int key) {

	/* first search the node which has this key */
	node *x = search(root, key);

	int index = -1;

	/* find the index of the key */
	for (int i = 0; i < x->keys_count; i++) {
		if (key == x->keys[i]) {
			index = i;
			break;
		}
	}

	if (is_leaf(x)) {

		for (int i = index; i < x->keys_count - 1; i++) /* shift items to the left */
			x->keys[i] = x->keys[i + 1];

		x->keys[x->keys_count - 1] = INT_MAX;

		x->keys_count--; /* decrease count */

		if (is_underflow(x)) { /* violates BTree property */
			if (is_root(x)) {
				if (x->keys_count == 0) { /* root node is exception but is supposed to have at least one key */
					free(root);
					root = NULL;
				}
			} else
				delete_fixup(x);
		}

	} else {

		/* find inorder successor for this key */
		node *y = tree_minimum(x->p[index + 1]);

		/* exchange the value between x and y */
		x->keys[index] = y->keys[0];

		/* update / reset the entries in y */
		for (int i = index; i < y->keys_count - 1; i++)
			y->keys[i] = y->keys[i + 1];

		y->keys[y->keys_count - 1] = INT_MAX;

		y->keys_count--;

		if (is_underflow(y)) /* violates BTree property */
			delete_fixup(y);
	}
}

void inorder(node *root) {
	if (root) {
		for (int i = 0; i < root->keys_count; i++) {
			inorder(root->p[i]); /* left */
			printf("%d ", root->keys[i]); /* root */
		}
		inorder(root->p[root->keys_count]); /* right */
	}
}

void preorder(node *root) {
	if (root) {
		for (int i = 0; i < root->keys_count; i++) {
			printf("%d ", root->keys[i]); /* root */
			preorder(root->p[i]); /* left */
		}
		preorder(root->p[root->keys_count]); /* right */
	}
}

int main() {

	printf("Enter the order of the BTree: ");
	scanf("%d", &T);

	while (T < 2) {
		printf("Minimum order for BTree is 2\n");
		printf("Enter the order of the BTree: ");
		scanf("%d", &T);
	}

	MAX = 2 * T - 1;

	printf("\nMaximum keys per node should be strictly less than: %d\n"
			"If # keys in a node become == %d then we split.\n\n", MAX, MAX);

	while (1) {

		printf("+-------------------------+\n"
				"[1] Insert into B tree.\n"
				"[2] Delete from B tree.\n"
				"[3] Search for a key.\n"
				"[4] Display Inorder\n"
				"[5] Display Preorder\n"
				"[6] Exit\n"
				"Enter your choice: ");

		char ch;
		scanf(" %c", &ch);

		switch (ch) {
		case '1':
			printf("Enter the key : ");
			int key;
			scanf("%d", &key);
			if (search(root, key)) { /* first check if the key already exists */
				printf("%d already exists in B tree.\n", key);
				break;
			}
			insert(key);
			if (root)
				printf("%d successfully inserted.\n", key);
			else {
				printf("Error occurred.\n");
				exit(1);
			}
			break;

		case '2':
			if (!root) {
				printf("B is empty!\n");
				break;
			}
			printf("Enter the key : ");
			scanf("%d", &key);
			if (!search(root, key)) {
				printf("%d is NOT present in the B tree.\n", key);
				break;
			}
			delete(key);
			if (root)
				printf("%d successfully deleted.\n", key);
			else if (!root)
				printf("B Tree is now empty!\n");
			else {
				printf("Error occurred.\n");
				exit(1);
			}
			break;

		case '3':
			printf("Enter the key : ");
			scanf("%d", &key);
			if (search(root, key))
				printf("%d is present in the B tree.\n", key);
			else
				printf("%d NOT FOUND in the B tree.\n", key);
			break;

		case '4':
			if (!root)
				printf("B tree is empty\n");
			else {
				inorder(root);
				printf("\n");
			}
			break;

		case '5':
			if (!root)
				printf("B tree is empty\n");
			else {
				preorder(root);
				printf("\n");
			}
			break;

		case '6':
			printf("Enter 'y' to exit : ");
			char ch;
			scanf(" %c", &ch);
			if (ch == 'y') {
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
