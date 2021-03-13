// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

static void check_rotate(uint16_t x, size_t n, uint16_t r)
{
	size_t i;
	bool b1;
	bool b2;

	for (i = 0; i < sizeof(x) * BITS_PER_BYTE; i++) {
		b1 = !!(x & (1UL << ((i + n) % (sizeof(x) * BITS_PER_BYTE))));
		b2 = !!(r & (1UL << i));

		assert(b1 == b2);
	}
}

int main(void)
{
	size_t i;
	size_t j;
	uint16_t x;
	uint16_t r;

	for (i = 0; i < 65536; i++) {
		x = i;

		for (j = 1; j < BITS_PER_BYTE * sizeof(x); j++) {
			r = bitops_ror16(x, j);
			check_rotate(x, j, r);
		}
	}

	return 0;
}
