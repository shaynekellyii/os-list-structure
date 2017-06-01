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
#define NODE_POOL_SIZE 100
#define LIST_POOL_SIZE 10

#define NODE_POOL_FULL				(numNodesAvailable <= 0)
#define LIST_POOL_FULL				(numListsAvailable <= 0)
#define LIST_IS_EMPTY				(list->size == 0)
#define CURRENT_NODE_BEYOND_START	(list->currentIsBeyond == -1)
#define CURRENT_NODE_BEYOND_END		(list->currentIsBeyond == 1)
#define CURRENT_NODE_IS_HEAD		(list->currentIndex == list->headIndex])
#define CURRENT_NODE_IS_TAIL		(list->currentIndex == list->tailIndex)

/***************************************************************
 * Statics                                                     *
 ***************************************************************/
static NODE nodePool[NODE_POOL_SIZE];
static LIST listPool[LIST_POOL_SIZE];
static int availableNodeArr[NODE_POOL_SIZE];
static int availableListArr[LIST_POOL_SIZE];
static int numNodesAvailable = NODE_POOL_SIZE;
static int numListsAvailable = LIST_POOL_SIZE;
static int initialisationFlag = 0;

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
	LIST list;

	/* Set all of the indices of the available nodes when the first list is created */
	if (!initialisationFlag) {
		for (int i = 0; i < NODE_POOL_SIZE; i++) {
			availableNodes[i] = i;
		}
		for (int i = 0; i < LIST_POOL_SIZE; i++) {
			availableLists[i] = i;
		}
		initialisationFlag = 1;
	}

	printf("Creating a new empty list...\n\n");

	if (NODE_POOL_FULL || LIST_POOL_FULL) {
		printf("There is no space for the list... returning NULL\n\n");
		return NULL;
	}

	list.current = NULL;
	list.head = NULL;
	list.tail = NULL;
	list.size = 0;
	list.currentIsBeyond = 0;

	int listIndex = availableListArr[numListsAvailable - 1];
	listPool[listIndex] = list;
	numListsAvailable--;

	return &listPool[listIndex];
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
	list->current = list->head;
	printf("Returning the first item, it has address %p\n\n", list->current->item;
	return list->current->item;
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
	list->current = list->tail;
	printf("Returning the last item, it has address %p\n\n", list->current->item);
	return list->current->item;
}

/**
 * Increments the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the end of the list.
 */
void *ListNext(LIST *list) {
	if (CURRENT_NODE_BEYOND_END || CURRENT_NODE_IS_TAIL) {
		printf("The current item is now beyond the end of the list, returning NULL\n\n");
		list->current = NULL;
		list->currentIsBeyond = 1;
		return NULL;
	}

	list->current = list->current->next;
	printf("Incremented current item, the new item has address %p\n\n", list->current->item);
	return list->current->item;
}

/**
 * Decrements the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the start of the list.
 */
void *ListPrev(LIST *list) {
	if (CURRENT_NODE_BEYOND_START || CURRENT_NODE_IS_HEAD) {
		printf("The current item is now beyond the start of the list, returning NULL\n\n");
		list->current = NULL;
		list->currentIsBeyond = -1;
		return NULL;
	}

	list->current = list->current->previous;
	printf("Decremented current item, the new item has address %p\n\n", list->current->item);
	return list->current->item;
}

/**
 * Returns a pointer to the current item in the list
 */
void *ListCurr(LIST *list) {
	if (CURRENT_NODE_BEYOND_START || CURRENT_NODE_BEYOND_END) {
		printf("The current index is beyond the boundaries of the list, returning NULL\n\n");
		return NULL;
	}

	printf("Returning the current item, it has address %p\n\n", list->current->item);
	return list->current->item;
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
		addItemBetweenTwoOthers(list, item, list->current, list->current->next;
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
		return 0;
	}

	printf("Adding item to the end of the list...\n\n");

	node.item = item;
	node.prev = list->tail;
	node.next = NULL;
	
	int nodeIndex = availableNodeArr[numNodesAvailable - 1]
	nodePool[nodeIndex] = node;
	numNodesAvailable--;

	list->tail->next = &nodePool[nodeIndex];
	list->tail = &nodePool[nodeIndex];
	list->size++;
	list->currentIsBeyond = 0;

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

	node.item = item;
	node.prev = NULL;
	node.next = list->head;

	int nodeIndex = availableNodeArr[numNodesAvailable - 1];
	nodePool[nodeIndex] = node;
	numNodesAvailable--;

	list->head->prev = &nodePool[nodeIndex];
	list->head = &nodePool[nodeIndex];
	list->size++;
	list->currentIsBeyond = 0;

	return 0;
}

/**
 * Return current item and take it out of the list.
 * Make the next item the current one.
 */
void *ListRemove(LIST *list) {
	if (LIST_IS_EMPTY || CURRENT_NODE_BEYOND_START || CURRENT_NODE_BEYOND_END) {
		return NULL;
	}

	void *item = list->current->item;
	ptrdiff_t nodeIndex = list->current - nodePool;

	if (list->size == 1) {
		printf("Removing the only item in the list...\n\n");
		list->head = NULL;
		list->current = NULL;
		list->tail = NULL;
	} else if (CURRENT_NODE_IS_HEAD) {
		printf("The current node is the head, removing it...\n\n")
		list->head = list->head->next;
		list->head->previous = NULL;
		list->current = list->head;
	} else if (CURRENT_NODE_IS_TAIL) {
		printf("The current node is the tail, removing it...\n\n");
		return ListTrim(list);
	} else {
		printf("Removing the current item...\n\n");
		NODE *removedNode = list->current;
		NODE *preRemovedNode = list->current->previous;
		NODE *postRemovedNode = list->current->next;
		preRemovedNode->next = postRemovedNode;
		postRemovedNode->previous = preRemovedNode;
	}

	list->currentIsBeyond = 0;
	list->size--;

	availableNodeArr[numNodesAvailable] = nodeIndex;
	numNodesAvailable++;
	return item;
}

/**
 * Adds list2 to the end of list1. 
 * The current pointer is set to the current pointer of list1. 
 * List2 no longer exists after the operation.
 */
void ListConcat(LIST *list1, LIST *list2) {
	list2->head->previous = list1->tail;
	list1->tail->next = list2->head;
	list1->size += list2->size;

	ptrdiff_t listIndex2 = list2 - listPool;
	availableListArr[numListsAvailable] = listIndex2;
	numListsAvailable++;
}

/** 
 * Delete list. 
 * ItemFree is a pointer to a routine that frees an item. 
 * It should be invoked (within ListFree) as: (* itemFree)(itemToBeFreed);
 */
void ListFree(LIST *list, void (*itemFree)(void *)) {
	NODE *nodeToDelete = list->head;
	while (nodeToDelete != NULL) {
		(* itemFree)(nodeToDelete->item);

		ptrdiff_t nodeIndex = nodeToDelete - nodePool;
		availableNodeArr[numNodesAvailable] = nodeIndex;
		numNodesAvailable++;

		nodeToDelete = nodeToDelete->next;
	}
}

/**
 * Return last item and take it out of list. 
 * Make the new last item the current one.
 */
void *ListTrim(LIST *list) {
	if (LIST_IS_EMPTY) {
		return NULL;
	}

	void *item = list->tail->item;
	ptrdiff_t nodeIndex = list->tail - nodePool;
	availableNodeArr[numNodesAvailable] = nodeIndex;
	numNodesAvailable++;

	NODE *newTail = list->tail->previous;
	newTail->next = NULL;
	list->tail = newTail;
	list->size--;
	list->current = list->tail;
	list->currentIsBeyond = 0;

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
	NODE *searchNode = list->current;

	while (searchNode != NULL) {
		if ((* comparator)(nodePool[currentIndex].item, comparisonArg) == 1) {
			list->current = searchNode;
			return list->current->item;
		}
	}

	list->current = NULL;
	list->currentIsBeyond = 1;
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
	node.previous = NULL;
	node.next = NULL;

	int nodeIndex = availableNodeArr[numNodesAvailable - 1];
	nodePool[nodeIndex] = node;
	numNodesAvailable--;

	list->current = &nodePool[nodeIndex];
	list->head = &nodePool[nodeIndex];
	list->tail = &nodePool[nodeIndex];
	list->size++;
}

/**
 * Add item between two index items of a non-empty list
 */
static void addItemBetweenTwoOthers(LIST *list, void *item, NODE *pre, NODE *post) {
	NODE node;
	node.item = item;
	node.prev = pre;
	node.next = post;

	int nodeIndex = availableNodeArr[numNodesAvailable - 1];
	nodePool[nodeIndex] = node;
	numNodesAvailable--;
	
	pre->next = &nodePool[nodeIndex];
	post->previous = &nodePool[nodeIndex];

	list->current = &nodePool[nodeIndex];
	list->size++;
	list->currentIsBeyond = 0;
}