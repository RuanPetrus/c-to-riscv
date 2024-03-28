#include "defs.h"
#include "synth.h"
#include "math.h"

/*
  TODO: Implement float pointer operations

  TODO: I think the hardware implementation of the syntethizer is not
  adding volume neither instruments to the play

  TODO: Pipeline keyboard is terrible, do something about'it
  TODO: Create proper keyboard modulo, with better keyhandling
*/

u32 get_time()
{
	u32 value ;
	__asm__ volatile ("csrr    %0, time" 
					  : "=r" (value)  /* output : register */
					  : /* input : none */
					  : /* clobbers: none */);
	return value;
}

#define MUSIC_SIZE 26*2
u32 music[] = {
	36, 166,
	36, 166,
	40,
	333,
	36,
	166,
	36,
	166,
	41,
	333,
	36,
	166,
	36,
	166,
	40,
	333,
	36,
	166,
	38,
	166,
	36,
	166,
	36,
	166,
	40,
	333,
	36,
	166,
	36,
	166,
	41,
	333,
	36,
	166,
	36,
	166,
	40,
	333,
	36,
	166,
	36,
	166,
	38,
	166,
	36,
	166,
	35,
	166,
	38,
	166
};

/*
 Kd data
 ASCII | Ready
 8'b    | 4'b

 TODO: Get more than one key
*/                 
#define KBDCRL  (*(volatile u32 *)0xFF200000)
#define KBDDATA (*(volatile u32 *)0xFF200004)

bool keyboard_event(char *key)
{
	bool ready = KBDCRL  & nbits_mask(1);
	bool val =   KBDDATA & nbits_mask(8);
	if (ready) *key = val;
	return ready;
}

i32 main(void)
{
	u32 last_time = get_time();
	u32 count = 0;
	u32 last_duration = 0;
	bool pause = false;
	float volume = 0.5;
	int instrument = 4;
	while(true) {
		char key;
		if (keyboard_event(&key)) {
			if (key == 'p') pause = !pause;
			else if (key == 'l') volume += 0.1F;
			else if (key == 'k') volume -= 0.1F;
			else if (key == 'j') instrument += 1;
			else if (key == 'h') instrument -= 1;
		}

		u32 current_time = get_time();
		u32 delta_time = current_time - last_time;
		if (!pause && delta_time > last_duration) {
			u32 note = music[count];
			u32 duration = music[count+1];
			count = (count + 2) % MUSIC_SIZE;
			last_time = current_time;
			last_duration = duration;
			play_note(note, duration, 20, volume);
		}
	}

	return 0;
}
