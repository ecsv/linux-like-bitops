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
#include <string.h>

#include "common.h"

#define MAX_TEST_BITS 500
#define TEST_RANDOM 500

static uint8_t bytemap[MAX_TEST_BITS];
static DECLARE_BITMAP(test, MAX_TEST_BITS);

static void check_equal(const unsigned long *bitmap, const uint8_t *bytemap,
			size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++)
		assert(test_bit(i, bitmap) == !!bytemap[i]);
}

int main(void)
{
	size_t i;
	size_t j;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		memset(bytemap, 0x0, sizeof(bytemap));
		bitmap_zero(test, i);

		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			change_bit(t, test);
			bytemap[t] ^= 1;
			check_equal(test, bytemap, i);
		}
	}
	return 0;
}
