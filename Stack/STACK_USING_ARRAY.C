/*
 * STACK DS using ARRAY DS
 * Implementation of STACK and basic operations on STACK using Array:
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

#define MAX 100

int STACK[MAX];
int TOP = -1;

/*
 * Checks if the STACK is full
 */
int is_full(int size) {
	return TOP == size - 1;
}

/*
 * Checks if the STACK is empty
 */
int is_empty() {
	return TOP == -1;
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
	for (int i = TOP; i >= 0; i--)
		printf("\t%d\n", STACK[i]);
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
	for (int i = 0; i <= TOP; i++) {
		if (STACK[i] == item)
			return 1;
	}
	return 0;
}

/*
 * Returns the TOP item of the STACK
 */
int peek() {
	return STACK[TOP];
}

/*
 * Extracts the TOP item from the STACK
 */
int pop() {
	return STACK[TOP--];
}

/*
 * Inserts the given item into the STACK
 * Returns the status, 1 if success, 0 otherwise.
 */
int push(int item, int size) {

	if (is_full(size))
		return 0;

	STACK[++TOP] = item;
	return 1;
}

int main() {

	printf("Enter the size of this STACK: ");
	int size;
	scanf("%d", &size);

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
			if (push(item, size))
				printf("%d successfully pushed\n", item);
			else
				printf("STACK OVERFLOWS!\n");
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
			exit(1);
			break;

		default:
			printf("Please enter a valid choice.\n");
		}
	}
	return 0;
}
