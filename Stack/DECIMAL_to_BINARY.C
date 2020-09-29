/*
 * Decimal Number System to Binary Number System using STACK.
 *
 * @author ABHISHEK
 */

#include<stdio.h>
#include<stdlib.h>

/*
 * Linked List Node
 */
struct node {
	char data;
	struct node *next;
} *TOP = NULL;

typedef struct node node;

int N = 0;	/* records the count of items in the stack */

/*
 * Checks if the STACK is empty
 */
int is_empty() {
	return TOP == NULL;
}

/*
 * Returns the TOP item of the STACK
 */
char peek() {
	return TOP->data;
}

/*
 * Extracts the TOP item from the STACK
 */
char pop() {
	int top = TOP->data;
	node *temp = TOP;
	TOP = TOP->next;
	free(temp);
	N--;
	return top;
}

/*
 * Creates a new node with the given item
 */
node* create_new_node(int item) {
	node *mem = NULL;
	mem = (node*) malloc(sizeof(node));
	mem->data = item;
	mem->next = NULL;
	return mem;
}

/*
 * Inserts the given item into the STACK
 * Returns the status, 1 if success, 0 otherwise
 */
int push(int item) {

	node *PTR = create_new_node(item);

	if (!PTR)
		return 0;
	if (!TOP)
		TOP = PTR;
	else {
		PTR->next = TOP;
		TOP = PTR;
	}
	N++;
	return 1;
}

int main() {

	int decimal;

	printf("Enter the Decimal Value: ");
	scanf("%d", &decimal);

	while(decimal > 0) {
		push(decimal & 1);	/* Bitwise AND */
		decimal >>= 1;	/* Right Shift */
	}

	char binary[N+1];
	int i = 0;

	while(!is_empty()) {
		binary[i++] = pop() + '0';
	}
	binary[i] = '\0';

	printf("Binary Equivalent: %s\n", binary);

	return 0;
}
