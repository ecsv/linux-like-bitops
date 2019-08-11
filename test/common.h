/* SPDX-License-Identifier: MIT */
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#ifndef __LINUX_LIKE_BITOPS_COMMON_H__
#define __LINUX_LIKE_BITOPS_COMMON_H__

#include <stdint.h>

/**
 * BITOPS_BUILD_BUG_ON - create "negative array" build error on expression
 * @e: expression which is considered to be a "bug"
 */
#define BITOPS_BUILD_BUG_ON(e) ((void)sizeof(char[1 - 2 * !!(e)]))

static __inline__ uint8_t getnum(void)
{
	static uint16_t s1 = UINT16_C(2);
	static uint16_t s2 = UINT16_C(1);
	static uint16_t s3 = UINT16_C(1);

	s1 *= UINT16_C(171);
	s1 %= UINT16_C(30269);

	s2 *= UINT16_C(172);
	s2 %= UINT16_C(30307);

	s3 *= UINT16_C(170);
	s3 %= UINT16_C(30323);

	return s1 ^ s2 ^ s3;
}

static __inline__ uint16_t get_unsigned16(void)
{
	uint16_t x = 0;
	size_t i;

	for (i = 0; i < sizeof(x); i++) {
		x <<= 8;
		x |= getnum();
	}

	return x;
}

static __inline__ uint32_t get_unsigned32(void)
{
	uint32_t x = 0;
	size_t i;

	BITOPS_BUILD_BUG_ON(BITS_PER_BYTE != 8);

	for (i = 0; i < sizeof(uint32_t); i++) {
		x <<= BITS_PER_BYTE;
		x |= getnum() & 0xff;
	}

	return x;
}

static __inline__ uint64_t get_unsigned64(void)
{
	uint64_t x = 0;
	size_t i;

	BITOPS_BUILD_BUG_ON(BITS_PER_BYTE != 8);

	for (i = 0; i < sizeof(uint64_t); i++) {
		x <<= BITS_PER_BYTE;
		x |= getnum() & 0xff;
	}

	return x;
}

static __inline__ unsigned long get_unsigned_long(void)
{
	unsigned long x = 0ul;
	size_t i;

	BITOPS_BUILD_BUG_ON(BITS_PER_BYTE != 8);

	for (i = 0; i < sizeof(unsigned long); i++) {
		x <<= BITS_PER_BYTE;
		x |= getnum() & 0xff;
	}

	return x;
}

#endif /* __LINUX_LIKE_BITOPS_COMMON_H__ */
