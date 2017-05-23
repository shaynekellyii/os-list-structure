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
 * Defines                                                     *
 ***************************************************************/
#define NODE_POOL_FULL				(nodesUsed >= NODE_POOL_SIZE)
#define HEAD_POOL_FULL				(headsUsed >= LIST_HEAD_SIZE)
#define LIST_IS_EMPTY				(list->size == 0)
#define CURRENT_NODE_BEYOND_START	(list->currentIndex == -1)
#define CURRENT_NODE_BEYOND_END		(list->currentIndex == NODE_POOL_SIZE)
#define CURRENT_NODE_IS_HEAD		(list->currentIndex == listHeads[list->headIndex])
#define CURRENT_NODE_IS_TAIL		(list->currentIndex == list->tailIndex)

/***************************************************************
 * Statics                                                     *
 ***************************************************************/
static NODE nodePool[NODE_POOL_SIZE];
static int listHeads[LIST_HEAD_SIZE];
static int nodesUsed = 0;
static int headsUsed = 0;

static void *getNodeItemFromPoolByIndex(int index);
static void addItemToEmptyList(LIST *list, void *item);
static void addItemBetweenTwoOthers(LIST *list, void *item, int pre, int post);

/***************************************************************
 * Global Functions                                            *
 ***************************************************************/

/** 
 * Creates a new list and returns a pointer to it.
 */
LIST *ListCreate(void) {
	printf("Creating a new empty list...\n\n");

	if (NODE_POOL_FULL || HEAD_POOL_FULL) {
		printf("There is no space for the list... returning NULL\n\n");
		return NULL;
	}

	LIST *list = malloc(sizeof(LIST));
	if (list == NULL) {
		return NULL;
	}

	list->headIndex = headsUsed;
	listHeads[list->headIndex] = 0;
	headsUsed++;

	list->currentIndex = -1;
	list->tailIndex = -1;
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
	if (LIST_IS_EMPTY) {
		printf("The list is empty and has no first item. Returning NULL\n\n");	
		return NULL;
	}

	printf("Setting the current item to the head\n");
	list->currentIndex = listHeads[list->headIndex];
	printf("Returning the first item, it has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Returns the last item in the list and makes it the current item.
 * Returns NULL if the list is empty.
 */
void *ListLast(LIST *list) {
	if (LIST_IS_EMPTY) {
		printf("The list is empty and has no last item, returning NULL\n\n");
		return NULL;
	}

	printf("Setting the current item to the tail\n");
	list->currentIndex = list->tailIndex;
	printf("Returning the last item, it has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Increments the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the end of the list.
 */
void *ListNext(LIST *list) {
	if (CURRENT_NODE_BEYOND_END || CURRENT_NODE_IS_TAIL) {
		printf("The current item is now beyond the end of the list, returning NULL\n\n");
		list->currentIndex = NODE_POOL_SIZE;
		return NULL;
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
	if (CURRENT_NODE_BEYOND_START || CURRENT_NODE_IS_HEAD) {
		printf("The current item is now beyond the start of the list, returning NULL\n\n");
		list->currentIndex = -1;
		return NULL;
	}

	list->currentIndex = nodePool[list->currentIndex].prevIndex;
	printf("Decremented current item, the new item has address %p\n\n", getNodeItemFromPoolByIndex(list->currentIndex));
	return getNodeItemFromPoolByIndex(list->currentIndex);
}

/**
 * Returns a pointer to the current item in the list
 */
void *ListCurr(LIST *list) {
	if (CURRENT_NODE_BEYOND_START || CURRENT_NODE_BEYOND_END) {
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
 * Returns 0 if successful, -1 if failed.
 */
int ListAdd(LIST *list, void *item) {
	if (NODE_POOL_FULL) {
		printf("The node pool is full, can't add a new item. Returning...\n\n");
		return -1;
	}

	if (LIST_IS_EMPTY) {
		printf("The list is empty, adding new item to the list...\n\n");
		addItemToEmptyList(list, item);
	} else if (CURRENT_NODE_BEYOND_START) {
		ListPrepend(list, item);
	} else if (CURRENT_NODE_BEYOND_END || CURRENT_NODE_IS_TAIL) {
		ListAppend(list, item);
	} else {
		printf("Adding item after the current item...\n\n");
		addItemBetweenTwoOthers(list, item, list->currentIndex, nodePool[list->currentIndex].nextIndex);
	}
	return 0;
}

/**
 * Adds the new item to the list directly before the current item and makes it the current item.
 * If the current pointer is before the start of the list, the item is added to the start.
 * If the current pointer is after the end of the list, the item is added to the end.
 * Returns 0 if successful, -1 if failed
 */
int ListInsert(LIST *list, void *item) {
	if (NODE_POOL_FULL) {
		printf("The node pool is full, can't add a new item. Returning...\n\n");
		return -1;
	}

	if (LIST_IS_EMPTY) {
		printf("The list is empty, adding new item to the list...\n\n");
		addItemToEmptyList(list, item);
	} else if (CURRENT_NODE_BEYOND_START || CURRENT_NODE_IS_HEAD) {
		ListPrepend(list, item);
	} else if (CURRENT_NODE_BEYOND_END) {
		ListAppend(list, item);
	} else {
		printf("Adding item after the current item...\n\n");
		addItemBetweenTwoOthers(list, item, nodePool[list->currentIndex].prevIndex, list->currentIndex);
	}
	return 0;
}

/**
 * Adds item to the end of the list and makes the new item the current one.
 * Returns 0 if successful, -1 if failed.
 */
int ListAppend(LIST *list, void *item) {
	NODE node;

	if (NODE_POOL_FULL) {
		printf("The node pool is full, can't add a new item. Returning...\n\n");
		return -1;
	}

	if (LIST_IS_EMPTY) {
		addItemToEmptyList(list, item);
	}

	printf("Adding item to the end of the list...\n\n");

	int oldTailIndex = list->tailIndex;
	NODE oldTail = nodePool[oldTailIndex];
	oldTail.nextIndex = nodesUsed;
	nodePool[oldTailIndex] = oldTail;

	node.item = item;
	node.prevIndex = oldTailIndex;
	node.nextIndex = -1;
	nodePool[nodesUsed] = node;

	list->tailIndex = nodesUsed;
	list->currentIndex = nodesUsed; 
	list->size++;
	nodesUsed++;

	return 0;
}

/**
 * Adds item to the front of list, and makes the new item the current one.
 * Returns 0 on success, -1 on failure.
 */
int ListPrepend(LIST *list, void *item) {
	NODE node;

	if (NODE_POOL_FULL) {
		printf("The node pool is full, can't add a new item. Returning...\n\n");
		return -1;
	}

	if (LIST_IS_EMPTY) {
		addItemToEmptyList(list, item);
	}

	printf("Adding item to the start of the list...\n\n");

	int oldHeadIndex = listHeads[list->headIndex];
	NODE oldHead = nodePool[oldHeadIndex];
	oldHead.prevIndex = nodesUsed;
	nodePool[oldHeadIndex] = oldHead;

	node.item = item;
	node.prevIndex = -1;
	node.nextIndex = listHeads[oldHeadIndex];
	nodePool[nodesUsed] = node;

	list->currentIndex = nodesUsed;
	listHeads[list->headIndex] = nodesUsed;
	list->size++;
	nodesUsed++;

	return 0;
}

/**
 * Return current item and take it out of the list.
 * Make the next item the current one. (Which is next?)
 */
void *ListRemove(LIST *list) {
	if (LIST_IS_EMPTY || CURRENT_NODE_BEYOND_START || CURRENT_NODE_BEYOND_END) {
		return NULL;
	}

	NODE node = nodePool[list->currentIndex];
	void *item = node.item;

	if (list->size == 1) {
		printf("Removing the only item in the list...\n\n");
		list->headIndex = -1;
		list->currentIndex = -1;
		list->tailIndex = -1;
	} else if (CURRENT_NODE_IS_HEAD) {
		printf("The current node is the head, removing it...\n\n")
		int newHeadIndex = node.nextIndex;
		NODE newHead = nodePool[newHeadIndex];
		newHead.prevIndex = -1;
		
		listHeads[list->headIndex] = newHeadIndex;
		list->currentIndex = newHeadIndex;
	} else if (CURRENT_NODE_IS_TAIL) {
		printf("The current node is the tail, removing it...\n\n");
		return ListTrim(list);
	} else {
		printf("Removing the current item...\n\n");
		int preNodeIndex = node.prevIndex;
		int postNodeIndex = node.nextIndex;
		NODE preNode = nodePool[preNodeIndex];
		NODE postNode = nodePool[postNodeIndex];

		preNode.nextIndex = postNodeIndex;
		postNode.prevIndex = preNodeIndex;

		nodePool[preNodeIndex] = preNode;
		nodePool[postNodeIndex] = postNode;
	}

	list->size--;
	return item;
}

/**
 * Adds list2 to the end of list1. 
 * The current pointer is set to the current pointer of list1. 
 * List2 no longer exists after the operation.
 */
void ListConcat(LIST *list1, LIST *list2) {
	int currentIndex2 = listHeads[list2->headIndex];
	while (currentIndex2 < NODE_POOL_SIZE) {
		NODE current2 = nodePool[currentIndex2]; 
		int currentTailIndex1 = list1->tailIndex;
		NODE currentTail1 = nodePool[currentTailIndex1];

		currentTail1.nextIndex = currentIndex2;
		current2.prevIndex = currentTailIndex1;

		list1.size++;
		list1.tailIndex = currentIndex2;
		currentIndex2 = current2.next;
	}
	free(list2);
}

/** 
 * Delete list. 
 * ItemFree is a pointer to a routine that frees an item. 
 * It should be invoked (within ListFree) as: (* itemFree)(itemToBeFreed);
 */
void ListFree(LIST *list, void (*itemFree)(void *)) {
	int currentIndex = listHeads[list->headIndex];
	for (int i = 0; i < list->size; i++) {
		(* itemFree)(nodePool[currentIndex].item);
		currentIndex = nodePool[currentIndex].nextIndex;
	}
	free(list);
}

/**
 * Return last item and take it out of list. 
 * Make the new last item the current one.
 */
void *ListTrim(LIST *list) {
	if (LIST_IS_EMPTY) {
		return NULL;
	}

	NODE node = nodePool[list->tailIndex];
	void *item = node.item;

	int newTailIndex = node.prevIndex;
	NODE newTail = nodePool[newTail];
	newTail.nextIndex = NODE_POOL_SIZE;

	list->tailIndex = newTailIndex;
	list->currentIndex = newTailIndex;
	list->size--;

	return item;
}

/**
 * Searches list starting at the current item until the end is reached or a match is found. 
 * In this context, a match is determined by the comparator parameter. 
 * This parameter is a pointer to a routine that takes as its first argument an item pointer, and as its second argument comparisonArg. 
 * Comparator returns 0 if the item and comparisonArg don't match, or 1 if they do. 
 * Exactly what constitutes a match is up to the implementor of comparator. 
 * If a match is found, the current pointer is left at the matched item and the pointer to that item is returned. 
 * If no match is found, the current pointer is left beyond the end of the list and a NULL pointer is returned.
 */
void *ListSearch(LIST *list, int (*comparator)(void *, void *), void *comparisonArg) {
	int currentIndex = listHeads[list->headIndex];
	for (int i = 0; i < list->size; i++) {
		if ((* comparator)(nodePool[currentIndex].item, comparisonArg) == 1) {
			list->current = currentIndex;
			return nodePool[currentIndex].item;
		}
		currentIndex = nodePool[currentIndex].nextIndex;
	}
	list->current = NODE_POOL_SIZE;
	return NULL;
}


/***************************************************************
 * Static Functions                                            *
 ***************************************************************/

/**
 * Given the index of the node in the pool, returns the pointer to the void data item at that node
 */
static void *getNodeItemFromPoolByIndex(int index) {
	return nodePool[index].item;
}

/**
 * Add item to empty list
 */
static void addItemToEmptyList(LIST *list, void *item) {
	NODE node;
	node.item = item;
	node.prevIndex = -1;
	node.nextIndex = -1;
	nodePool[nodesUsed] = node;

	list->currentIndex = nodesUsed;
	list->tailIndex = nodesUsed;
	listHeads[list->headIndex] = nodesUsed;
	list->size++;
	nodesUsed++;
}

/**
 * Add item between two index items of a non-empty list
 */
static void addItemBetweenTwoOthers(LIST *list, void *item, int pre, int post) {
	NODE node;
	node.item = item;
	node.prevIndex = pre;
	node.nextIndex = post;

	NODE preNode = nodePool[pre];
	NODE postNode = nodePool[post];
	
	preNode.nextIndex = nodesUsed;
	postNode.prevIndex = nodesUsed;

	nodePool[pre] = preNode;
	nodePool[post] = postNode;

	list->currentIndex = nodesUsed;
	list->size++;
	
	nodePool[nodesUsed] = node;
	nodesUsed++;
}