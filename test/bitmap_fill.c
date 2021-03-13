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

#define MAX_TEST_BITS 500

static DECLARE_BITMAP(test, MAX_TEST_BITS);

static bool is_filled(const unsigned long *bitmap, size_t bits)
{
	size_t i;
	size_t j;

	for (i = 0; i < bits; i += BITS_PER_LONG) {
		for (j = 0; j < BITS_PER_LONG; j++) {
			if (i + j < bits) {
				if (!(bitmap[i / BITS_PER_LONG] & (1UL << j)))
					return false;
			} else {
				if (bitmap[i / BITS_PER_LONG] & (1UL << j))
					return false;
			}
		}
	}

	return true;
}

int main(void)
{
	size_t i;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		memset(test, 0xff, sizeof(test));
		bitmap_fill(test, i);
		assert(is_filled(test, i));

		memset(test, 0x0, sizeof(test));
		bitmap_fill(test, i);
		assert(is_filled(test, i));
	}
	return 0;
}
