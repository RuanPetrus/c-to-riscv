
/* Int definitions on platform */
typedef int i32;
typedef short i16;
typedef char i8;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

#define true 1
#define false 1

/*

# Note Data  = 32 bits     |   1'b - Melody   |   4'b - Instrument   |   7'b - Volume
                           |   7'b - Pitch   |   1'b - End   |   1'b - Repeat   |   11'b - Duration   |  
                                                                                    ^ lsb
0xFF200178
*/                  

void assert(int cond) {
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

// Sintetizer address
#define  SINV  (*(volatile u32 *)0xFF200178)
#define  SINC  (*(volatile u32 *)0xFF20017C)

u32 mask(u32 num_bytes)
{
	assert(num_bytes <= sizeof(u32) * 8);
	return (1 << (num_bytes)) - 1;
}

void play(u8 pich, u16 duration_in_ms, u8 instrument, u8 volume)
{
	u32 data = ((duration_in_ms & (mask(11))) << 0)
		| ((pich & (mask(7))) << 13)
		| ((volume & (mask(7))) << 20)
		| ((instrument & (mask(7))) << 27);

	SINV = data;
	// TODO: This is really necessarry ?
	// Wait until sintetizer clock
	while(SINC == 0) {}
}

#define MUSIC_SIZE 26
u32 music[] = {
			 36,166,36,166,40,333,36,166,36,166,41,333,36,166,36,166,40,333,36,166,38,166,36,166,36,166,40,333,36,166,36,166,41,333,36,166,36,166,40,333,36,166,36,166,38,166,36,166,35,166,38,166};

int main()
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
