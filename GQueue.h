#ifndef _GQUEUE_H_
#define _GQUEUE_H_

#include "utypes.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
	void *ptr;
	int in;
	int out;
	int count;
	int size;
	int ele_size;
} GQueue;

/**
* Criação de uma nova Queue Genérica 
*/
GQueue *newGQueue(int n_ele, int ele_size);

/**
* Liberta a memória
*/
void deleteGQueue(GQueue *q);

/** Put void 'ele' in the queue pointed to by 'q'
* Returns false if operation failed (the queue is full)
*/
Bool putGQueue(GQueue *q, void *ele);

/** Get next element from queue
*  Returns -1 (an int) if there are no elements in the queue
*/
void *getGQueue(GQueue *q);

/** Returns true if the queue is empty
*/
Bool isEmptyGQueue(GQueue *q);

/** Returns true if the queue if full
*/
Bool isFullGQueue(GQueue *q);

#endif /* _QUEUE_H_ */
