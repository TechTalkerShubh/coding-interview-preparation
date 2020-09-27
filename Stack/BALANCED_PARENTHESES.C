/*
 * Given an expression containing:
 * '(', ')', '{', '}', '[' and ']'
 * Find whether the expression is balanced.
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

	char string[MAX] = { 0 }; /* string initialized to null */

	printf("Enter the expression: ");
	scanf("%s", string);

	push(string[0]);

	for (int i = 1; string[i]; i++) {
		if (string[i] == ')' && peek() == '(')
			pop();
		else if (string[i] == '}' && peek() == '{')
			pop();
		else if (string[i] == ']' && peek() == '[')
			pop();
		else
			push(string[i]);
	}

	if (is_empty())
		printf("BALANCED!\n");
	else
		printf("NOT BALANCED!\n");

	return 0;
}
