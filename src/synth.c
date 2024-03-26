#include "synth.h"
#include "math.h"

void play_note(u32 note, u32 duration_in_ms, u32 instrument, f32 volume)
{
	u32 volume_int = clamp(volume, 0, 1) * ((1 << 7) -1);
	u32 data =
		((duration_in_ms & (nbits_mask(11))) << 00) |
		((note & nbits_mask(7))              << 13) |
		((volume_int & nbits_mask(7))        << 20) |
		((instrument & nbits_mask(7))        << 27);

	SINV = data;
	// TODO: This is really necessarry ?
	// Wait until sintetizer clock
	while(SINC == 0) {}
}
