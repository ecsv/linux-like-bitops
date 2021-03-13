// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

static unsigned int trivial_hweight(uint32_t x)
{
	size_t i;
	unsigned int c = 0;

	for (i = 0; i < BITS_PER_BYTE * sizeof(uint32_t); i++) {
		if ((UINT32_C(1) << i) & x)
			c++;
	}

	return c;
}

int main(void)
{
	size_t i;
	uint32_t x;

	assert(hweight32(0) == 0);
	assert(hweight32(1) == 1);
	assert(hweight32(2) == 1);
	assert(hweight32(3) == 2);
	assert(hweight32(UINT32_C(0xff)) == 8);
	assert(hweight32(UINT32_C(0xffff)) == 16);
	assert(hweight32(UINT32_C(0xffffff)) == 24);
	assert(hweight32(UINT32_C(0xffffffff)) == 32);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned32();

		assert(hweight32(x) == trivial_hweight(x));
	}

	return 0;
}
