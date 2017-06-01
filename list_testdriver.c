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
static void ListCountTest();
static void ListFirstTest();
static void ListTrimTest();

/***************************************************************
 * Main (test driver)                                          *
 ***************************************************************/
int main(void) {
	printf("\n\n*****************************************************\n");
	printf("* List tests running...                             *\n");
	printf("*****************************************************\n\n");

	ListCreateTest();
	ListCountTest();
	ListFirstTest();

	ListTrimTest();

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
 * Also checks: ListFree()
 */
static void ListCreateTest() {
	LIST *listArr[11];

	/* Test Case 1 */
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

	/* Test Case 2 */
	/* Attempt to create a list when list space is exhausted */
	listArr[10] = ListCreate();
	assert(listArr[10] == NULL
		&& "FAIL: A pointer was returned even though list space should be full.");

	/* Cleanup */
	for (int i = 0; i < 10; i++) {
		ListFree(listArr[i], NULL);
	}
	printf("ListCreate tests passed. 61 assertions passed.\n");
}

/**
 * 1. Create a list with 100 items (also tests ListAdd()).
 * 2. Delete all items and check count.
 * Also checks: ListAdd(), ListTrim(), ListFree()
 */
static void ListCountTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	int listItems[100];
	for (int i = 0; i < 100; i++) {
		listItems[i] = i;
		ListAdd(list, (void *)&listItems[i]);
		assert(ListCount(list) == i + 1
			&& "FAIL: List count did not reflect the actual list size");
	}

	/* Test Case 2 */
	for (int i = 100; i > 0; i--) {
		ListTrim(list);
		assert(ListCount(list) == i - 1
			&& "FAIL: List count did not reflect the actual list size");
	}

	/* Cleanup */
	ListFree(list, NULL);
	printf("ListCount tests passed. 200 assertions passed.\n");
}

/**
 * 1. Check first item of empty list returns NULL.
 * 2. Check first item of list with one item.
 * 3. Check first item of list with multiple items.
 * Also checks: ListCurrent(), ListFree()
 */
static void ListFirstTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	assert(ListFirst(list) == NULL
		&& "FAIL: Checking first item of an empty list did not return NULL\n");
	assert(list->current == NULL
		&& "FAIL: Checking first item modified the current item of an empty list\n");

	/* Test Case 2 */
	char testChar[3];
	testChar[0] = 'a';
	ListAdd(list, (void *)&testChar[0]);
	assert(ListFirst(list) == &testChar[0]
		&& "FAIL: Checking first item of a list with 1 item\n");
	assert(list->current->item == &testChar[0]
		&& "FAIL: The current item was not set to the first item for a list with 1 item\n");

	/* Test Case 3 */
	testChar[1] = 'b';
	ListAdd(list, (void *)&testChar[1]);
	assert(ListFirst(list) == &testChar[0]
		&& "FAIL: Checking first item of a list with multiple items\n");
	assert(list->current->item == &testChar[0]
		&& "FAIL: The current item was not set to the first item for a list with multiple items\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("ListFirst tests passed. 6 assertions passed.\n");
}






/**
 * 1. Trim list with one item.
 */
static void ListTrimTest() {
	LIST *list = ListCreate();
	int *listItem = (int *)malloc(sizeof(int));
	*listItem = 0;
	ListAdd(list, (void *)listItem);
	ListTrim(list);
	assert(list->size == 0);
	assert(list->current == NULL);
	assert(list->head == NULL);
	assert(list->tail == NULL);
}