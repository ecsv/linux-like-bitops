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

static DECLARE_BITMAP(test, MAX_TEST_BITS);

static size_t find_next(unsigned long *bitmap, size_t bits, size_t start)
{
	size_t i;

	for (i = start; i < bits; i++) {
		if (!test_bit(i, bitmap))
			return i;
	}

	return bits;
}

static size_t check_bits(unsigned long *bitmap, size_t bits)
{
	size_t r;
	size_t bit;
	size_t c = 0;
	size_t last_bit = 0;

	for_each_clear_bit (bit, bitmap, bits) {
		r = find_next(bitmap, bits, last_bit);
		assert(bit == r);

		last_bit = bit + 1;
		c++;
	}

	assert(find_next(bitmap, bits, last_bit) == bits);

	return c;
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		bitmap_zero(test, i);
		assert(check_bits(test, i) == i);

		bitmap_fill(test, i);
		assert(check_bits(test, i) == 0);

		/* changes in overhead bits don't change found bits */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test);

				assert(check_bits(test, i) == 0);
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
