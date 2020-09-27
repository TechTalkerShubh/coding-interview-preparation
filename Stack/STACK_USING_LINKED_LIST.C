/*
 * STACK DS using Linked List DS
 * Implementation of STACK and basic operations on STACK using Linked List:
 * - Push operation O(1)
 * - Pop operation O(1)
 * - Peek operation O(1)
 * - Search operation O(n)
 * - Display operation O(n)
 *
 * @author ABHISHEK
 */

#include<stdio.h>
#include<stdlib.h>

/*
 * Linked List Node
 */
struct node {
	int data;
	struct node *next;
}*TOP = NULL;

typedef struct node node;

/*
 * Checks if the STACK is empty
 */
int is_empty() {
	return TOP == NULL;
}

/*
 * Prints the items of the STACK
 */
void display() {
	/*
	 * STACK Visualizer
	 */
	printf("+------------+\n");
	printf("TOP -> ");
	for(node *temp = TOP; temp; temp = temp->next)
		printf("\t%d\n", temp->data);
	printf("+------------+\n");
}

/*
 * Checks if the given item is present in the STACK
 * Returns the status, 1 if found, 0 otherwise
 */
int search(int item) {
	/*
	 * Applying a linear search O(n)
	 */
	for(node *temp = TOP; temp; temp = temp->next) {
		if (temp->data == item)
			return 1;
	}
	return 0;
}

/*
 * Returns the TOP item of the STACK
 */
int peek() {
	return TOP->data;
}

/*
 * Extracts the TOP item from the STACK
 */
int pop() {
	int top = TOP->data;
	node *temp = TOP;
	TOP = TOP->next;
	free(temp);
	return top;
}

/*
 * Creates a new node with the given item
 */
node *create_new_node(int item) {
	node *mem = NULL;
	mem = (node *)malloc(sizeof(node));
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

	if(!PTR)
		return 0;
	if(!TOP)
		TOP = PTR;
	else {
		PTR->next = TOP;
		TOP = PTR;
	}
	return 1;
}

int main() {

	while (1) {

		printf("\n+------------------------+\n"
				"[1] Push\n"
				"[2] Pop\n"
				"[3] Peek\n"
				"[4] Search\n"
				"[5] Display\n"
				"[6] Exit\n"
				"Enter your choice: ");
		char ch;
		scanf(" %c", &ch);

		switch (ch) {
		case '1':
			printf("Enter the item: ");
			int item;
			scanf("%d", &item);
			if(push(item))
				printf("%d successfully inserted\n", item);
			else
				printf("MEMORY FAILURE!\n");
			break;

		case '2':
			if (is_empty())
				printf("STACK UNDERFLOWS!\n");
			else
				printf("%d popped out\n", pop());
			break;

		case '3':
			if (is_empty())
				printf("STACK is empty!\n");
			else
				printf("Item at the TOP -> %d\n", peek());
			break;

		case '4':
			if (is_empty())
				printf("STACK is empty!\n");
			else {
				printf("Enter the item: ");
				scanf(" %d", &item);
				printf("%d is%s present in the STACK\n", item,
						search(item) ? "\0" : " NOT");
			}
			break;

		case '5':
			if (is_empty())
				printf("STACK is empty!\n");
			else
				display();
			break;

		case '6':
			free(TOP);
			exit(1);
			break;

		default:
			printf("Please enter a valid choice.\n");
		}
	}
	return 0;
}
