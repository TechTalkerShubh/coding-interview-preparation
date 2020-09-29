/*
 * Decimal Number System to Binary Number System using STACK.
 *
 * @author ABHISHEK
 */

#include<stdio.h>
#include<stdlib.h>

#define MAX 100

/*
 * Linked List Node
 */
struct node {
	char data;
	struct node *next;
} *TOP = NULL;

typedef struct node node;

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
	return 1;
}

int main() {

	int decimal;

	printf("Enter the Decimal Value: ");
	scanf("%d", &decimal);

	while(decimal > 0) {
		push(decimal%2);
		decimal /= 2;
	}

	char binary[MAX] = { 0 };
	int i = 0;

	while(!is_empty()) {
		binary[i++] = pop() + '0';
	}

	printf("Binary Equivalent: %s\n", binary);

	return 0;
}

