#include "gqueue.h"

GQueue* newGQueue(int n_ele, int ele_size)
{
	GQueue* q = (GQueue*) malloc(sizeof(GQueue));
	if(!q) return NULL;
	
	q->ptr = malloc(n_ele * ele_size);
	if(!(q->ptr))
	{
		free(q);
		return NULL;
	}
	
	q->in = q->out = q->count = 0;
	q->size = n_ele; q->ele_size = ele_size;
	q->being_accessed = false;

	return q;
}


void deleteGQueue(GQueue* q)
{
	free(q->ptr);
	free(q);
}


Bool putGQueue(GQueue* q, void* ele)
{
	if(q->being_accessed) return false;
	if (isFullGQueue(q)) return false;
	
	q->being_accessed = true;
	// memcpy(dest, src, n_bytes) - �memory copy�
	memcpy(q->ptr + q->in, ele, q->ele_size);
	
	q->count++;
	q->in += q->ele_size;
	if(q->in == q->size * q->ele_size) q->in = 0;

	q->being_accessed = false;
	return true;
}


void* peekGQueue(GQueue* q)
{
	if (isEmptyGQueue(q)) return NULL;
	return q->ptr + q->out;
}


void* getGQueue(GQueue* q)
{
	if(q->being_accessed) return NULL;
	if (isEmptyGQueue(q)) return NULL;

	q->being_accessed = true;
	void* pnt_data = q->ptr + q->out;
	
	q->count--;
	q->out += q->ele_size;
	if(q->out == q->size * q->ele_size) q->out = 0;

	q->being_accessed = false;
	return pnt_data;
}


void clearGQueue(GQueue* q)
{
	q->in = q->out = q->count = 0;
}


Bool isEmptyGQueue(GQueue* q)
{
	return q->count == 0;
}


Bool isFullGQueue(GQueue* q)
{
	return q->count == q->size;
}
