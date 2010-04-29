
CC=gcc
CFLAGS=-Wall

PROG=BSoD.exe
OBJS=main.o setup.o video.o game.o queue.o ints.o timer.o kbd_isr.o t0_isr.o sprite.o read_xpm.o rtc.o music.o GQueue.o kbc.o mouse_isr.o

$(PROG): $(OBJS)
	$(CC) $^ kbc_init.o -o $@

main.o: setup.h game.h
setup.o: setup.h video.h ints.h game.h queue.h GQueue.h kbc.h
video.o: video.h
game.o: game.h video.h queue.h sprite.h
queue.o: queue.h
ints.o: ints.h
timer.o: timer.h ints.h
rtc.o: rtc.h ints.h
sprite.o: sprite.h video.h
read_xpm.o: sprite.h video.h
music.o: music.h timer.h
GQueue.o: GQueue.h
kbc.o: kbc.h


kbd_isr.o: kbd_isr.asm
	nasm -t -f coff $< -o $@
t0_isr.o: t0_isr.asm
	nasm -t -f coff $< -o $@
mouse_isr.o: mouse_isr.asm
	nasm -t -f coff $< -o $@

	
clean:
	rm $(OBJS)
