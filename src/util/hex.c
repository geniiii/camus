#include <util/hex.h>

void u16_to_hex(u16 num, char* restrict str, u8 group_size) {
	static const char* charset = "0123456789ABCDEF";

	for (i8 i = group_size - 1; i >= 0; --i) {
		str[i] = charset[num & 0xF];
		num /= 0x10;
	}

	str[group_size] = '\0';
}
