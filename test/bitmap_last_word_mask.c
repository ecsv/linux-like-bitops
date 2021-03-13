// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_TEST_BITS 50000

static bool test_bits_from_msb(unsigned long x, size_t bits)
{
	size_t i;
	size_t bits_filled;

	bits_filled = bits % BITS_PER_LONG;
	if (bits_filled == 0)
		bits_filled = BITS_PER_LONG;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if (i < bits_filled) {
			if (!(x & (1UL << i)))
				return false;
		} else {
			if (x & (1UL << i))
				return false;
		}
	}

	return true;
}

int main(void)
{
	unsigned long x;
	size_t i;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		x = BITMAP_LAST_WORD_MASK(i);
		assert(test_bits_from_msb(x, i));
	}

	return 0;
}
