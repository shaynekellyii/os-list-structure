/**
 * Test driver for the implementation of a custom list data structure
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	/* Create a list */
	LIST *list = ListCreate();

	/* Print the count */
	printf("Number of items in list: %d\n", ListCount(list));

	/* Add one item to list */
	printf("Adding an int (9) to the list\n");
	int randomInt = 9;
	ListAdd(list, (void *)&randomInt);

	/* Print the count */
	printf("Number of items in list: %d\n", ListCount(list));

	/* Get item from list */
	int *shouldBeRandomInt = ListFirst(list);
	printf("The first item in the list should be 9, it is %d\n", *(int*)shouldBeRandomInt);
}