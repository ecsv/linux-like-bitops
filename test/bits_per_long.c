// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stddef.h>

int main(void)
{
	size_t i;
	unsigned long x = 1u;

	for (i = 0; i < BITS_PER_LONG; i++) {
		assert(x);
		x <<= 1;
	}

	assert(!x);

	return 0;
}
