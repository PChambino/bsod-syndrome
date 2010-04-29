
#include "kbc.h"
#include <stdio.h>

int DEBUG_KBC = 0;

/*int kbc_init(int debug) {
	DEBUG_KBC = debug;

	while (read_kbc() != -1) {}
	
	write_kbc_cmd(EnableMouse);
	if (write_aux(MouseReset) != ACK) {
		DEBUG_KBC = 0;
		return 0;
	}
	
	mili_sleep(1000);
	if (read_kbc() != 0xAA || read_kbc() != 0x00) {
		DEBUG_KBC = 0;
		return 0;
	}
	
	write_aux(StreamDisable);
	
	write_aux(SampleRate);
	write_kbc_data(100);
	
	write_aux(Resolution);
	write_kbc_data(8);

	write_aux(Scaling1);
	
	int readCommandValue = 0;
	int writeCommandValue = 0;
	write_kbc_cmd(ReadCommand);
	if ((readCommandValue = read_kbc()) == 0xFF)
		writeCommandValue = ~(EN_KBD | EN_MOUSE) & (INT_1 | INT_12);
	else
		writeCommandValue = readCommandValue | INT_12;
	write_kbc_cmd(WriteCommand);
	write_kbc_data(writeCommandValue);

	write_aux(StreamEnable);
	
	write_kbc_data(KBDRate);
	write_kbc_data(0);
	
	DEBUG_KBC = 0;
	
	return 1;
}
*/
void mouse_disable(void) {
	write_aux(StreamDisable);
	
	write_kbc_cmd(ReadCommand);
	int value = read_kbc();
	write_kbc_cmd(WriteCommand);
	write_kbc_data(~(INT_12 | EN_MOUSE) & value);
	
	write_kbc_cmd(DisableMouse);
	disable_irq(MOUSE_IRQ);
}

int  write_kbc_cmd( unsigned data) {
	return write_kbc(CMD_REG, data);
}

int has_DOS_mouse_ISR() {
	return kbc_init(1);
}
  
int  write_kbc_data( unsigned data) {
	int i = 0;
	for (i = 0; i < 3; i++) {
		write_kbc(DATA_REG, data);
		int rsp = read_kbc();
		if (rsp != Resend)
			return rsp;
	}
	return -1;
}

int  write_aux(unsigned cmd) {
	int i = 0;
	for (i = 0; i < 3; i++) {
		write_kbc_cmd(WriteMouse);
		write_kbc(DATA_REG, cmd);
		int rsp = read_kbc();
		if (rsp != Resend)
			return rsp;
	}
	return -1;
}

int  read_kbc(void) {
	if (DEBUG_KBC == 1) printf("read : ");
	int time = 0;
	while (time < KBC_TIMEOUT) {
		int status = inportb(STAT_REG);
		if ((status & OBF) != 0) {
			if ((status & (TO_ERR | PAR_ERR)) != 0) {
				if (DEBUG_KBC == 1) printf("error\n");
				return -1;
			}
			int value = inportb(DATA_REG);
			if (DEBUG_KBC == 1) printf("data=%x\n", value);
			return value;
		}
		time++;
		mili_sleep(1);
	}
	if (DEBUG_KBC == 1) printf("timeout\n");
	return -1;
}

int  write_kbc(unsigned adr, unsigned data) {
	if (DEBUG_KBC == 1) printf("write : ");
	int time = 0;
	while (time < KBC_TIMEOUT) {
		int status = inportb(STAT_REG);
		if ((status & IBF) == 0) {
			if (DEBUG_KBC == 1) printf("addr=%x data=%x\n", adr, data);
			outportb(adr, data);
			return 0;
		}
		time++;
		mili_sleep(1);
	}
	if (DEBUG_KBC == 1) printf("timeout\n");
	return -1;
}

void blink_leds(void) {
	blink_led(NUM_LOCK | CAPS_LOCK | SCROLL_LOCK);
}

void blink_led(uchar led) {
	write_kbc_data(WriteLEDS);
	write_kbc_data(led);
	mili_sleep(400);
	write_kbc_data(WriteLEDS);
	write_kbc_data(0);
}

void parse_mouse_event(GQueue *mouseQueue, Mouse *mouse) {
	mouse->data = getGQueue(mouseQueue);
	
	if (mouse->data != NULL) {
		mouse->yov = Y_OVERFLOW(mouse->data[0]);
		mouse->xov = X_OVERFLOW(mouse->data[0]);
		mouse->ysig = Y_SIGN(mouse->data[0]);
		mouse->xsig = X_SIGN(mouse->data[0]);
		mouse->mb = MIDDLE_BUTTON(mouse->data[0]);
		mouse->lb = LEFT_BUTTON(mouse->data[0]);
		mouse->rb = RIGHT_BUTTON(mouse->data[0]);
		mouse->dx = mouse->data[1];
		mouse->dy = mouse->data[2];
	} else {
		mouse->dx = 0;
		mouse->dy = 0;
	}
}

void print_mouse_event(Mouse *mouse) {
	printf("%x %x %x LB=%d MB=%d RB=%d XOV=%d YOV=%d dX=%d dY=%d\n",
	mouse->data[0], mouse->data[1], mouse->data[2],
	mouse->lb, mouse->mb, mouse->rb, mouse->xov, mouse->yov, mouse->xsig*mouse->dx, mouse->ysig*mouse->dy);
}
