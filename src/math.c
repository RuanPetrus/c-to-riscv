#include "math.h"

inline f32 clamp(f32 val, f32 l, f32 r)
{
	return (val < l ? l : (val > r ? r : val));
}

inline i32 clampi32(i32 val, i32 l, i32 r)
{
	return (val < l ? l : (val > r ? r : val));
}

inline u32 nbits_mask(u32 num_bytes)
{
	return (1 << (num_bytes)) - 1;
}
