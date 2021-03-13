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

static size_t find_next(unsigned long *bitmap, size_t bits, size_t start)
{
	size_t i;

	for (i = start; i < bits; i++) {
		if (test_bit(i, bitmap))
			return i;
	}

	return bits;
}

static void check_bits(unsigned long *bitmap, size_t bits)
{
	size_t i = 0;
	size_t r1;
	size_t r2;

	while (i < bits) {
		r1 = find_next(bitmap, bits, i);
		r2 = find_next_bit(bitmap, bits, i);

		assert(r1 == r2);
		i = r1 + 1;
	}
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		bitmap_fill(test, i);
		assert(find_next_bit(test, i, 0) == 0);

		bitmap_zero(test, i);
		assert(find_next_bit(test, i, 0) == i);

		/* changes in overhead bits don't change found bits */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test);

				assert(find_next_bit(test, i, 0) == i);
			}
		}

		/* random */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;
			change_bit(t, test);

			check_bits(test, i);
		}
	}
	return 0;
}
