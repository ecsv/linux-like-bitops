// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "common.h"

#define MAX_TEST_BITS 500
#define TEST_RANDOM 500

static DECLARE_BITMAP(test1, MAX_TEST_BITS);
static DECLARE_BITMAP(test2, MAX_TEST_BITS);

static void clear_bitrange(unsigned long *bitmap, size_t start, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++)
		clear_bit(start + i, bitmap);
}

int main(void)
{
	size_t i;
	size_t j;
	uint16_t t;
	size_t restbits;
	uint16_t r;

	for (i = 1; i < MAX_TEST_BITS; i++) {

		bitmap_fill(test1, i);
		bitmap_clear(test1, 0, i);
		bitmap_zero(test2, i);
		assert(bitmap_equal(test1, test2, i));

		bitmap_fill(test2, i);
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			bitmap_fill(test1, i);
			bitmap_clear(test1, t, 0);
			assert(bitmap_equal(test1, test2, i));
		}

		bitmap_fill(test1, i);
		bitmap_fill(test2, i);

		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			restbits = i - t;
			if (!restbits)
				r = 0;
			else
				r = get_unsigned16() % restbits;

			bitmap_clear(test1, t, r);
			clear_bitrange(test2, t, r);

			assert(bitmap_equal(test1, test2, i));
		}
	}
	return 0;
}
