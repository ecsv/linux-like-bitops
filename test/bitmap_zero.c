// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#define MAX_TEST_BITS 50000

static DECLARE_BITMAP(test, MAX_TEST_BITS);

static bool is_empty(const unsigned long *bitmap, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i += BITS_PER_LONG) {
		if (bitmap[i / BITS_PER_LONG] != 0UL)
			return false;
	}

	return true;
}

int main(void)
{
	size_t i;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		memset(test, 0xff, sizeof(test));
		bitmap_zero(test, i);
		assert(is_empty(test, i));

		memset(test, 0x0, sizeof(test));
		bitmap_zero(test, i);
		assert(is_empty(test, i));
	}
	return 0;
}
