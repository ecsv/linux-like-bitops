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

static unsigned int trivial_ffs(unsigned long x)
{
	size_t i;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if ((1UL << i) & x)
			return i + 1;
	}

	return 0;
}

int main(void)
{
	size_t i;
	unsigned long x;

	assert(bitops_ffs(0) == 0);
	assert(bitops_ffs(1) == 1);
	assert(bitops_ffs(2) == 2);
	assert(bitops_ffs(3) == 1);
	assert(bitops_ffs(ULONG_MAX) == 1);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned_long();

		assert(bitops_ffs(x) == trivial_ffs(x));
	}

	return 0;
}
