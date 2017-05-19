/**
 * Implementation of a custom list data structure
 */

/**
 * Imports
 */
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Functions
 */

/** 
 * Creates a new list and returns a pointer to it.
 * TODO: Add failure handling
 */
LIST *ListCreate(void) {
	printf("Creating a new empty list...\n\n");
	LIST *list = malloc(sizeof(LIST));
	if (list == NULL) {
		return NULL;
	}
	list->current = 0;
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
	if (list->size != 0) {
		printf("Setting the currrent item to index 0\n");
		list->current = 0;
		printf("Returning the first item, it has address %p\n\n", list->items[list->current]);
		return list->items[list->current]; 
	} else {
		printf("The list is empty and has no first item, returning NULL\n\n");
		return NULL;
	}
}

/**
 * Returns the last item in the list and makes it the last item.
 * Returns NULL if the list is empty.
 */
void *ListLast(LIST *list) {
	if (list->size != 0) {
		printf("The size of the list is %d, setting the current item to index %d\n", list->size, list->size - 1);
		list->current = list->size - 1;
		printf("Returning the last item, it has address %p\n\n", list->items[list->current]);
		return list->items[list->current];
	} else {
		printf("The list is empty and has no last item, returning NULL\n\n");
		return NULL;
	}
}

/**
 * Increments the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the end of the list.
 */
void *ListNext(LIST *list) {
	printf("Incrementing the current item from index %d to %d\n", list->current, list->current + 1);
	(list->current)++;
	if (list->current < list->size) {
		printf("The address of the current item is now %p\n\n", list->items[list->current]);
		return list->items[list->current];
	} else {
		printf("The index of the current item has gone beyond the list, returning NULL\n\n");
		return NULL;
	}
}

/**
 * Decrements the current item.
 * Returns a pointer to the new current item.
 * Returns NULL if the current item advances beyond the start of the list.
 */
void *ListPrev(LIST *list) {
	(list->current)--;
	if (list->current >= 0) {
		return list->items[list->current];
	} else {
		return NULL;
	}
}

/**
 * Returns a pointer to the current item in the list
 */
void *ListCurr(LIST *list) {
	return list->items[list->current];
}

/**
 * Adds the new item to the list directly after the current item and makes it the current item.
 * If the current pointer is before the start of the list, the item is added to the start.
 * If the current pointer is after the end of the list, the item is added to the end.
 * TODO: Add failure handling. Returns 0 if successful, -1 if failed.
 */
int ListAdd(LIST *list, void *item) {
	if (list->current < 0 || list->size == 0) {
		list->current = 0;
	} else if (list->current >= LIST_SIZE) {
		list->current = LIST_SIZE - 1;
	} else {
		(list->current)++;
	}
	list->items[list->current] = item;
	list->size++;
	return 0;
}

/**
 * Adds the new item to the list directly before the current item and makes it the current item.
 * If the current pointer is before the start of the list, the item is added to the start.
 * If the current pointer is after the end of the list, the item is added to the end.
 * TODO: Add failure handling. Returns 0 if successful, -1 if failed.
 */
int ListInsert(LIST *list, void *item) {
	if (list->current < 0) {
		list->current = 0;
	} else if (list->current >= LIST_SIZE) {
		list->current = LIST_SIZE - 1;
	} else {
		(list->current)--;
	}
	list->items[list->current] = item;
	list->size++;
	return 0;
}

/**
 * Adds item to the end of the list and makes the new item the current one.
 * Returns 0 if successful, -1 if failed.
 */
int ListAppend(LIST *list, void *item) {
	if (list->size == LIST_SIZE) {
		return -1;
	}
	list->items[list->size] = item;
	list->current = list->size;
	list->size++;
	return 0;
}

/**
 * Adds item to the front of list, and makes the new item the current one.
 *  Returns 0 on success, -1 on failure.
 */
int ListPrepend(LIST *list, void *item) {
	if (list->size == LIST_SIZE) {
		return -1;
	}
	if (list->size > 0) {
		for (int i = list->size; i > 0; i--) {
			list->items[i] = list->items[i-1];
		}
	}
	list->items[0] = item;
	list->current = 0;
	list->size++;
	return 0;
}

/**
 * Return current item and take it out of the list.
 * Make the next item the current one. (Which is next?)
 */
void *ListRemove(LIST *list) {
	if (list->current < 0 || list->size == 0 || list->current >= LIST_SIZE) {
		return NULL;
	}
	if (list->size == 1) {
		void *item = list->items[0];
		list->items[0] = NULL;
		list->size--;
		return item;
	}
	void *item = list->items[list->current];
	for (int i = list->current; i < list->size - 2; i++) {
		list->items[i] = list->items[i+1];
	}
	list->items[list->size - 1] = NULL;
	list->size--;
	return item;
}

/**
 * Adds list2 to the end of list1. 
 * The current pointer is set to the current pointer of list1. 
 * List2 no longer exists after the operation.
 */
void ListConcat(LIST *list1, LIST *list2) {
	for (int i = 0; i < list2->size && list1->size < LIST_SIZE; i++) {
		list1->items[list1->size - 1] = list2->items[i];
		list1->size++;
	}
	free(list2);
}

/** 
 * Delete list. 
 * ItemFree is a pointer to a routine that frees an item. 
 * It should be invoked (within ListFree) as: (* itemFree)(itemToBeFreed);
 */
void ListFree(LIST *list, void (*itemFree)(void *)) {
	for (int i = 0; i < list->size; i++) {
		(* itemFree)(list->items[i]);
	}
	free(list);
}

/**
 * Return last item and take it out of list. 
 * Make the new last item the current one.
 */
void *ListTrim(LIST *list) {
	if (list->size == 0) {
		return NULL;
	}
	void *item = list->items[list->size - 1];
	list->items[list->size - 1] = NULL;
	list->size--;
	list->current = list->size - 1;
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
	for (int i = list->current; i < list->size; i++) {
		if ((* comparator)(list->items[i], comparisonArg) == 1) {
			list->current = i;
			return list->items[i];
		}
	}
	list->current = list->size;
	return NULL;
}