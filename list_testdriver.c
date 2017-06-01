/***************************************************************
 * Test driver for the implementation of a custom list         *
 * data structure              								   *
 * Author: Shayne Kelly II                                     *
 * Date: May 19, 2017                                          *
 ***************************************************************/

/***************************************************************
 * Imports                                                     *
 ***************************************************************/
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/***************************************************************
 * Statics                                                     *
 ***************************************************************/
static void ListCreateTest();
static void printPassMessage(char *funcName, int numAssertions);

/***************************************************************
 * Main (test driver)                                          *
 ***************************************************************/
int main(void) {
	printf("*****************************************************\n");
	printf("* List tests running...                             *\n");
	printf("*****************************************************\n\n");

	ListCreateTest();

	printf("\n*****************************************************\n");
	printf("* All tests passed! Exiting...                      *\n");
	printf("*****************************************************\n\n");
	return 0;
}

/***************************************************************
 * Static Functions                                            *
 ***************************************************************/

/**
 * ListCreate test cases:
 * 1. Create a list, returns pointer to empty list, check list parameters
 * 2. Fill up all list head space, returns NULL
 */
static void ListCreateTest() {
	LIST *listArr[10];

	/* Attempt to create 10 lists (maximum number) successfully */
	for (int i = 0; i < 10; i++) {
		listArr[i] = ListCreate();
		assert(listArr[i] != NULL 
			&& "FAIL: List creation failed. List pointer is NULL.");
		assert(listArr[i]->current == NULL 
			&& "FAIL: Current pointer of new list is not NULL.");
		assert(listArr[i]->head == NULL 
			&& "FAIL: Head pointer of new list is not NULL.");
		assert(listArr[i]->tail == NULL 
			&& "FAIL: Tail pointer of new list is not NULL.");
		assert(listArr[i]->size == 0 
			&& "FAIL: Size of new list was non-zero.");
		assert(listArr[i]->currentIsBeyond == 0 
			&& "FAIL: The new list indicated the current pointer was outside the list.");
	}

	/* Attempt to create a list when list space is exhausted */
	LIST *list = ListCreate();
	assert(list == NULL
		&& "FAIL: A pointer was returned even though list space should be full.");

	printPassMessage("ListCreate", 61);
}

static void printPassMessage(char *funcName, int numAssertions) {
	printf("%s tests passed. %d assertions passed.\n", funcName, numAssertions);
}