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

int main(void)
{
	size_t i;
	unsigned long numerator, denominator, result;

	for (i = 0; i < 512; i++) {
		numerator = get_unsigned_long();
		denominator = get_unsigned_long();

		/* division by zero */
		if (denominator == 0)
			continue;

		/* limit caused by addition in BITOPS_DIV_CEIL */
		if (numerator > 0 && numerator > (ULONG_MAX - denominator))
			continue;

		result = numerator / denominator;
		if (numerator % denominator)
			result++;

		assert(result == BITOPS_DIV_CEIL(numerator, denominator));
	}

	return 0;
}
