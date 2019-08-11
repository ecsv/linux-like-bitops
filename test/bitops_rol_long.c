// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

#define TEST_RANDOM 50000

static void check_rotate(unsigned long x, size_t n, unsigned long r)
{
	size_t i;
	bool b1;
	bool b2;

	for (i = 0; i < sizeof(x) * BITS_PER_BYTE; i++) {
		b1 = !!(x & (1UL << ((i - n) % (sizeof(x) * BITS_PER_BYTE))));
		b2 = !!(r & (1UL << i));

		assert(b1 == b2);
	}
}

int main(void)
{
	size_t i;
	size_t j;
	unsigned long x;
	unsigned long r;

	for (i = 0; i < TEST_RANDOM; i++) {
		x = get_unsigned_long();

		for (j = 1; j < BITS_PER_BYTE * sizeof(x); j++) {
			r = bitops_rol_long(x, j);
			check_rotate(x, j, r);
		}
	}

	return 0;
}
