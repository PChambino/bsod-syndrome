
#ifndef __QUEUE_H
#define __QUEUE_H

#include "utypes.h"

/** @defgroup Queue Queue
 * @{
 *
 * Queue related functions
 */
 
/** Queue definition
*/
typedef struct {
    char buf[10]; //< Buffer
    int in; //< input position
    int out; //< output position
    int cnt; //< count elements
    int size; //< size of queue
} Queue;

// typedef enum {false = 0, true = 1} Bool;

/** Inicializes Queue
*/
void queueInit(Queue *q);

/** Puts char to queue
*/
Bool queuePut(Queue *q, char c);

/** Gets char from queue
*/
int queueGet(Queue *q);

/** Checks if it's empty

	@return true if it's empty
*/
Bool queueEmpty(Queue *q);

/** Checks if it's full

	@return true if it's full
*/
Bool queueFull(Queue *q);

//@}

#endif /* __QUEUE_H */
