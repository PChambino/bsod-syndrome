
#include "queue.h"

#include <dos.h>
#include <pc.h>

#include "ints.h"

void queueInit(Queue *q) {
    q->in = q->out = q->cnt = 0;
    q->size = 10;
}

// called from isr, interrupts are disabled,
// can't call C library I/O functions, as they are BIOS based, not reentrant
Bool queuePut(Queue *q, char c) {
	void beep(void);
	
    if (queueFull(q)) {
		//beep(); // warning, uses T0 ints!
		//extern Bool beepFlag;
		//beepFlag = true;
        return false;
	}
    
    q->buf[q->in++] = c;
    q->in %= q->size;
    q->cnt++;
    
    return true;
}

// called from C. As it can be interrupted by the isr and manipulates structure fields it is a critical section
int queueGet(Queue *q) {	
	disable_irq(KBD_IRQ); // critical section, disable kbd interrupts
    if (queueEmpty(q)) {
		enable(); // reenable interrupts
        return -1;
	}
    
    char c = q->buf[q->out++];
    q->out %= q->size;
    q->cnt--;
	enable_irq(KBD_IRQ); // end of critical section, reenable interrupts
    return c;
}

// should be critical a section
Bool queueEmpty(Queue *q) {
    return q->cnt == 0;
}

// should be critical a section
Bool queueFull(Queue *q) {
    return q->cnt == q->size;
}
