
#include "setup.h"

static int mode = 0x103;
int fps = 100;
int soundON = 1;

static void printUsage() {
	printf("Usage: BSoD [options] \n"
		   "Options: \n"
/*		   " -mode <0x101 | 0x103 | 0x105 | 0x107> Where 0x101 = 640x480 \n"
		   "                                             0x103 = 800x600 (Default) \n"
		   "                                             0x105 = 1024x768 \n"
		   "                                             0x107 = 1280x1024 \n"*/
		   " -fps <n>        Where <n> is a positive number. \n"
		   "                 Default is 100. \n"
		   " -nosound        Disables sound. \n"
		   " -help           Prints this info and exits. \n"
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
/*		else if (strcmp(argv[i],"-mode") == 0) {
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
		}*/
		else if (strcmp(argv[i],"-fps") == 0) {
			i++;
			if (i >= argc) {	error = 1; break; }
			if (sscanf(argv[i],"%d",&fps) != 1) { error = 1; break; }
			if (fps <= 0 || fps > 1000) { error = 1; break; }
		}
		else if (strcmp(argv[i],"-nosound") == 0) {
			soundON = 0;
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

FILE* logger;

void setup_log() {
	logger = fopen("log.txt", "a+");
//	logger = stderr;
	
	RTC_DATE date; rtc_read_date(&date);
	RTC_TIME time; rtc_read_time(&time);
	fprintf(logger, "%d-%d-%d %d:%d:%d\n", date.day, date.month, date.year, time.hour, time.min, time.sec);
}

char *base;

void setup_video() {
	fprintf(logger, "Setup Video Graphics\n");
	
	int i = 0;
	for (i = 0; i < 3; i++)
		if ((base = enter_graphics(mode)) != NULL)
			break;
			
	if (base == NULL) {
		fprintf(logger, "Modo Grafico Nao Suportado!\n");
		exit(2);
	}
}

static _go32_dpmi_seginfo old_t0_isr, old_kbd_isr, old_rtc_isr, old_mouse_isr;

Queue keys;
GQueue *mouseQueue;

static Bool hasMouse = false;

void setup_kbc() {
	fprintf(logger, "Setup KBC\n");
	
	queueInit(&keys);
	mouseQueue = newGQueue(10000, 3 * sizeof(uchar));
	
	disable_irq(KBD_IRQ);
	disable_irq(MOUSE_IRQ);  

	fprintf(logger, "KBC INIT\n");
	if ((hasMouse = kbc_init(0)) == false)
		fprintf(logger, "Rato Nao Encontrado\n");
	else
		fprintf(logger, "Rato Iniciado\n");

	void mouse_isr();
	if (hasMouse) {
		install_asm_irq_handler(MOUSE_IRQ, mouse_isr, &old_mouse_isr); 
	}	

	void kbd_isr();
	install_asm_irq_handler(KBD_IRQ, kbd_isr, &old_kbd_isr);

	if (hasMouse) {
		void mouse_isr_end();
		_go32_dpmi_lock_code(mouse_isr, ((unsigned int) mouse_isr_end) - ((unsigned int) mouse_isr));
	}

	enable_irq(KBD_IRQ);
	enable_irq(MOUSE_IRQ);
}

int mili_tick = 0;

void setup_rtc() {
	fprintf(logger, "Setup RTC\n");

	void rtc_isr();
	install_c_irq_handler(RTC_IRQ, rtc_isr, &old_rtc_isr);
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) | RTC_PIE);
	enable_irq(RTC_IRQ);
}

void setup_timer() {
	fprintf(logger, "Setup Timer 0\n");
	
	timer_load(TIMER_0, TIMER_CLK / 1000);
	
	void t0_isr();
	install_asm_irq_handler(T0_IRQ, t0_isr, &old_t0_isr);
	enable_irq(T0_IRQ);
}

void tear_down() {
	fprintf(logger, "Tear Down\n\n");
	
	// graphics
	leave_graphics();

	// kbd
	reinstall_asm_irq_handler(KBD_IRQ, &old_kbd_isr);	
		
	// mouse
	reinstall_asm_irq_handler(MOUSE_IRQ, &old_mouse_isr);
	deleteGQueue(mouseQueue);
	/*if(hasMouse)
		mouse_disable();*/

	// timer 0
	//reinstall_asm_irq_handler(T0_IRQ, &old_t0_isr);

	// rtc
	speaker_off();
	disable_irq(RTC_IRQ);
	write_rtcv(RTC_STAT_B, read_rtcv(RTC_STAT_B) & ~(RTC_PIE | RTC_AIE | RTC_UIE));
	reinstall_c_irq_handler(RTC_IRQ, &old_rtc_isr);
		
	fclose(logger);
}
