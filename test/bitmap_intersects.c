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

static bool check_intersect(const unsigned long *test1,
			    const unsigned long *test2, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++) {
		if (!test_bit(i, test1))
			continue;

		if (!test_bit(i, test2))
			continue;

		return true;
	}

	return false;
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;
	bool intersected;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* filled have bits in common */
		bitmap_fill(test1, i);
		bitmap_fill(test2, i);
		assert(bitmap_intersects(test1, test2, i));

		/* filled and not filled have nothing in common */
		bitmap_zero(test1, i);
		assert(!bitmap_intersects(test1, test2, i));

		/* not filled have nothing in common */
		bitmap_zero(test2, i);
		assert(!bitmap_intersects(test1, test2, i));

		/* changes in overhead bits doesn't make them intersect */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test1);
				change_bit(i + t, test2);

				assert(!bitmap_intersects(test1, test2, i));
			}
		}

		/* random filled may intersect */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;
			change_bit(t, test1);

			t = get_unsigned16() % i;
			change_bit(t, test2);

			intersected = check_intersect(test1, test2, i);
			assert(bitmap_intersects(test1, test2, i) == intersected);
		}
	}
	return 0;
}
