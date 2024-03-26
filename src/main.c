#include "defs.h"
#include "synth.h"

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

i32 main()
{
	u32 last_time = get_time();
	u32 count = 0;
	u32 last_duration = 0;
	while(true) {
		u32 current_time = get_time();
		if (current_time - last_time > last_duration) {
			u32 note = music[count];
			u32 duration = music[count+1];
			count = (count + 2) % MUSIC_SIZE;
			last_time = current_time;
			last_duration = duration;
			play_note(note, duration, 4, 0.5);
		}
	}

	return 0;
}
