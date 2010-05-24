
#include <dos.h>

#include "setup.h"
#include "game.h"



int main(int argc, char **argv) {
	process_args(argc, argv);

	setup_log();
	setup_video();
	setup_kbc();
	setup_rtc();
	
	atexit(tear_down);
	
	game_loop(fps);
	
	return 0;
}
