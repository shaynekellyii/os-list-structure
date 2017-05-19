/***************************************************************
 * Implementation of a custom list data structure              *
 * Author: Shayne Kelly II                                     *
 * Date: May 19, 2017                                          *
 ***************************************************************/

/***************************************************************
 * Imports                                                     *
 ***************************************************************/
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/***************************************************************
 * Statics                                                     *
 ***************************************************************/
static NODE nodePool[NODE_POOL_SIZE];
static int listHeads[LIST_HEAD_SIZE];
static int nodesUsed = 0;
static int headsUsed = 0;

/***************************************************************
 * Global Functions                                            *
 ***************************************************************/

/** 
 * Creates a new list and returns a pointer to it.
 */
LIST *ListCreate(void) {
	printf("Creating a new empty list...\n\n");

	if (nodesUsed == NODE_POOL_SIZE || headsUsed == LIST_HEAD_SIZE) {
		printf("There is no space for the list... returning NULL\n\n");
		return NULL;
	}

	LIST *list = malloc(sizeof(LIST));
	if (list == NULL) {
		return NULL;
	}

	list->headIndex = headsUsed;
	listHeads[list->headIndex] = NULL;
	headsUsed++;

	list->currentIndex = -1;
	list->size = 0;
	return list;
}

/** 
 * Returns the number of items in the list
 */
int ListCount(LIST *list) {
	printf("The number of items in the list is %d\n\n", list->size);
	return list->size;
}

/** 
 * Returns a pointer to the first item in the list and makes it the current item.
 * Returns NULL if the list is empty.
 */
void *ListFirst(LIST *list) {
	if (list->size == 0) {
		printf("The list is empty and has no first item. Returning NULL\n\n");	
		return NULL;
	}

	printf("Setting the current item to the head\n");
	list->currentIndex = listHeads[list->headIndex];
	printf("Returning the first item, it has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Returns the last item in the list and makes it the last item.
 * Returns NULL if the list is empty.
 */
void *ListLast(LIST *list) {
	if (list->size == 0) {
		printf("The list is empty and has no last item, returning NULL\n\n");
		return NULL;
	}

	int headNodeIndex = listHeads[list->headIndex];
	NODE currentNode = nodePool[headNodeIndex];
	while (currentNode.nextIndex != NULL) {
		list->currentIndex = currentNode.nextIndex;
		currentNode = nodePool[currentNode.nextIndex];
	}

	printf("Returning the last item, it has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex)
}

/**
 * Increments the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the end of the list.
 */
void *ListNext(LIST *list) {
	if (list->currentIndex == NODE_POOL_SIZE || nodePool[list->currentIndex].nextIndex == NULL) {
		printf("The current item is now beyond the end of the list, returning NULL\n\n");
		list->currentIndex == NODE_POOL_SIZE;
		return list->currentIndex;
	}

	list->currentIndex = nodePool[list->currentIndex].nextIndex;
	printf("Incremented current item, the new item has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Decrements the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the start of the list.
 */
void *ListPrev(LIST *list) {
	if (list->currentIndex == -1 || nodePool[list->currentIndex].prevIndex == NULL) {
		printf("The current item is now beyond the start of the list, returning NULL\n\n");
		list->currentIndex = -1;
		return list->currentIndex;
	}

	list->currentIndex = nodePool[list->currentIndex].prevIndex;
	printf("Decremented current item, the new item has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Returns a pointer to the current item in the list
 */
void *ListCurr(LIST *list) {
	if (list->currentIndex == -1 || list->currentIndex == NODE_POOL_SIZE) {
		printf("The current index is beyond the boundaries of the list, returning NULL\n\n");
		return NULL;
	}

	printf("Returning the current item, it has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Adds the new item to the list directly after the current item and makes it the current item.
 * If the current pointer is before the start of the list, the item is added to the start.
 * If the current pointer is after the end of the list, the item is added to the end.
 * TODO: Add failure handling. Returns 0 if successful, -1 if failed.
 */
int ListAdd(LIST *list, void *item) {
	// TODO......
}


/***************************************************************
 * Private Functions                                           *
 ***************************************************************/

/**
 * Given the index of the node in the pool, returns the pointer to the void data item at that node
 */
static void *getNodeItemFromPoolByIndex(int index) {
	return nodePool[index].item;
}