
#include "rtc.h"

Byte bcd2dec(Byte i) {
	return (((i >> 4) & 0x0F) * 10) + (i & 0x0F);
}

Byte dec2bcd(Byte i) {
	return (((i / 10) % 10) << 4) + (i % 10);
}

void rtc_valid() {
	disable();
	while (read_rtc(RTC_STAT_A) == 1) {}
	enable();
}

Byte read_rtc(Byte add) {
	disable();
	outportb(RTC_ADDR_REG, add);
	Byte res = inportb(RTC_DATA_REG);
	enable();
	return res;
}

Byte read_rtcv(Byte add) {
	rtc_valid();
	return read_rtc(add);
}

void write_rtc(Byte add, Byte value) {
	disable();
	outportb(RTC_ADDR_REG, add);
	outportb(RTC_DATA_REG, value);
	enable();
}

void write_rtcv(Byte add, Byte value) {
	rtc_valid();
	write_rtc(add, value);
}

void rtc_read_time(RTC_TIME *time) {
	time->sec = bcd2dec(read_rtcv(SEC));
	time->min = bcd2dec(read_rtcv(MIN));
	time->hour = bcd2dec(read_rtcv(HOUR));
}

void rtc_read_date(RTC_DATE *date) {
	date->day = bcd2dec(read_rtcv(MONTH_DAY));
	date->month = bcd2dec(read_rtcv(MONTH));
	date->year = bcd2dec(read_rtcv(YEAR));
}

void rtc_read_alarm(RTC_TIME *alarm) {
	alarm->sec = bcd2dec(read_rtcv(SEC_ALARM));
	alarm->min = bcd2dec(read_rtcv(MIN_ALARM));
	alarm->hour = bcd2dec(read_rtcv(HOUR_ALARM));	
}

void rtc_write_alarm(RTC_TIME *alarm) {
	write_rtcv(SEC_ALARM, dec2bcd(alarm->sec));
	write_rtcv(MIN_ALARM, dec2bcd(alarm->min));
	write_rtcv(HOUR_ALARM, dec2bcd(alarm->hour));
}
