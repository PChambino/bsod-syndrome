
#include <dos.h>

#include "setup.h"
#include "game.h"

int main(int argc, char **argv) {
	process_args(argc, argv);
	
	setup_timer();
	setup_rtc();
	setup_kbc();
	setup_video();
	
	atexit(tear_down);
	
	game_loop(fps);
	
	return 0;
}
