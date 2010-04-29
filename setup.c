
#include "setup.h"

static int mode = 0x103;
int fps = 100;

static void printUsage() {
	printf("Usage: BSoD [options] \n"
		   "Options: \n"
		   " -mode <0x101 | 0x103 | 0x105 | 0x107> Where 0x101 = 640x480 \n"
		   "                                             0x103 = 800x600 (Default) \n"
		   "                                             0x105 = 1024x768 \n"
		   "                                             0x107 = 1280x1024 \n"
		   " -fps <n>                              Where <n> is a positive number. \n"
		   "                                       Default is 100. \n"
		   " -help                                 Prints this info and exits. \n"
		   "\n");
}

void process_args(int argc, char **argv) {
	int error = 0;
	int i = 0;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i],"-help") == 0) {
			printUsage();
			exit(0);
		}
		else if (strcmp(argv[i],"-mode") == 0) {
			i++;
			if (i >= argc) { error = 1; break; }
			if (sscanf(argv[i],"%x",&mode) != 1) { error = 1; break; }
			switch (mode) {
				case 0x101:
				case 0x103:
				case 0x105:
				case 0x107:
					break;
				default:
					error = 1;
					break;
			}
			if (error) break;
		}
		else if (strcmp(argv[i],"-fps") == 0) {
			i++;
			if (i >= argc) {	error = 1; break; }
			if (sscanf(argv[i],"%d",&fps) != 1) { error = 1; break; }
			if (fps <= 0 || fps > 1000) { error = 1; break; }
		}
		else {
			error = 1; break;
		}
	}
	
	if (error) {
		printUsage();
		exit(1);
	}
}

static _go32_dpmi_seginfo old_t0_isr, old_kbd_isr, old_rtc_isr, old_mouse_isr;

void setup_timer() {
	timer_load(TIMER_0, TIMER_CLK / 1000);
	
	void t0_isr();
	install_asm_irq_handler(T0_IRQ, t0_isr, &old_t0_isr);
	enable_irq(T0_IRQ);
}

void setup_rtc() {
	void rtc_isr();
	install_c_irq_handler(RTC_IRQ, rtc_isr, &old_rtc_isr);
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) | RTC_PIE);
	enable_irq(RTC_IRQ);
}

Queue keys;
GQueue *mouseQueue;

void setup_kbc() {	
	queueInit(&keys);
	mouseQueue = newGQueue(3, 3 * sizeof(uchar));
	
	disable_irq(KBD_IRQ);
	disable_irq(MOUSE_IRQ);  

	if (kbc_init(0) != 1) {
		fprintf(stderr, "Rato Nao Encontrado\n");
		enable_irq(KBD_IRQ);
		enable_irq(MOUSE_IRQ);	
		//exit(2);
	}
	
	void mouse_isr();
	install_asm_irq_handler(MOUSE_IRQ, mouse_isr, &old_mouse_isr); 
	
	void mouse_isr_end();
	_go32_dpmi_lock_code(mouse_isr, ((unsigned int) mouse_isr_end) - ((unsigned int) mouse_isr));

	void kbd_isr();
	install_asm_irq_handler(KBD_IRQ, kbd_isr, &old_kbd_isr);
	
	enable_irq(KBD_IRQ);
	//enable_irq(MOUSE_IRQ); 
}

char *base;

void setup_video() {
	int i = 0;
	for (i = 0; i < 3; i++)
		if ((base = enter_graphics(mode)) != NULL)
			break;
			
	if (base == NULL) {
		fprintf(stderr, "Modo Grafico Nao Suportado!\n");
		exit(2);
	}
}

void tear_down() {
	leave_graphics();

	speaker_off();
	disable_irq(RTC_IRQ);
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) & ~(RTC_PIE | RTC_AIE | RTC_UIE));
	reinstall_c_irq_handler(RTC_IRQ, &old_rtc_isr);

	reinstall_asm_irq_handler(KBD_IRQ, &old_kbd_isr);	
	reinstall_asm_irq_handler(T0_IRQ, &old_t0_isr);
	reinstall_asm_irq_handler(MOUSE_IRQ, &old_mouse_isr);
	deleteGQueue(mouseQueue);
}
