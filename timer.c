
#include "timer.h"

void timer_init(int timer, int mode) {
	outportb(TIMER_CTRL, ((0x03 & timer) << 6) | mode);
}

void timer_load(int timer, int value) {
	timer_init(timer, LCOM_MODE);
	outportb(timer, LSB(value));
	outportb(timer, MSB(value));
}

void mili_sleep(int mili) {
	int end = time_tick + mili;
	
	while (time_tick < end) { end = end; }
}
