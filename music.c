
#include "music.h"

float notes_frequency[] = { 16.35, 17.32, 18.35, 19.45, 20.6, 21.83, 23.12, 24.5, 25.96, 27.5, 29.14, 30.87,
	32.7, 34.65, 36.71, 38.89, 41.2, 43.65, 46.25, 49, 51.91, 55, 58.27, 61.74,
	65.41, 69.3, 73.42, 77.78, 82.41, 87.31, 92.5, 98, 103.83, 110, 116.54, 123.47,
	130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94,
	261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 493.88,
	523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880, 932.33, 987.77,
	1046.5, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760, 1864.66, 1975.53,
	2093, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520, 3729.31, 3951.07,
	4186.01, 4434.92, 4698.64, 4978.03};


typedef enum {OFF, INIT, PLAYING, PAUSING} MusicState;
static MusicState state = OFF;

static Song *song;
static int noteTime = 0;
static int songIndex = 0;

void rtc_isr() {
	Byte statC = read_rtcv(RTC_STAT_C);

	if ((statC & RTC_IRQF) != 0 && (statC & RTC_PF) != 0) {
		switch (state) {
			case OFF:
				break;
			case INIT:
				if (songIndex >= song->lenght) {
					state = OFF;
					break;
				}
				
				Note *note = &song->notes[songIndex++];
				noteTime = note->dur;
				timer_load(TIMER_2, TIMER_CLK / notes_frequency[note->freq]);
				speaker_on();
				state = PLAYING;
				break;
			case PLAYING:
				noteTime--;
				if (noteTime <= 0) {
					speaker_off();
					state = PAUSING;
					noteTime = song->pause;
				}
				break;
			case PAUSING:
				noteTime--;
				if (noteTime <= 0) {
					state = INIT;
				}				
		}
	}
	
	outportb(PIC1_CMD, EOI);
	outportb(PIC2_CMD, EOI);
}

void speaker_on() {
	outportb(SPEAKER_CTRL, inportb(SPEAKER_CTRL) | SPEAKER_ENABLE | TIMER2_ENABLE);
}

void speaker_off() {
	outportb(SPEAKER_CTRL, inportb(SPEAKER_CTRL) & ~(SPEAKER_ENABLE | TIMER2_ENABLE));
}

Song* newSong(int pause, Note notes[], int lenght) {
	Song *s = malloc(sizeof(Song));
	s->pause = pause;
	s->lenght = lenght;
	int sizeofnotes = lenght * sizeof(Note);
	s->notes = malloc(sizeofnotes);
	memcpy(s->notes, notes, sizeofnotes);
	
	return s;
}

void deleteSong(Song *s) {
	free(s->notes);
	free(s);
}

void play_song(Song *s) {
	song = s;
	songIndex = 0;
	state = INIT;
}


