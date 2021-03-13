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

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* filled is not empty */
		bitmap_fill(test, i);
		assert(!bitmap_empty(test, i));

		/* not filled is empty */
		bitmap_zero(test, i);
		assert(bitmap_empty(test, i));

		/* changes in overhead bits doesn't change emptyness */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test);

				assert(bitmap_empty(test, i));
			}
		}

		/* randomly filled is not empty */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			set_bit(t, test);

			assert(!bitmap_empty(test, i));
		}
	}
	return 0;
}
