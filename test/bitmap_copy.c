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

int main(void)
{
	size_t i;
	size_t j;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		bitmap_zero(test1, i);

		/* randomly filled equal */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			change_bit(t, test1);
		}

		bitmap_copy(test2, test1, i);
		assert(bitmap_equal(test1, test2, i));
	}
	return 0;
}
