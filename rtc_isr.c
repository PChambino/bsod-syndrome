
#include "rtc.h"

void rtc_isr() {
	void music_state_machine();
	extern int mili_tick;
	
	Byte statC = read_rtcv(RTC_STAT_C);

	if ((statC & RTC_IRQF) != 0 && (statC & RTC_PF) != 0) {
		music_state_machine();
		
		mili_tick++;
	}
	
	outportb(PIC1_CMD, EOI);
	outportb(PIC2_CMD, EOI);
}
