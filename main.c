
#include <dos.h>

#include "setup.h"
#include "game.h"

int main(int argc, char **argv) {
	disable();
	
	process_args(argc, argv);
	
	setup_timer();
	setup_rtc();
	setup_kbd();
	setup_mouse();
	setup_video();
	
	atexit(tear_down);
	
	game_loop(fps);
	
	return 0;
}
