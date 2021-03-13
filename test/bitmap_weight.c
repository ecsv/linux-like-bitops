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

static DECLARE_BITMAP(test, MAX_TEST_BITS);

static size_t get_weight(const unsigned long *test, size_t bits)
{
	size_t i;
	size_t sum = 0;

	for (i = 0; i < bits; i++) {
		if (!test_bit(i, test))
			continue;

		sum++;
	}

	return sum;
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		bitmap_fill(test, i);
		assert(bitmap_weight(test, i) == i);

		bitmap_zero(test, i);
		assert(bitmap_weight(test, i) == 0);

		/* changes in overhead bits don't change weight */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test);

				assert(bitmap_weight(test, i) == 0);
			}
		}

		/* random weight */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;
			change_bit(t, test);

			assert(bitmap_weight(test, i) == get_weight(test, i));
		}
	}
	return 0;
}
