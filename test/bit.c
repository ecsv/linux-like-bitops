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

static int getmsb_bitpos(unsigned long x)
{
	int i = -1;

	while (x) {
		i++;
		x >>= 1;
	}

	return i;
}

int main(void)
{
	size_t i;
	unsigned long x;
	uint8_t bitpos;

	assert(BIT(0) == 0x01UL);
	if (BITS_PER_LONG == 32)
		assert(BIT(BITS_PER_LONG - 1) == 0x80000000UL);
	if (BITS_PER_LONG == 64)
		assert(BIT(BITS_PER_LONG - 1) == 0x8000000000000000UL);

	for (i = 0; i < 1024; i++) {
		bitpos = getnum();
		bitpos %= BITS_PER_LONG;

		x = BIT(bitpos);
		assert(bitpos == getmsb_bitpos(x));
	}

	return 0;
}
