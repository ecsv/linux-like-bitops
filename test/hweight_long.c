// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <limits.h>
#include <stddef.h>

#include "common.h"

static unsigned int trivial_hweight(unsigned long x)
{
	size_t i;
	unsigned int c = 0;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if ((1UL << i) & x)
			c++;
	}

	return c;
}

int main(void)
{
	size_t i;
	unsigned long x;

	assert(hweight_long(0) == 0);
	assert(hweight_long(1) == 1);
	assert(hweight_long(2) == 1);
	assert(hweight_long(3) == 2);
	assert(hweight_long(ULONG_MAX) == BITS_PER_LONG);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned_long();

		assert(hweight_long(x) == trivial_hweight(x));
	}

	return 0;
}
