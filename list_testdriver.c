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
	ListLastTest();
	ListNextTest();
	ListPrevTest();
	ListCurrTest();
	ListAddTest();
	ListInsertTest();

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
	printf("ListCreate tests passed. 61 assertions passed.\n");
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
	printf("ListCount tests passed. 201 assertions passed.\n");
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
	printf("ListFirst tests passed. 7 assertions passed.\n");
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
	printf("ListLast tests passed. 7 assertions passed.\n");
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
	printf("ListNext tests passed. 26 assertions passed.\n");
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
	printf("ListPrev tests passed. 26 assertions passed.\n");
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
	printf("ListCurr tests passed. 10 assertions passed.\n");
}

/**
 * (Normal functionality tested in other tests)
 * 1. Add null item
 * 2. Add item to null list
 * 3. Add item after node pool full
 * 4. Add item to empty list, check list params
 * 5. Add item to list with one item, check list params
 * 6. Add item to list with multiple items, check list params
 * TODO: Add more cases if time (different current pointers).
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

	/* Cleanup */
	ListFree(list, NULL);
	printf("ListAdd tests passed. 23 assertions passed.\n");
}

/**
 * 1. Insert null item
 * 2. Insert item to null list
 * 3. Insert item after node pool full
 * 4. Insert item to empty list, check list params
 * 5. Insert item to list with one item, check list params
 * 6. Insert item to list with multiple items, check list params
 * TODO: Add more cases if time.
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
	printf("ListInsert tests passed. X assertions passed.\n");

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