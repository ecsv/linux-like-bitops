// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_TEST_BITS 50000

static bool test_bits_from_lsb(unsigned long x, size_t bits)
{
	size_t i;
	size_t bits_zero;

	bits_zero = bits % BITS_PER_LONG;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if (i < bits_zero) {
			if (x & (1UL << i))
				return false;
		} else {
			if (!(x & (1UL << i)))
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
		x = BITMAP_FIRST_WORD_MASK(i);
		assert(test_bits_from_lsb(x, i));
	}

	return 0;
}
