#ifndef _GQUEUE_H_
#define _GQUEUE_H_

#include <stdlib.h>
#include <string.h>

#include "utypes.h"

/** @defgroup GQueue GQueue
 * @{
 *
 * GQueue functions
 */
 
 /** GQueue definition
 */
typedef struct
{
	void *ptr;
	int in, out, count, size, ele_size;
	Bool being_accessed;
	
} GQueue;

/** New GQueue
*/
GQueue* newGQueue(int n_ele, int ele_size);

/** Delete GQueue
*/
void deleteGQueue(GQueue* q);

/** Put element to GQueue
	@return true if put did not gave errors, false otherwise
*/
Bool putGQueue(GQueue* q, void* ele);

/** Peek element GQueue
*/
void* peekGQueue(GQueue* q);

/** Get element GQueue
*/
void* getGQueue(GQueue* q);

/** Clear GQueue
*/
void clearGQueue(GQueue* q);

/** Checks if GQueue is empty
	@return true if empty, false otherwise
*/
Bool isEmptyGQueue(GQueue* q);

/** Checks if GQueue is full
	@return true if GQueue is full, false otherwise
*/
Bool isFullGQueue(GQueue* q);

/** @} end of GQueue */

#endif
