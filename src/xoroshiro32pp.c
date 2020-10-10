#include "xoroshiro32pp.h"

static inline u16 rotl(const u16 x, u16 k) {
	return (x << k) | (x >> ((sizeof x * 8) - k));
}

static u16 s[2] = {0, 1};

u16 xs32pp_next(void) {
	const u16 s0	 = s[0];
	u16		  s1	 = s[1];
	const u16 result = rotl(s0 + s1, 9) + s0;

	s1 ^= s0;
	s[0] = rotl(s0, 13) ^ s1 ^ (s1 << 5);  // a, b
	s[1] = rotl(s1, 10);				   // c

	return result;
}
