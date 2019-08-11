// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "common.h"

#define MAX_TEST_BITS 500
#define TEST_RANDOM 500

static DECLARE_BITMAP(test1, MAX_TEST_BITS);
static DECLARE_BITMAP(test2, MAX_TEST_BITS);
static DECLARE_BITMAP(test3, MAX_TEST_BITS);

int main(void)
{
	size_t i;
	size_t j;
	bool t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* ~0 == 1 */
		bitmap_zero(test1, i);
		bitmap_complement(test2, test1, i);
		assert(bitmap_full(test2, i));

		/* ~1 == 0 */
		bitmap_fill(test1, i);
		bitmap_complement(test2, test1, i);
		assert(bitmap_empty(test2, i));

		/* randomly assign bits */
		bitmap_zero(test1, i);
		bitmap_fill(test2, i);
		for (j = 0; j < i; j++) {
			t = !!(getnum() & 0x1);

			if (t) {
				set_bit(j, test1);
				clear_bit(j, test2);
			}

			bitmap_complement(test3, test1, i);
			assert(bitmap_equal(test2, test3, i));
		}
	}
	return 0;
}
