#ifndef _LIST_H_
#define _LIST_H_

/**
 * Constants
 */
#define NODE_POOL_SIZE 100
#define LIST_HEAD_SIZE 10

/**
 * Structs
 */
typedef struct NODE {
	void *item;
	int prevIndex;
	int nextIndex;
} NODE;
typedef struct LIST {
	int headIndex; // Location in heads array
	int currentIndex; // Location of current node in node pool array
	int tailIndex; // Location of tail node in node pool array 
	int size;
} LIST;

/**
 * Function prototypes
 */
LIST *ListCreate(void);
int ListCount(LIST *list);
void *ListFirst(LIST *list);
void *ListLast(LIST *list);
void *ListNext(LIST *list);
void *ListPrev(LIST *list);
void *ListCurr(LIST *list);
int ListAdd(LIST *list, void *item);
int ListInsert(LIST *list, void *item);
int ListAppend(LIST *list, void *item);
int ListPrepend(LIST *list, void *item);
void *ListRemove(LIST *list);
void ListConcat(LIST *list1, LIST *list2);
void ListFree(LIST *list, void (*itemFree)(void *));
void *ListTrim(LIST *list);
void *ListSearch(LIST *list, int (*comparator)(void *, void *), void *comparisonArg);

#endif /* _LIST_H_ */