
/* Type definitions on platform */
typedef int i32;
typedef short i16;
typedef char i8;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

typedef i32 bool;
#define true 1
#define false 0

// NOTE: If you inline a function in C99 standard, you must compile with -O2 or make the function static

// TODO: Implement a MEMSET, MEMMOVE, MEMCPY, MEMCMP so the compiler can optimize out some for loops
// TODO: Do something for assert

void assert(bool cond)
{
	(void) cond;
}

u32 get_time()
{
	u32 value ;
	__asm__ volatile ("csrr    %0, time" 
					  : "=r" (value)  /* output : register */
					  : /* input : none */
					  : /* clobbers: none */);
	return value;
}

static inline u32 nbits_mask(u32 num_bytes)
{
	assert(num_bytes <= sizeof(u32) * 8);
	return (1 << (num_bytes)) - 1;
}

// Sintetizer address
#define  SINV  (*(volatile u32 *)0xFF200178)
#define  SINC  (*(volatile u32 *)0xFF20017C)

/*
 Sintesizer data
 Melody | Instrument | Volume | Pitch | End | Repeat | Duration
 1'b    | 4'b        | 7'b    | 7'b   | 1'b | 1'b    | 11'b
*/                  

void play(u32 pich, u32 duration_in_ms, u32 instrument, u32 volume)
{
	u32 data = ((duration_in_ms & (nbits_mask(11))) << 0)
		| ((pich & (nbits_mask(7))) << 13)
		| ((volume & (nbits_mask(7))) << 20)
		| ((instrument & (nbits_mask(7))) << 27);

	SINV = data;
	// TODO: This is really necessarry ?
	// Wait until sintetizer clock
	while(SINC == 0) {}
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
			play(note, duration, 4, 40);
		}
	}

	return 0;
}
