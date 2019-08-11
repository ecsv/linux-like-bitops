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

static unsigned int trivial_ffz(unsigned long x)
{
	size_t i;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if (((1UL << i) & x) == 0)
			return i + 1;
	}

	return 0;
}

int main(void)
{
	size_t i;
	unsigned long x;

	assert(bitops_ffz(0) == 1);
	assert(bitops_ffz(1) == 2);
	assert(bitops_ffz(2) == 1);
	assert(bitops_ffz(3) == 3);
	assert(bitops_ffz(ULONG_MAX) == 0);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned_long();

		assert(bitops_ffz(x) == trivial_ffz(x));
	}

	return 0;
}
