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
static void ListLastTest();
static void ListNextTest();
static void ListPrevTest();
static void ListCurrTest();
static void ListAddTest();
static void ListInsertTest();
static void ListAppendTest();
static void ListPrependTest();
static void ListRemoveTest();
static void ListConcatTest();
static void ListTrimTest();
static void ListSearchTest();

/***************************************************************
 * Globals                                                     *
 ***************************************************************/
int successComparator(void *item1, void *item2);
int failComparator(void *item1, void *item2);

/***************************************************************
 * Main (test driver)                                          *
 ***************************************************************/
int main(void) {
	printf("\n\n*****************************************************\n");
	printf("* CMPT 300 - Assignment 1 - List Data Structure     *\n");
	printf("* Shayne Kelly II 	                            *\n");
	printf("* List tests running...                             *\n");
	printf("*****************************************************\n\n");

	printf("------------------------------------------------------\n");
	printf("| Test        | # Test Cases | # Assertions | Status |\n");
	printf("------------------------------------------------------\n");

	ListCreateTest();
	ListCountTest();
	ListFirstTest();
	ListLastTest();
	ListNextTest();
	ListPrevTest();
	ListCurrTest();
	ListAddTest();
	ListInsertTest();
	ListAppendTest();
	ListPrependTest();
	ListRemoveTest();
	ListConcatTest();
	ListTrimTest();
	ListSearchTest();

	printf("------------------------------------------------------\n");
	printf("| TOTAL:      |     121      |      697     |  PASS  |\n");
	printf("------------------------------------------------------\n\n");
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
	LIST *listArr[11];

	/* Test Case 1 */
	/* Attempt to create 10 lists (maximum number) successfully */
	for (int i = 0; i < 10; i++) {
		listArr[i] = ListCreate();
		assert(listArr[i] != NULL 
			&& "FAIL: List creation failed. List pointer is NULL.\n");
		assert(listArr[i]->current == NULL 
			&& "FAIL: Current pointer of new list is not NULL.\n");
		assert(listArr[i]->head == NULL 
			&& "FAIL: Head pointer of new list is not NULL.\n");
		assert(listArr[i]->tail == NULL 
			&& "FAIL: Tail pointer of new list is not NULL.\n");
		assert(listArr[i]->size == 0 
			&& "FAIL: Size of new list was non-zero.\n");
		assert(listArr[i]->currentIsBeyond == 0 
			&& "FAIL: The new list indicated the current pointer was outside the list.\n");
	}

	/* Test Case 2 */
	/* Attempt to create a list when list space is exhausted */
	listArr[10] = ListCreate();
	assert(listArr[10] == NULL
		&& "FAIL: A pointer was returned even though list space should be full.\n");

	/* Cleanup */
	for (int i = 0; i < 10; i++) {
		ListFree(listArr[i], NULL);
	}
	printf("| ListCreate  |       2      |       61     |  PASS  |\n");
}

/**
 * 1. Create a list with 100 items (also tests ListAdd()).
 * 2. Delete all items and check count.
 * 3. Pass a NULL list.
 */
static void ListCountTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	int listItems[100];
	for (int i = 0; i < 100; i++) {
		listItems[i] = i;
		ListAdd(list, (void *)&listItems[i]);
		assert(ListCount(list) == i + 1
			&& "FAIL: List count did not reflect the actual list size\n");
	}

	/* Test Case 2 */
	for (int i = 100; i > 0; i--) {
		ListTrim(list);
		assert(ListCount(list) == i - 1
			&& "FAIL: List count did not reflect the actual list size\n");
	}

	/* Test Case 3 */
	assert(ListCount(NULL) == 0
		&& "FAIL: List count for a NULL list returned non-zero\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListCount   |       3      |      201     |  PASS  |\n");
}

/**
 * 1. Check first item of empty list returns NULL.
 * 2. Check first item of list with one item.
 * 3. Check first item of list with multiple items.
 * 4. Check first item of NULL list.
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

	/* Test Case 4 */
	assert(ListFirst(NULL) == NULL
		&& "FAIL: Checking first item of NULL list returned non-NULL\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListFirst   |       4      |        7     |  PASS  |\n");
}

/**
 * 1. Check last item of empty list returns NULL.
 * 2. Check last item of list with one item.
 * 3. Check last item of list with multiple items.
 * 4. Check last item of NULL list returns NULL.
 */
static void ListLastTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	assert(ListLast(list) == NULL
		&& "FAIL: Checking last item of an empty list did not return NULL\n");
	assert(list->current == NULL
		&& "FAIL: Checking last item modified the current item of an empty list\n");

	/* Test Case 2 */
	char testChar[3];
	testChar[0] = 'a';
	ListAdd(list, (void *)&testChar[0]);
	assert(ListLast(list) == &testChar[0]
		&& "FAIL: Checking last item of a list with 1 item\n");
	assert(list->current->item == &testChar[0]
		&& "FAIL: The current item was not set to the last item for a list with 1 item\n");

	/* Test Case 3 */
	testChar[1] = 'b';
	ListAdd(list, (void *)&testChar[1]);
	assert(ListLast(list) == &testChar[1]
		&& "FAIL: Checking first item of a list with multiple items\n");
	assert(list->current->item == &testChar[1]
		&& "FAIL: The current item was not set to the first item for a list with multiple items\n");

	/* Test Case 4 */
	assert(ListLast(NULL) == NULL
		&& "FAIL: Checking last item of NULL list returned non-NULL\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListLast    |       4      |        7     |  PASS  |\n");
}

/**
 * 1. Check next item of empty list returns NULL.
 * 2. Check next item of list with one item when current item is first.
 * 3. Check next item of list with one item when current item is beyond the end of the list.
 * 4. Check next item of list with one item when current item is beyond the start of the list.
 * 5. Check next item of list with multiple items when current item is beyond the start.
 * 6. Check next item of list with multiple items when current item is first.
 * 7. Check next item of list with multiple items when current item is in the middle.
 * 8. Check next item of list with multiple items when current item is the last.
 * 9. Check next item of list with multiple items when current item is beyond the end.
 * 10. Check next item of NULL list returns NULL.
 */
static void ListNextTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	assert(ListNext(list) == NULL
		&& "FAIL: Checking next item of empty list returned non-NULL\n");

	/* Test Case 2 */
	int testInt[10];
	testInt[0] = 0;
	ListAdd(list, (void *)&testInt[0]);
	assert(ListNext(list) == NULL
		&& "FAIL: Checking next item of list with one item returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == 1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 3 */
	assert(ListNext(list) == NULL
		&& "FAIL: Checking next item of list with one item when current item is beyond the end returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == 1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 4 */
	ListFirst(list);
	ListPrev(list);
	assert(ListNext(list) == &testInt[0]
		&& "FAIL: Checking next item of list with one item when current item is beyond the start returned the wrong value\n");
	assert(list->current->item == &testInt[0]
		&& "FAIL: The current item was not set to the next item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 5 */
	testInt[1] = 1;
	testInt[2] = 2;
	ListAdd(list, (void *)&testInt[1]);
	ListAdd(list, (void *)&testInt[2]);
	ListFirst(list);
	ListPrev(list);
	assert(ListNext(list) == &testInt[0]
		&& "FAIL: Checking next item of list with multiple items when current item is beyond the start returned the wrong value\n");
	assert(list->current->item == &testInt[0]
		&& "FAIL: The current item was not set to the next item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 6 */
	assert(ListNext(list) == &testInt[1]
		&& "FAIL: Checking next item of list with multiple items when current item is the first returned the wrong value\n");
	assert(list->current->item == &testInt[1]
		&& "FAIL: The current item was not set to the next item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 7 */
	assert(ListNext(list) == &testInt[2]
		&& "FAIL: Checking next item of list with multiple items when current item is in the middle returned the wrong value\n");
	assert(list->current->item == &testInt[2]
		&& "FAIL: The current item was not set to the next item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 8 */
	assert(ListNext(list) == NULL
		&& "FAIL: Checking next item of list with multiple items when current item is the last returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == 1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 9 */
	assert(ListNext(list) == NULL
		&& "FAIL: Checking next item of list with multiple items when current item is beyond the end returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == 1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 10 */
	assert(ListNext(NULL) == NULL
		&& "FAIL: Checking next item of a NULL list returned non-NULL");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListNext    |      10      |       26     |  PASS  |\n");
}

/**
 * 1. Check prev item of empty list returns NULL.
 * 2. Check prev item of list with one item when current item is first.
 * 3. Check prev item of list with one item when current item is beyond the start of the list.
 * 4. Check prev item of list with one item when current item is beyond the end of the list.
 * 5. Check prev item of list with multiple items when current item is beyond the end.
 * 6. Check prev item of list with multiple items when current item is the last.
 * 7. Check prev item of list with multiple items when current item is in the middle.
 * 8. Check prev item of list with multiple items when current item is the first.
 * 9. Check prev item of list with multiple items when current item is beyond the start.
 * 10. Check prev item of NULL list returns NULL.
 */
static void ListPrevTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	assert(ListPrev(list) == NULL
		&& "FAIL: Checking prev item of empty list returned non-NULL\n");

	/* Test Case 2 */
	int testInt[10];
	testInt[0] = 0;
	ListAdd(list, (void *)&testInt[0]);
	assert(ListPrev(list) == NULL
		&& "FAIL: Checking prev item of list with one item returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == -1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 3 */
	assert(ListPrev(list) == NULL
		&& "FAIL: Checking prev item of list with one item when current item is beyond the start returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == -1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 4 */
	ListLast(list);
	ListNext(list);
	assert(ListPrev(list) == &testInt[0]
		&& "FAIL: Checking prev item of list with one item when current item is beyond the end returned the wrong value\n");
	assert(list->current->item == &testInt[0]
		&& "FAIL: The current item was not set to the prev item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 5 */
	testInt[1] = 1;
	testInt[2] = 2;
	ListAdd(list, (void *)&testInt[1]);
	ListAdd(list, (void *)&testInt[2]);
	ListNext(list);
	assert(ListPrev(list) == &testInt[2]
		&& "FAIL: Checking prev item of list with multiple items when current item is beyond the end returned the wrong value\n");
	assert(list->current->item == &testInt[2]
		&& "FAIL: The current item was not set to the prev item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 6 */
	assert(ListPrev(list) == &testInt[1]
		&& "FAIL: Checking prev item of list with multiple items when current item is the last returned the wrong value\n");
	assert(list->current->item == &testInt[1]
		&& "FAIL: The current item was not set to the prev item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 7 */
	assert(ListPrev(list) == &testInt[0]
		&& "FAIL: Checking prev item of list with multiple items when current item is in the middle returned the wrong value\n");
	assert(list->current->item == &testInt[0]
		&& "FAIL: The current item was not set to the prev item\n");
	assert(list->currentIsBeyond == 0
		&& "FAIL: The list does not recognise that the current pointer is no longer beyond the list bounds\n");

	/* Test Case 8 */
	assert(ListPrev(list) == NULL
		&& "FAIL: Checking prev item of list with multiple items when current item is the first returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == -1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 9 */
	assert(ListPrev(list) == NULL
		&& "FAIL: Checking prev item of list with multiple items when current item is beyond the start returned non-NULL\n");
	assert(list->current == NULL
		&& "FAIL: The current item was not set to NULL\n");
	assert(list->currentIsBeyond == -1
		&& "FAIL: The list does not recognise that the current pointer is beyond the list bounds\n");

	/* Test Case 10 */
	assert(ListPrev(NULL) == NULL
		&& "FAIL: Checking next item of a NULL list returned non-NULL");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListPrev    |      10      |       26     |  PASS  |\n");
}

/**
 * 1. Check current item of empty list.
 * 2. Check current item of list with one item when current item is first.
 * 3. Check current item of list with one item when current item is beyond the end of the list.
 * 4. Check current item of list with one item when current item is beyond the start of the list.
 * 5. Check current item of list with multiple items when current item is beyond the start.
 * 6. Check current item of list with multiple items when current item is first.
 * 7. Check current item of list with multiple items when current item is in the middle.
 * 8. Check current item of list with multiple items when current item is the last.
 * 9. Check current item of list with multiple items when current item is beyond the end.
 * 10. Check current item of NULL list returns NULL.
 */
static void ListCurrTest() {
	LIST *list = ListCreate();

	/* Test Case 1 */
	assert(ListCurr(list) == NULL
		&& "FAIL: Checking current item of empty list returned non-NULL\n");

	/* Test Case 2 */
	int testInt[10];
	testInt[0] = 0;
	ListAdd(list, (void *)&testInt[0]);
	assert(ListCurr(list) == &testInt[0]
		&& "FAIL: Checking current item of list with one item returned wrong value\n");

	/* Test Case 3 */
	ListNext(list);
	assert(ListCurr(list) == NULL
		&& "FAIL: Checking current item of list with one item when current item is beyond the end returned non-NULL\n");

	/* Test Case 4 */
	ListFirst(list);
	ListPrev(list);
	assert(ListCurr(list) == NULL
		&& "FAIL: Checking current item of list with one item when current item is beyond the start returned non-NULL\n");

	/* Test Case 5 */
	testInt[1] = 1;
	testInt[2] = 2;
	ListFirst(list);
	ListAdd(list, (void *)&testInt[1]);
	ListAdd(list, (void *)&testInt[2]);
	ListFirst(list);
	ListPrev(list);
	assert(ListCurr(list) == NULL
		&& "FAIL: Checking current item of list with multiple items when current item is beyond the start returned non-NULL\n");

	/* Test Case 6 */
	ListFirst(list);
	assert(ListCurr(list) == &testInt[0]
		&& "FAIL: Checking current item of list with multiple items when current item is the first returned the wrong value\n");

	/* Test Case 7 */
	ListNext(list);
	assert(ListCurr(list) == &testInt[1]
		&& "FAIL: Checking current item of list with multiple items when current item is in the middle returned the wrong value\n");

	/* Test Case 8 */
	ListNext(list);
	assert(ListCurr(list) == &testInt[2]
		&& "FAIL: Checking current item of list with multiple items when current item is the last returned non-NULL\n");

	/* Test Case 9 */
	ListNext(list);
	assert(ListCurr(list) == NULL
		&& "FAIL: Checking current item of list with multiple items when current item is beyond the end returned non-NULL\n");

	/* Test Case 10 */
	assert(ListPrev(NULL) == NULL
		&& "FAIL: Checking current item of a NULL list returned non-NULL\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListCurr    |      10      |       10     |  PASS  |\n");
}

/**
 * (Normal functionality tested in other tests)
 * 1. Add null item
 * 2. Add item to null list
 * 3. Add item after node pool full
 * 4. Add item to empty list, check list params
 * 5. Add item to list with one item, check list params
 * 6. Add item to list with multiple items, check list params
 * 7. Add 5 items, check all items linked correctly
 */
static void ListAddTest() {
	LIST *list = ListCreate();
	float testFloat[100] = {1.0};

	/* Test Case 1 */
	assert(ListAdd(list, NULL) == -1
		&& "FAIL: Adding NULL item did not return -1\n");

	/* Test Case 2 */
	assert(ListAdd(NULL, &testFloat[0]) == -1
		&& "FAIL: Adding item to NULL list did not return -1\n");

	/* Test Case 3 */
	for (int i = 0; i < 100; i++) {
		ListAdd(list, &testFloat[i]);
	}
	assert(ListAdd(NULL, &testFloat[0]) == -1
		&& "FAIL: Adding item after node pool full did not return -1\n");
	ListFree(list, NULL);
	list = ListCreate();

	/* Test Case 4 */
	ListAdd(list, &testFloat[0]);
	assert(ListCurr(list) == &testFloat[0]
		&& "FAIL: The list's current item is not the added item\n");
	assert(list->size == 1
		&& "FAIL: The list's size is wrong for a list with a single item\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with a single item\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a single item\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's added item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's added item's previous pointer is wrong\n");

	/* Test Case 5 */
	ListAdd(list, &testFloat[1]);
	assert(ListCurr(list) == &testFloat[1]
		&& "FAIL: The list's current item is not the added item\n");
	assert(list->size == 2
		&& "FAIL: The list's size is wrong for a list with two items\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with two items\n");
	assert(list->tail->item == &testFloat[1]
		&& "FAIL: The list's tail is wrong for a list with a two items\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's added item's next pointer is wrong\n");
	assert(list->current->previous->item == &testFloat[0]
		&& "FAIL: The list's added item's previous pointer is wrong\n");
	assert(list->head->next->item == &testFloat[1]
		&& "FAIL: The list head's next pointer is wrong\n");

	/* Test Case 6 */
	ListAdd(list, &testFloat[2]);
	assert(ListCurr(list) == &testFloat[2]
		&& "FAIL: The list's current item is not the added item\n");
	assert(list->size == 3
		&& "FAIL: The list's size is wrong for a list with three items\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with three items\n");
	assert(list->tail->item == &testFloat[2]
		&& "FAIL: The list's tail is wrong for a list with a three items\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's added item's next pointer is wrong\n");
	assert(list->current->previous->item == &testFloat[1]
		&& "FAIL: The list's added item's previous pointer is wrong\n");
	assert(list->tail->previous->next->item == &testFloat[2]
		&& "FAIL: The list item before the added item did not update its next pointer\n");

	/* Test Case 7 */
	ListFree(list, NULL);
	list = ListCreate();
	for (int i = 0; i < 5; i++) ListAdd(list, &testFloat[i]);
	NODE *node = list->head;
	for (int i = 0; i < 5; i++) {
		assert(node->item == &testFloat[i]
			&& "FAIL: The list was not linked correctly");
		node = node->next;
	}
	node = list->tail;
	for (int i = 5; i > 0; i--) {
		assert(node->item == &testFloat[i - 1]
			&& "FAIL: The list was not linked correctly");
		node = node->previous;
	}

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListAdd     |       6      |       23     |  PASS  |\n");
}

/**
 * 1. Insert null item
 * 2. Insert item to null list
 * 3. Insert item after node pool full
 * 4. Insert item to empty list, check list params
 * 5. Insert item to list with one item, check list params
 * 6. Insert item to list with multiple items, check list params
 */
static void ListInsertTest() {
	LIST *list = ListCreate();
	float testFloat[100] = {1.0};

	/* Test Case 1 */
	assert(ListInsert(list, NULL) == -1
		&& "FAIL: Inserting NULL item did not return -1\n");

	/* Test Case 2 */
	assert(ListInsert(NULL, &testFloat[0]) == -1
		&& "FAIL: Inserting item to NULL list did not return -1\n");

	/* Test Case 3 */
	for (int i = 0; i < 100; i++) {
		ListInsert(list, &testFloat[i]);
	}
	assert(ListInsert(NULL, &testFloat[0]) == -1
		&& "FAIL: Inserting item after node pool full did not return -1\n");
	ListFree(list, NULL);
	list = ListCreate();

	/* Test Case 4 */
	ListInsert(list, &testFloat[0]);
	assert(ListCurr(list) == &testFloat[0]
		&& "FAIL: The list's current item is not the inserted item\n");
	assert(list->size == 1
		&& "FAIL: The list's size is wrong for a list with a single item\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with a single item\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a single item\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's inserted item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's inserted item's previous pointer is wrong\n");

	/* Test Case 5 */
	ListInsert(list, &testFloat[1]);
	assert(ListCurr(list) == &testFloat[1]
		&& "FAIL: The list's current item is not the inserted item\n");
	assert(list->size == 2
		&& "FAIL: The list's size is wrong for a list with two items\n");
	assert(list->head->item == &testFloat[1]
		&& "FAIL: The list's head is wrong for a list with two items\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a two items\n");
	assert(list->current->next->item == &testFloat[0]
		&& "FAIL: The list's inserted item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's inserted item's previous pointer is wrong\n");
	assert(list->tail->previous->item == &testFloat[1]
		&& "FAIL: The list tail's previous pointer is wrong\n");

	/* Test Case 6 */
	ListInsert(list, &testFloat[2]);
	assert(ListCurr(list) == &testFloat[2]
		&& "FAIL: The list's current item is not the inserted item\n");
	assert(list->size == 3
		&& "FAIL: The list's size is wrong for a list with three items\n");
	assert(list->head->item == &testFloat[2]
		&& "FAIL: The list's head is wrong for a list with three items\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a three items\n");
	assert(list->current->next->item == &testFloat[1]
		&& "FAIL: The list's inserted item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's inserted item's previous pointer is wrong\n");
	assert(list->current->next->previous->item == &testFloat[2]
		&& "FAIL: The list item after the inserted item did not update its previous pointer\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListInsert  |       6      |       23     |  PASS  |\n");
}

/**
 * 1. Append null item
 * 2. Append item to null list
 * 3. Append item after node pool full
 * 4. Append item to empty list, check list params
 * 5. Append item to list with one item, check list params
 * 6. Append item to list with multiple items, check list params
 */
static void ListAppendTest() {
	LIST *list = ListCreate();
	float testFloat[100] = {1.0};

	/* Test Case 1 */
	assert(ListAppend(list, NULL) == -1
		&& "FAIL: Appending NULL item did not return -1\n");

	/* Test Case 2 */
	assert(ListAppend(NULL, &testFloat[0]) == -1
		&& "FAIL: Appending item to NULL list did not return -1\n");

	/* Test Case 3 */
	for (int i = 0; i < 100; i++) {
		ListAppend(list, &testFloat[i]);
	}
	assert(ListAppend(NULL, &testFloat[0]) == -1
		&& "FAIL: Appending item after node pool full did not return -1\n");
	ListFree(list, NULL);
	list = ListCreate();

	/* Test Case 4 */
	ListAppend(list, &testFloat[0]);
	assert(ListCurr(list) == &testFloat[0]
		&& "FAIL: The list's current item is not the appended item\n");
	assert(list->size == 1
		&& "FAIL: The list's size is wrong for a list with a single item\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with a single item\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a single item\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's appended item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's appended item's previous pointer is wrong\n");

	/* Test Case 5 */
	ListAppend(list, &testFloat[1]);
	assert(ListCurr(list) == &testFloat[1]
		&& "FAIL: The list's current item is not the appended item\n");
	assert(list->size == 2
		&& "FAIL: The list's size is wrong for a list with two items\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with two items\n");
	assert(list->tail->item == &testFloat[1]
		&& "FAIL: The list's tail is wrong for a list with a two items\n");
	assert(list->current->previous->item == &testFloat[0]
		&& "FAIL: The list's appended item's previous pointer is wrong\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's appended item's next pointer is wrong\n");
	assert(list->head->next->item == &testFloat[1]
		&& "FAIL: The list head's next pointer is wrong\n");

	/* Test Case 6 */
	ListAppend(list, &testFloat[2]);
	assert(ListCurr(list) == &testFloat[2]
		&& "FAIL: The list's current item is not the appended item\n");
	assert(list->size == 3
		&& "FAIL: The list's size is wrong for a list with three items\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with three items\n");
	assert(list->tail->item == &testFloat[2]
		&& "FAIL: The list's tail is wrong for a list with a three items\n");
	assert(list->current->previous->item == &testFloat[1]
		&& "FAIL: The list's appended item's previous pointer is wrong\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's appended item's next pointer is wrong\n");
	assert(list->current->previous->next->item == &testFloat[2]
		&& "FAIL: The list item before the appended item did not update its previous pointer\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListAppend  |       6      |       23     |  PASS  |\n");
}

/**
 * 1. Prepend null item
 * 2. Prepend item to null list
 * 3. Prepend item after node pool full
 * 4. Prepend item to empty list, check list params
 * 5. Prepend item to list with one item, check list params
 * 6. Prepend item to list with multiple items, check list params
 */
static void ListPrependTest() {
	LIST *list = ListCreate();
	float testFloat[100] = {1.0};

	/* Test Case 1 */
	assert(ListPrepend(list, NULL) == -1
		&& "FAIL: Prepending NULL item did not return -1\n");

	/* Test Case 2 */
	assert(ListPrepend(NULL, &testFloat[0]) == -1
		&& "FAIL: Prepending item to NULL list did not return -1\n");

	/* Test Case 3 */
	for (int i = 0; i < 100; i++) {
		ListPrepend(list, &testFloat[i]);
	}
	assert(ListPrepend(NULL, &testFloat[0]) == -1
		&& "FAIL: Prepending item after node pool full did not return -1\n");
	ListFree(list, NULL);
	list = ListCreate();

	/* Test Case 4 */
	ListPrepend(list, &testFloat[0]);
	assert(ListCurr(list) == &testFloat[0]
		&& "FAIL: The list's current item is not the prepended item\n");
	assert(list->size == 1
		&& "FAIL: The list's size is wrong for a list with a single item\n");
	assert(list->head->item == &testFloat[0]
		&& "FAIL: The list's head is wrong for a list with a single item\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a single item\n");
	assert(list->current->next == NULL
		&& "FAIL: The list's prepended item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's prepended item's previous pointer is wrong\n");

	/* Test Case 5 */
	ListPrepend(list, &testFloat[1]);
	assert(ListCurr(list) == &testFloat[1]
		&& "FAIL: The list's current item is not the prepended item\n");
	assert(list->size == 2
		&& "FAIL: The list's size is wrong for a list with two items\n");
	assert(list->head->item == &testFloat[1]
		&& "FAIL: The list's head is wrong for a list with two items\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a two items\n");
	assert(list->current->next->item == &testFloat[0]
		&& "FAIL: The list's prepended item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's prepended item's previous pointer is wrong\n");
	assert(list->tail->previous->item == &testFloat[1]
		&& "FAIL: The list tail's previous pointer is wrong\n");

	/* Test Case 6 */
	ListPrepend(list, &testFloat[2]);
	assert(ListCurr(list) == &testFloat[2]
		&& "FAIL: The list's current item is not the prepended item\n");
	assert(list->size == 3
		&& "FAIL: The list's size is wrong for a list with three items\n");
	assert(list->head->item == &testFloat[2]
		&& "FAIL: The list's head is wrong for a list with three items\n");
	assert(list->tail->item == &testFloat[0]
		&& "FAIL: The list's tail is wrong for a list with a three items\n");
	assert(list->current->next->item == &testFloat[1]
		&& "FAIL: The list's prepended item's next pointer is wrong\n");
	assert(list->current->previous == NULL
		&& "FAIL: The list's prepended item's previous pointer is wrong\n");
	assert(list->current->next->previous->item == &testFloat[2]
		&& "FAIL: The list item after the prepended item did not update its previous pointer\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListPrepend |       6      |       23     |  PASS  |\n");
}

/**
 * 1. Remove item from NULL list
 * 2. Remove item from list with 1 item - current item beyond start
 * 3. Remove item from list with 1 item - current item beyond end
 * 4. Remove item from list with 1 item - current item is head
 * 5. Remove item from list with 2 items - current item beyond start
 * 6. Remove item from list with 2 items - current item beyond end
 * 7. Remove item from list with 2 items - current item is head
 * 8. Remove item from list with 2 items - current item is tail
 * 9. Remove item from list with 3 items - current item is beyond start
 * 10. Remove item from list with 3 items - current item is beyond end
 * 11. Remove item from list with 3 items - current item is head
 * 12. Remove item from list with 3 items - current item is middle
 * 13. Remove item from list with 3 items - current item is tail
 */
static void ListRemoveTest() {
	LIST *list = ListCreate();
	int testInt[10] = {0};

	/* Test Case 1 */
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing NULL item returned non-NULL\n");

	/* Test Case 2 */
	ListAdd(list, &testInt[0]);
	ListPrev(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the start of the list returned non-NULL\n");

	/* Test Case 3 */
	ListLast(list);
	ListNext(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the end of the list returned non-NULL\n");

	/* Test Case 4 */
	ListFirst(list);
	assert(ListRemove(list) == &testInt[0]
		&& "FAIL: Removed item's value was not correct");
	assert(list->size == 0
		&& "FAIL: List's size should be 0 after removing item");
	assert(list->current == NULL
		&& "FAIL: List's current pointer should be NULL after removing item");
	assert(list->head == NULL
		&& "FAIL: List's head pointer should be NULL after removing item");
	assert(list->tail == NULL
		&& "FAIL: List's tail pointer should be NULL after removing item");

	/* Test Case 5 */
	ListAdd(list, &testInt[0]);
	ListAdd(list, &testInt[1]);
	ListFirst(list);
	ListPrev(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the start of the list returned non-NULL\n");

	/* Test Case 6 */
	ListLast(list);
	ListNext(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the end of the list returned non-NULL\n");

	/* Test Case 7 */
	ListFirst(list);
	assert(ListRemove(list) == &testInt[0]
		&& "FAIL: Removed item's value was not correct\n");
	assert(list->size == 1
		&& "FAIL: List's size should be 1 after removing item\n");
	assert(list->current->item == &testInt[1]
		&& "FAIL: List's current pointer should be set to the item after the removed item\n");
	assert(list->head->item == &testInt[1]
		&& "FAIL: List's head pointer should be set to the item after the removed item\n");
	assert(list->tail->item == &testInt[1]
		&& "FAIL: List's tail pointer should be set to the item after the removed item\n");

	/* Test Case 8 */
	ListPrepend(list, &testInt[0]);
	ListLast(list);
	assert(ListRemove(list) == &testInt[1]
		&& "FAIL: Removed item's value was not correct\n");
	assert(list->size == 1
		&& "FAIL: List's size should be 1 after removing item\n");
	assert(list->current->item == &testInt[0]
		&& "FAIL: List's current pointer should be set to the item before the removed item\n");
	assert(list->head->item == &testInt[0]
		&& "FAIL: List's head pointer should be set to the item before the removed item\n");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: List's tail pointer should be set to the item before the removed item\n");

	/* Test Case 9 */
	ListAppend(list, &testInt[1]);
	ListAppend(list, &testInt[2]);
	ListFirst(list);
	ListPrev(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the start of the list returned non-NULL\n");

	/* Test Case 10 */
	ListLast(list);
	ListNext(list);
	assert(ListRemove(list) == NULL
		&& "FAIL: Removing item when current is beyond the end of the list returned non-NULL\n");

	/* Test Case 11 */
	ListFirst(list);
	assert(ListRemove(list) == &testInt[0]
		&& "FAIL: Removed item's value was not correct\n");
	assert(list->size == 2
		&& "FAIL: List's size should be 2 after removing item\n");
	assert(list->current->item == &testInt[1]
		&& "FAIL: List's current pointer should be set to the item after the removed item\n");
	assert(list->head->item == &testInt[1]
		&& "FAIL: List's head pointer should be set to the item after the removed item\n");
	assert(list->tail->item == &testInt[2]
		&& "FAIL: List's tail pointer should be set to the last item\n");
	assert(list->head->next->item == &testInt[2]
		&& "FAIL: List head's next pointer should be set to the tail\n");
	assert(list->tail->previous->item == &testInt[1]
		&& "FAIL: List tail's previous pointer should be set to the item after the removed item\n");

	/* Test Case 12 */
	ListPrepend(list, &testInt[0]);
	ListNext(list);
	assert(ListRemove(list) == &testInt[1]
		&& "FAIL: Removed item's value was not correct\n");
	assert(list->size == 2
		&& "FAIL: List's size should be 2 after removing item\n");
	assert(list->current->item == &testInt[2]
		&& "FAIL: List's current pointer should be set to the item after the removed item\n");
	assert(list->head->item == &testInt[0]
		&& "FAIL: List's head pointer should be set to the item before the removed item\n");
	assert(list->tail->item == &testInt[2]
		&& "FAIL: List's tail pointer should be set to the item after the removed item\n");
	assert(list->head->next->item == &testInt[2]
		&& "FAIL: List head's next pointer should be set to the item after the removed item\n");
	assert(list->tail->previous->item == &testInt[0]
		&& "FAIL: List tail's previous pointer should be set to the item before the removed item\n");

	/* Test Case 13 */
	ListFirst(list);
	ListAdd(list, &testInt[1]);
	ListNext(list);
	assert(ListRemove(list) == &testInt[2]
		&& "FAIL: Removed item's value was not correct\n");
	assert(list->size == 2
		&& "FAIL: List's size should be 2 after removing item\n");
	assert(list->current->item == &testInt[1]
		&& "FAIL: List's current pointer should be set to the item before the removed item\n");
	assert(list->head->item == &testInt[0]
		&& "FAIL: List's head pointer should be set to the first item\n");
	assert(list->tail->item == &testInt[1]
		&& "FAIL: List's tail pointer should be set to the item before the removed item\n");
	assert(list->head->next->item == &testInt[1]
		&& "FAIL: List head's next pointer should be set to the item after the removed item\n");
	assert(list->tail->previous->item == &testInt[0]
		&& "FAIL: List tail's previous pointer should be set to the item before the removed item\n");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListRemove  |      13      |       43     |  PASS  |\n");
}

/**
 * 1. List1 NULL
 * 2. List2 NULL
 * 3. List1 empty, List2 empty
 * 4. List1 empty, List2 1 item
 * 5. List1 empty, List2 2 items
 * 6. List1 empty, List2 3 items
 * 7. List1 1 item, List2 empty
 * 8. List1 1 item, List2 1 item
 * 9. List1 1 item, List2 2 items
 * 10. List1 1 item, List2 3 items
 * 11. List1 2 items, List2 empty
 * 12. List1 2 items, List2 1 item
 * 13. List1 2 items, List2 2 items
 * 14. List1 2 items, List2 3 items
 * 15. List1 3 items, List2 empty
 * 16. List1 3 items, List2 1 item
 * 17. List1 3 items, List2 2 items
 * 18. List1 3 items, List2 3 items
 */
static void ListConcatTest() {
	LIST *list1 = NULL;
	LIST *list2 = ListCreate();

	/* Test Case 1 */
	LIST *listAddr = list2;
	ListConcat(list1, list2);
	assert(list1 == NULL
		&& "FAIL: List1 was modified even though it was NULL");
	assert(list2 == listAddr
		&& "FAIL: List2 was modified even though list1 was NULL");

	/* Test Case 2 */
	list1 = list2;
	list2 = NULL;
	ListConcat(list1, list2);
	assert(list1 == listAddr
		&& "FAIL: List1 was modified even though list2 was NULL");
	assert(list2 == NULL
		&& "FAIL: List2 was modified even though it was NULL");

	/* Test Case 3 */
	list2 = ListCreate();
	ListConcat(list1, list2);
	assert(list1->size == 0
		&& "FAIL: Size was non-zero when both lists were empty");
	assert(list1->current == NULL
		&& "FAIL: Current ptr was non-NULL when both lists were empty");
	assert(list1->head == NULL
		&& "FAIL: Head ptr was non-NULL when both lists were empty");
	assert(list1->tail == NULL
		&& "FAIL: Tail ptr was non-NULL when both lists were empty");

	/* Test Case 4 */
	int testInt2[5] = {0};
	ListAdd(list2, &testInt2[0]);
	ListConcat(list1, list2);
	assert(list1->size == 1
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt2[0]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt2[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[0]
		&& "FAIL: Tail ptr was wrong after concatenating lists");

	/* Test Case 5 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListConcat(list1, list2);
	assert(list1->size == 2
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt2[1]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt2[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[1]
		&& "FAIL: Tail ptr was wrong after concatenating lists");

	/* Test Case 6 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListAdd(list2, &testInt2[2]);
	ListConcat(list1, list2);
	assert(list1->size == 3
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt2[2]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt2[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[2]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	assert(list1->head->next->item == &testInt2[1]
		&& "FAIL: Head next ptr was wrong after concatenating lists");
	assert(list1->tail->previous->item == &testInt2[1]
		&& "FAIL: Tail prev ptr was wrong after concatenating lists");

	/* Test Case 7 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	int testInt1[5] = {0};
	ListAdd(list1, &testInt1[0]);
	ListConcat(list1, list2);
	assert(list1->size == 1
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[0]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt1[0]
		&& "FAIL: Tail ptr was wrong after concatenating lists");

	/* Test Case 8 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list2, &testInt2[0]);
	ListConcat(list1, list2);
	assert(list1->size == 2
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[0]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[0]
		&& "FAIL: Tail ptr was wrong after concatenating lists");

	/* Test Case 9 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListConcat(list1, list2);
	assert(list1->size == 3
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[0]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[1]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	NODE *testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i == 0) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-1]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 10 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListAdd(list2, &testInt2[2]);
	ListConcat(list1, list2);
	assert(list1->size == 4
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[0]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[2]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i == 0) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-1]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 11 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListConcat(list1, list2);
	assert(list1->size == 2
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[1]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt1[1]
		&& "FAIL: Tail ptr was wrong after concatenating lists");

	/* Test Case 12 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list2, &testInt2[0]);
	ListConcat(list1, list2);
	assert(list1->size == 3
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[1]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[0]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 1) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-2]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 13 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListConcat(list1, list2);
	assert(list1->size == 4
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[1]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[1]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 1) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-2]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 14 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListAdd(list2, &testInt2[2]);
	ListConcat(list1, list2);
	assert(list1->size == 5
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[1]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[2]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 1) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-2]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 15 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list1, &testInt1[2]);
	ListConcat(list1, list2);
	assert(list1->size == 3
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[2]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt1[2]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		assert(testNode->item == &testInt1[i]
			&& "FAIL: Nodes in incorrect order after concatenating lists");
		testNode = testNode->next;
	}

	/* Test Case 16 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list1, &testInt1[2]);
	ListAdd(list2, &testInt2[0]);
	ListConcat(list1, list2);
	assert(list1->size == 4
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[2]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[0]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 2) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-3]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 17 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list1, &testInt1[2]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListConcat(list1, list2);
	assert(list1->size == 5
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[2]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[1]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 2) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-3]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Test Case 18 */
	ListFree(list1, NULL);
	list1 = ListCreate();
	list2 = ListCreate();
	ListAdd(list1, &testInt1[0]);
	ListAdd(list1, &testInt1[1]);
	ListAdd(list1, &testInt1[2]);
	ListAdd(list2, &testInt2[0]);
	ListAdd(list2, &testInt2[1]);
	ListAdd(list2, &testInt2[2]);
	ListConcat(list1, list2);
	assert(list1->size == 6
		&& "FAIL: Size was wrong after concatenating lists");
	assert(list1->current->item == &testInt1[2]
		&& "FAIL: Current ptr was wrong after concatenating lists");
	assert(list1->head->item == &testInt1[0]
		&& "FAIL: Head ptr was wrong after concatenating lists");
	assert(list1->tail->item == &testInt2[2]
		&& "FAIL: Tail ptr was wrong after concatenating lists");
	testNode = list1->head;
	for (int i = 0; i < list1->size; i++) {
		if (i <= 2) {
			assert(testNode->item == &testInt1[i]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		} else {
			assert(testNode->item == &testInt2[i-3]
				&& "FAIL: Nodes in incorrect order after concatenating lists");
		}
		testNode = testNode->next;
	}

	/* Cleanup */
	ListFree(list1, NULL);
	ListFree(list2, NULL);
	printf("| ListFree    |      18      |      107     |  PASS  |\n");
}

/**
 * 1. Trim NULL list.
 * 2. Trim empty list.
 * 3. Trim list with one item.
 * 4. Trim list with two items.
 * 5. Trim list with three items.
 * 6. Trim list with four items.
 */
static void ListTrimTest() {
	LIST *list = NULL;

	/* Test Case 1 */
	assert(ListTrim(list) == NULL
		&& "FAIL: Trimming NULL list returned non-NULL");
	assert(list == NULL
		&& "FAIL: Trimming NULL list set it to non-NULL");

	/* Test Case 2 */
	list = ListCreate();
	assert(ListTrim(list) == NULL
		&& "FAIL: Trimming empty list returned non-NULL");
	assert(list->size == 0
		&& "FAIL: Trimming empty list returned non-zero size");
	assert(list->current == NULL
		&& "FAIL: Trimming empty list returned non-NULL current ptr");
	assert(list->head == NULL
		&& "FAIL: Trimming empty list returned non-NULL head ptr");
	assert(list->tail == NULL
		&& "FAIL: Trimming empty list returned non-NULL tail ptr");


	/* Test Case 3 */
	int testInt[5] = {0};
	ListAdd(list, &testInt[0]);
	assert(ListTrim(list) == &testInt[0]
		&& "FAIL: The list did not return the trimmed item");
	assert(list->size == 0
		&& "FAIL: Trimming list with one item returned non-zero size");
	assert(list->current == NULL
		&& "FAIL: Trimming list with one item returned non-NULL current ptr");
	assert(list->head == NULL
		&& "FAIL: Trimming list with one item returned non-NULL head ptr");
	assert(list->tail == NULL
		&& "FAIL: Trimming list with one item returned non-NULL tail ptr");

	/* Test Case 4 */
	ListAdd(list, &testInt[0]);
	ListAdd(list, &testInt[1]);
	assert(ListTrim(list) == &testInt[1]
		&& "FAIL: The list did not return the trimmed item");
	assert(list->size == 1
		&& "FAIL: Trimming list with two items returned wrong size");
	assert(list->current->item == &testInt[0]
		&& "FAIL: Trimming list with two items returned wrong current ptr");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Trimming list with two items returned wrong head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Trimming list with two items returned wrong tail ptr");

	/* Test Case 5 */
	ListAdd(list, &testInt[1]);
	ListAdd(list, &testInt[2]);
	assert(ListTrim(list) == &testInt[2]
		&& "FAIL: The list did not return the trimmed item");
	assert(list->size == 2
		&& "FAIL: Trimming list with 3 items returned wrong size");
	assert(list->current->item == &testInt[1]
		&& "FAIL: Trimming list with 3 items returned wrong current ptr");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Trimming list with 3 items returned wrong head ptr");
	assert(list->tail->item == &testInt[1]
		&& "FAIL: Trimming list with 3 items returned wrong tail ptr");

	/* Test Case 6 */
	ListFree(list, NULL);
	list = ListCreate();
	ListAdd(list, &testInt[0]);
	ListAdd(list, &testInt[1]);
	ListAdd(list, &testInt[2]);
	ListAdd(list, &testInt[3]);
	assert(ListTrim(list) == &testInt[3]
		&& "FAIL: The list did not return the trimmed item");
	assert(list->size == 3
		&& "FAIL: Trimming list with 4 items returned wrong size");
	assert(list->current->item == &testInt[2]
		&& "FAIL: Trimming list with 4 items returned wrong current ptr");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Trimming list with 4 items returned wrong head ptr");
	assert(list->tail->item == &testInt[2]
		&& "FAIL: Trimming list with 4 items returned wrong tail ptr");

	/* Cleanup */
	ListFree(list, NULL);
	printf("| ListTrim    |       6      |       27     |  PASS  |\n");
}

/** 
 * 1. Search NULL list
 * 2. Search with NULL comparator
 * 3. Search empty list
 * 4. Search list of 1, current = head, success
 * 5. Search list of 1, current = head, fail
 * 6. Search list of 1, current before head, success
 * 7. Search list of 1, current before head, fail
 * 8. Search list of 1, current after head
 * 9. Search list of multiple, current before head, success
 * 10. Search list of multiple, current before head, fail
 * 11. Search list of multiple, current is head, success
 * 12. Search list of multiple, current is head, fail
 * 13. Search list of multiple, current is middle, success 
 * 14. Search list of multiple, current is middle, fail
 * 15. Search list of multiple, current is tail, success
 * 16. Search list of multiple, current is tail, fail
 * 17. Search list of multiple, current is beyond tail, fail
 */
static void ListSearchTest() {
	LIST *list = NULL;

	/* Test Case 1 */
	assert(ListSearch(list, successComparator, NULL) == NULL
		&& "FAIL: Searching NULL list returned non-NULL");

	/* Test Case 2 */
	list = ListCreate();
	assert(ListSearch(list, NULL, NULL) == NULL
		&& "FAIL: Searching with NULL comparator returned non-NULL");

	/* Test Case 3 */
	assert(ListSearch(list, successComparator, NULL) == NULL
		&& "FAIL: Searching empty list returned non-NULL");
	assert(list->size == 0
		&& "FAIL: Searching empty list modified list size");
	assert(list->head == NULL
		&& "FAIL: Searching empty list modified head ptr");
	assert(list->tail == NULL
		&& "FAIL: Searching empty list modified tail ptr");
	assert(list->current == NULL
		&& "FAIL: Searching empty list modified current ptr");

	/* Test Case 4 */
	int testInt[3] = {0};
	ListAdd(list, &testInt[0]);
	assert(ListSearch(list, successComparator, NULL) == &testInt[0]
		&& "FAIL: Searching list returned wrong value");
	assert(list->size == 1
		&& "FAIL: Searching list modified list size");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Searching list modified tail ptr");
	assert(list->current->item == &testInt[0]
		&& "FAIL: Searching list did not set current ptr correctly");

	/* Test Case 5 */
	assert(ListSearch(list, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list->size == 1
		&& "FAIL: Searching list modified list size");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Searching list modified tail ptr");
	assert(list->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 6 */
	ListFirst(list);
	ListPrev(list);
	assert(ListSearch(list, successComparator, NULL) == &testInt[0]
		&& "FAIL: Searching list returned wrong value");
	assert(list->size == 1
		&& "FAIL: Searching list modified list size");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Searching list modified tail ptr");
	assert(list->current->item == &testInt[0]
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list->currentIsBeyond == 0
		&& "FAIL: List current ptr recognised as beyond the end");

	/* Test Case 7 */
	ListFirst(list);
	ListPrev(list);
	assert(ListSearch(list, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list->size == 1
		&& "FAIL: Searching list modified list size");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Searching list modified tail ptr");
	assert(list->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 8 */
	ListLast(list);
	ListNext(list);
	assert(ListSearch(list, successComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list->size == 1
		&& "FAIL: Searching list modified list size");
	assert(list->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list->tail->item == &testInt[0]
		&& "FAIL: Searching list modified tail ptr");
	assert(list->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 9 */
	LIST *list2 = ListCreate();
	ListAdd(list2, &testInt[0]);
	ListAdd(list2, &testInt[1]);
	ListAdd(list2, &testInt[2]);
	ListFirst(list2);
	ListPrev(list2);
	assert(ListSearch(list2, successComparator, NULL) == &testInt[0]
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current->item == &testInt[0]
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 0
		&& "FAIL: List current ptr recognised as beyond the end");

	/* Test Case 10 */
	ListFirst(list2);
	ListPrev(list2);
	assert(ListSearch(list2, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 11 */
	ListFirst(list2);
	assert(ListSearch(list2, successComparator, NULL) == &testInt[0]
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current->item == &testInt[0]
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 0
		&& "FAIL: List current ptr recognised as beyond the end");

	/* Test Case 12 */
	ListFirst(list2);
	assert(ListSearch(list2, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 13 */
	ListFirst(list2);
	ListNext(list2);
	assert(ListSearch(list2, successComparator, NULL) == &testInt[1]
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current->item == &testInt[1]
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 0
		&& "FAIL: List current ptr recognised as beyond the end");

	/* Test Case 14 */
	ListFirst(list2);
	ListNext(list2);
	assert(ListSearch(list2, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 15 */
	ListLast(list2);
	assert(ListSearch(list2, successComparator, NULL) == &testInt[2]
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current->item == &testInt[2]
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 0
		&& "FAIL: List current ptr recognised as beyond the end");

	/* Test Case 16 */
	ListLast(list2);
	assert(ListSearch(list2, failComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Test Case 17 */
	ListLast(list2);
	ListNext(list2);
	assert(ListSearch(list2, successComparator, NULL) == NULL
		&& "FAIL: Searching list returned wrong value");
	assert(list2->size == 3
		&& "FAIL: Searching list modified list size");
	assert(list2->head->item == &testInt[0]
		&& "FAIL: Searching list modified head ptr");
	assert(list2->tail->item == &testInt[2]
		&& "FAIL: Searching list modified tail ptr");
	assert(list2->current == NULL
		&& "FAIL: Searching list did not set current ptr correctly");
	assert(list2->currentIsBeyond == 1
		&& "FAIL: List current ptr not recognised as beyond the end");

	/* Cleanup */
	ListFree(list, NULL);
	ListFree(list2, NULL);
	printf("| ListSearch  |      17      |       90     |  PASS  |\n");
}

/***************************************************************
 * Globals                                                     *
 ***************************************************************/

int successComparator(void *item1, void *item2) {
	/* Suppress unused variable warnings */
	item1 = (int *)item1;
	item2 = (int *)item2;
	return 1;
}
int failComparator(void *item1, void *item2) {
	/* Suppress unused variable warnings */
	item1 = (int *)item1;
	item2 = (int *)item2;
	return 0;
}