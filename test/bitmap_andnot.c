// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
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
static DECLARE_BITMAP(test3, MAX_TEST_BITS);
static DECLARE_BITMAP(test4, MAX_TEST_BITS);

int main(void)
{
	size_t i;
	size_t j;
	uint8_t t;
	bool t1, t2;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* 0 & ~0 == 0 */
		bitmap_zero(test1, i);
		bitmap_zero(test2, i);
		bitmap_andnot(test3, test1, test2, i);
		assert(bitmap_empty(test3, i));

		/* 0 & ~1 == 0 */
		bitmap_zero(test1, i);
		bitmap_fill(test2, i);
		bitmap_and(test3, test1, test2, i);
		assert(bitmap_empty(test3, i));

		/* 1 & ~0 == 1 */
		bitmap_fill(test1, i);
		bitmap_zero(test2, i);
		bitmap_andnot(test3, test1, test2, i);
		assert(bitmap_full(test3, i));

		/* 1 & ~1 == 0 */
		bitmap_fill(test1, i);
		bitmap_fill(test2, i);
		bitmap_andnot(test3, test1, test2, i);
		assert(bitmap_empty(test3, i));

		/* randomly assign bits */
		bitmap_zero(test1, i);
		bitmap_zero(test2, i);
		bitmap_zero(test3, i);
		for (j = 0; j < i; j++) {
			t = getnum() & 0x3;
			t1 = !!(t & 0x1);
			t2 = !!(t & 0x2);

			if (t1)
				set_bit(j, test1);

			if (t2)
				set_bit(j, test2);

			if (t1 && !t2)
				set_bit(j, test3);

			bitmap_andnot(test4, test1, test2, i);
			assert(bitmap_equal(test3, test4, i));
		}
	}
	return 0;
}
