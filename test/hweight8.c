// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

static unsigned int trivial_hweight(uint8_t x)
{
	size_t i;
	unsigned int c = 0;

	for (i = 0; i < 8; i++) {
		if ((UINT8_C(1) << i) & x)
			c++;
	}

	return c;
}

int main(void)
{
	size_t i;
	uint8_t x;

	assert(hweight8(0) == 0);
	assert(hweight8(1) == 1);
	assert(hweight8(2) == 1);
	assert(hweight8(3) == 2);
	assert(hweight8(UINT8_C(0xff)) == 8);

	for (i = 0; i < 256; i++) {
		x = i;

		assert(hweight8(x) == trivial_hweight(x));
	}

	return 0;
}
