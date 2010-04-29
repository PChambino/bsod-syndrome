#include "GQueue.h"

GQueue *newGQueue(int n_ele, int ele_size)
{
	GQueue *q = (GQueue *)malloc(sizeof(GQueue));
	
	q->ptr = malloc(n_ele * ele_size);
	q->in = 0;
	q->out = 0;
	q->count = 0;
	q->size = n_ele;
	q->ele_size = ele_size;
	
	return q;
}

void deleteGQueue(GQueue *q)
{
	free(q->ptr);
	free(q);
}

Bool putGQueue(GQueue *q, void *ele)
{
	if (isFullGQueue(q))
		return false;
	else {
		/* memcpy(void *destination, const void *source, size_t num) */
		memcpy(q->ptr + q->in, ele, q->ele_size);
		/* incrementa para a proxima posicao */
		q->in = q->in + q->ele_size;
		/* incrementar o número de elementos da queue */
		q->count++;
		//++q->count;
		
		if(q->in == q->size)
			q->in = 0;
		
		return true;
	}
}

void *getGQueue(GQueue *q)
{
	if (isEmptyGQueue(q))
		return NULL;
	else {
		void *ret = q->ptr + q->out;
		q->out = q->out + q->ele_size;
		/* decrementa o número de elementos da queue */
		q->count--;
		//--q->count;
		
		if(q->out == q->size)
			q->out = 0;

		return ret;
	}
}

Bool isEmptyGQueue(GQueue *q)
{
	if (q->count == 0)
		return true;
		
	return false;
}

Bool isFullGQueue(GQueue *q)
{
	if (q->count == q->size)
		return true;
		
	return false;
}
