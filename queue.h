
#ifndef __QUEUE_H
#define __QUEUE_H

#include "utypes.h"

typedef struct {
    char buf[10];
    int in;
    int out;
    int cnt;
    int size;
} Queue;

// typedef enum {false = 0, true = 1} Bool;

void queueInit(Queue *q);
Bool queuePut(Queue *q, char c);
int queueGet(Queue *q);
Bool queueEmpty(Queue *q);
Bool queueFull(Queue *q);

#endif /* __QUEUE_H */
