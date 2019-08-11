// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

static unsigned int trivial_hweight(uint16_t x)
{
	size_t i;
	unsigned int c = 0;

	for (i = 0; i < BITS_PER_BYTE * sizeof(uint16_t); i++) {
		if ((UINT16_C(1) << i) & x)
			c++;
	}

	return c;
}

int main(void)
{
	size_t i;
	uint16_t x;

	assert(hweight16(0) == 0);
	assert(hweight16(1) == 1);
	assert(hweight16(2) == 1);
	assert(hweight16(3) == 2);
	assert(hweight16(UINT16_C(0xff)) == 8);
	assert(hweight16(UINT16_C(0xffff)) == 16);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned16();

		assert(hweight16(x) == trivial_hweight(x));
	}

	return 0;
}
