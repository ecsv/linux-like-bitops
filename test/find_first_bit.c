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

static size_t find_first(unsigned long *bitmap, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++) {
		if (test_bit(i, bitmap))
			return i;
	}

	return bits;
}

static void check_bits(unsigned long *bitmap, size_t bits)
{
	size_t r1;
	size_t r2;

	r1 = find_first(bitmap, bits);
	r2 = find_first_bit(bitmap, bits);

	assert(r1 == r2);
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		bitmap_fill(test, i);
		assert(find_first_bit(test, i) == 0);

		bitmap_zero(test, i);
		assert(find_first_bit(test, i) == i);

		/* changes in overhead bits don't change found bits */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test);

				assert(find_first_bit(test, i) == i);
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
