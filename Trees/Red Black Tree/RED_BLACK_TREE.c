/**
  * Implementation and Basic Operations (Insert, Delete, Search, Display) on Red-Black trees.
  *
  * Following Insertion Cases are applied to restore RB tree properties:
  * Case 1: z's uncle y is RED (Left Sub Tree)
  * Case 2: z’s uncle y is BLACK and z is a right child (Left Sub Tree)
  * Case 3: z’s uncle y is BLACK and z is a left child (Left Sub Tree)
  * Case 4: symmetric to case 1
  * Case 5: symmetric to case 2
  * Case 6: symmetric to case 3
  *
  * Following Deletion Cases are applied to restore RB tree properties:
  * Case 1: x's sibling w is RED
  * Case 2: x's sibling w is BLACK and both of w's children are BLACK
  * Case 3: x's sibling w is BLACK, w's left child is RED and w's right child is BLACK
  * Case 4: x's sibling w is BLACK and w's right child is RED
  * ** Symmetric cases also exist **
  *
  **/

#include<stdio.h>
#include<stdlib.h>

/* RB tree node */
struct node {
	int key;
	char color;
	struct node *left;
	struct node *right;
	struct node *parent;
};

typedef struct node node;

node *root;

void insert(node*, int);
void insert_fixup(node*, node*);
void left_rotate(node*, node*);
void right_rotate(node*, node*);
void delete(node*, int);
node *tree_minimum(node*, node*);
node *tree_maximum(node*, node*);
int search(node*, node*, int);
void inorder(node*, node*);

int main() {

	/* Initialize sentinel node */
	node *nil = (node*)malloc(sizeof(node));
	nil->color = 'B';
	nil->parent = nil->left = nil->right = NULL;

	root = nil;

	while(1) {

		printf("+-------------------------+\n"
				"[1] Insert into RB tree.\n"
				"[2] Delete from RB tree.\n"
				"[3] Search in RB tree.\n"
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
				if(search(nil, root, key)) {	/* first check if the key already exists */
					printf("%d already exists in RB tree.\n", key);
					break;
				}
				insert(nil, key);
				if(root)
					printf("%d successfully inserted.\n", key);
				else {
					printf("Error occurred.\n");
					exit(1);
				}
				break;

			case '2':
				if(root == nil) {
					printf("RB is empty!\n");
					break;
				}
				printf("Enter the key : ");
				scanf("%d", &key);
				if(!search(nil, root, key)) {
					printf("%d is NOT present in the RB.\n", key);
					break;
				}
				delete(nil, key);
				if(root != nil)
					printf("Success\n");
				else if(root == nil)
					printf("RB Tree is now empty!\n");
				else {
					printf("Error occurred.\n");
					exit(1);
				}
				break;

			case '3':
				printf("Enter the key : ");
				scanf("%d", &key);
				int found = search(nil, root, key);
				if(found)
					printf("%d is present in the RB tree.\n", key);
				else
					printf("%d NOT found in the RB tree.\n", key);
				break;

			case '4':
				if(root == nil)
					printf("RB tree is empty\n");
				else
					inorder(nil, root);
				break;

			case '5':
				printf("Enter 'y' to exit : " );
				char ch;
				scanf(" %c", &ch);
				if(ch == 'y') {
					free(root);
					free(nil);
					exit(1);
				}
				break;

			default:
				printf("Invalid choice! Try again...\n");
		}
	}

	return 0;
}

void insert(node *nil, int key) {

	node *z = (node*)malloc(sizeof(node));

	z->key = key;

	node *y = nil;			/* holds the parent of this node */
	node *x = root;			/* holds the current node */

	while(x != nil) {
		y = x;
		if(z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}

	/* assign parent for this node */
	if(y == nil)
		z->parent = nil;
	else
		z->parent = y;

	if(y == nil)				/* tree was initially empty */
		root = z;
	else if(z->key < y->key)	/* compare with root node and append to either left or right */
		y->left = z;
	else
		y->right = z;

	z->left = nil;
	z->right = nil;
	z->color = 'R';			/* new node is always RED */

	insert_fixup(nil, z);		/* check and balance the new tree */

}

void insert_fixup(node *nil, node *z) {

	/**
	 * For the first node in the beginning,
	 * sentinel (the parent) will be BLACK
	 * so while loop fails
	 */

	while(z->parent->color == 'R') {	/* Parent node is Red. */

		if(z->parent == z->parent->parent->left)  {	/* Left Sub Tree */

			node *y = z->parent->parent->right;		/* uncle of the new node (parent's sibling) */

			if(y != nil && y->color == 'R') {	/* case #1 */

				printf("*** Case 1 occurred ***\n");

				/* change the color of parent and uncle to BLACK */
				z->parent->color = y->color = 'B';

				/* change color of grandparent */
				z->parent->parent->color = 'R';

				/* move up two nodes */
				z = z->parent->parent;
			}
			else if(z == z->parent->right) {		/* case #2 */

				printf("*** Case 2 occurred ***\n");

				/* move up one node */
				z = z->parent;

				/* left rotate node 'z' */
				left_rotate(nil, z);
			}
			else {									/* case #3 */

				printf("*** Case 3 occurred ***\n");

				/* change parent color to BLACK */
				z->parent->color = 'B';

				if(z->parent->parent != nil) {	/* grandparent is not the sentinel */

					/* change grandparent color to RED */
					z->parent->parent->color = 'R';

					/* right rotate grandparent */
					right_rotate(nil, z->parent->parent);
				}
			}
		}
		else {	/* Right Sub Tree */

			node *y = z->parent->parent->left;	/* uncle of the new node (parent's sibling) */

			if(y != nil && y->color == 'R') {	/* case #4 */

				printf("*** Case 4 occurred ***\n");

				/* change the color of parent and uncle */
				z->parent->color = y->color = 'B';

				/* change color of grandparent */
				z->parent->parent->color = 'R';

				/* move up two nodes */
				z = z->parent->parent;
			}
			else if(z == z->parent->left) {			/* case #5 */

				printf("*** Case 5 occurred ***\n");

				/* move up one node */
				z = z->parent;

				/* right rotate node 'z' */
				right_rotate(nil, z);
			}
			else {									/* case #6 */

				printf("*** Case 6 occurred ***\n");

				/* change parent color to BLACK */
				z->parent->color = 'B';

				/* grandparent is not the sentinel */
				if(z->parent->parent != nil) {

					/* change grandparent color to RED */
					z->parent->parent->color = 'R';

					/* left rotate grandparent */
					left_rotate(nil, z->parent->parent);
				}
			}
		}
	}

	/* if this is new root node,
	 * change the color to black */
	root->color = 'B';
}

void left_rotate(node *nil, node *gpar) {

	node *par = gpar->right;
	node *child = par->left;

	par->left = gpar;		/* grand-parent becomes left child of parent */
	gpar->right = child;	/* left child of parent becomes right child of grandparent */

	/* fix parent links */
	if(gpar == root)
		root = par;
	else if(gpar == gpar->parent->left)
		gpar->parent->left = par;
	else
		gpar->parent->right = par;
	par->parent = gpar->parent;
	gpar->parent = par;
	if(child != nil)
		child->parent = gpar;

	printf("[%d, %c] was left rotated.\n", gpar->key, gpar->color);
}

void right_rotate(node *nil, node *gpar) {

	node *par = gpar->left;
	node *child = par->right;

	par->right = gpar;		/* grand-parent becomes right child of parent */
	gpar->left = child;		/* right child of parent becomes left child of grand-parent */

	/* fix parent links */
	if(gpar == root)
		root = par;
	else if(gpar == gpar->parent->left)
		gpar->parent->left = par;
	else
		gpar->parent->right = par;
	par->parent = gpar->parent;
	gpar->parent = par;
	if(child != nil)
		child->parent = gpar;

	printf("[%d, %c] was right rotated.\n", gpar->key, gpar->color);
}

/**
 * Symmetric cases also exist for below delete cases
 */

void delete_case_4(node *nil, node *x) {
	printf("*** Case 4 occurred ***\n");
	node *par = x->parent;
	left_rotate(nil, par);
	par->color = 'B';
	if(par->parent == root)
		par->parent->color = 'B';
	else
		par->parent->color = 'R';
	par->parent->right->color = 'B';
}

void delete_case_3(node *nil, node *x) {
	printf("*** Case 3 occurred ***\n");
	node *w = x->parent->right;
	right_rotate(nil, w);
	w->color = 'R';
	w->parent->color = 'B';
	delete_case_4(nil, x);
}

void delete_case_2(node *nil, node *x) {
	printf("*** Case 2 occurred ***\n");
	if(x->parent->color == 'R') {
		/* exchange the color between parent and sibling */
		char color = x->parent->color;
		x->parent->color = x->parent->right->color;
		x->parent->right->color = color;
	} else {
		/* parent becomes double black node */
		node *par = x->parent;
		par->right->color = 'R';
		if(par != root)
			delete_case_3(nil, par);
		printf("Double black at [%d, %c] resolved.\n", par->key, par->color);
	}
}

void delete_case_1(node *nil, node *x) {
	printf("*** Case 1 occurred ***\n");
	left_rotate(nil, x->parent);
	x->parent->color = 'R';
	x->parent->parent->color = 'B';
}

void delete_red_leaf(node *nil, node *x) {
	if(x == x->parent->left)
		x->parent->left = nil;
	else
		x->parent->right = nil;
	free(x);
}

void delete_black_node_with_red_child(node *nil, node *x) {
	/* change the color of the RED child */
	if(x == x->parent->left) {		/* red node is the right child */
		x->parent->left = x->right;
		x->right->parent = x->parent;
		x->right->color = 'B';
	} else {						/* red node is the left child */
		x->parent->right = x->left;
		x->left->parent = x->parent;
		x->left->color = 'B';
	}
	free(x);
}

int is_case_1(node *nil, node *x) {
	node *w = x->parent->right;
	return w != nil
			&& w->color == 'R';
}

int is_case_2(node *nil, node *x) {
	node *w = x->parent->right;
	return w != nil
			&& w->left->color == 'B'
			&& w->right->color == 'B';
}

int is_case_3(node *nil, node *x) {
	node *w = x->parent->right;
	return w != nil
			&& w->left != nil
			&& w->left->color == 'R'
			&& w->right->color == 'B';
}

int is_case_4(node *nil, node *x) {
	node *w = x->parent->right;
	return w != nil
			&& w->right != nil
			&& w->right->color == 'R';
}

void handle_and_delete(node *nil, node *x) {

	if(x->right == nil &&
			x->color == 'R') {			/* a RED leaf node */

		/* we can safely delete a RED leaf node
		 * because it does not affect the
		 * no. of black nodes on that path */

		delete_red_leaf(nil, x);

	} else if(x->right != nil &&
			x->color == 'B' &&
			x->right->color == 'R') {	/* BLACK node with a RED child */

		delete_black_node_with_red_child(nil, x);

	} else { /* a BLACK leaf node */

		/* since a black node is to be deleted,
		 * the new nil node will carry the
		 * overhead of 1 black node */

		/*******************************************
		 * For this case we have 4 different cases *
		 *******************************************/

		if (is_case_1(nil, x)) {

			delete_case_1(nil, x);
			x->parent->left = nil;
			free(x);

		} else if(is_case_2(nil, x)) {

			delete_case_2(nil, x);
			x->parent->left = nil;
			free(x);

		} else if(is_case_3(nil, x)) {

			delete_case_3(nil, x);
			x->parent->left = nil;
			free(x);

		} else if(is_case_4(nil, x)) {

			delete_case_4(nil, x);
			x->parent->left = nil;
			free(x);

		} else
			printf("No deletion case encountered for this node!\n"
					"If we delete this node, we can not restore"
					"RB Tree property!\n");
	}
}

void delete(node *nil, int key) {

	/* find the node with this key */
	node *z = root;
	while(z->key != key) {
		if(key < z->key)
			z = z->left;
		else
			z = z->right;
	}

	if(z->right != nil) {

		node *x = tree_minimum(nil, z->right);	/* inorder successor */

		z->key = x->key;

		handle_and_delete(nil, x);

	} else if(z->left != nil) {

		/* same cases as above with "left" and "right" exchanged
		 * delete cases for black leaf node will also be symmetric */

		printf("Sorry, symmetric cases are not handled!\n");

	} else {
		if(z == root) {					/* root node */
			free(root);
			root = nil;
		}
		else if(z == z->parent->left)	/* left child */
			handle_and_delete(nil, z);

		else							/*right child*/
			printf("Sorry, symmetric cases are not handled!\n");
	}
}

node *tree_minimum(node *nil, node *root) {
	if(root->left != nil)
		return tree_minimum(nil, root->left);
	return root;
}

node *tree_maximum(node *nil, node *root) {
	if(root->right != nil)
		return tree_maximum(nil, root->right);
	return root;
}

int search(node *nil, node *root, int key) {
	if(root == nil)
		return 0;
	if(key > root->key)
		return search(nil, root->right, key);
	if(key < root->key)
		return search(nil, root->left, key);
	return 1;
}

void inorder(node *nil, node *root) {
	if(root != nil) {
		inorder(nil, root->left);
		printf("[%d, %c] ", root->key, root->color);
		inorder(nil, root->right);
	}
}
