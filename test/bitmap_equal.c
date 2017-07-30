/* Minimal Linux-like bit manipulation helper functions test
 *
 * Copyright (c) 2012-2017, Sven Eckelmann <sven@narfation.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
static DECLARE_BITMAP(flipped, MAX_TEST_BITS);

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* filled is equal */
		bitmap_fill(test1, i);
		bitmap_fill(test2, i);
		assert(bitmap_equal(test1, test2, i));

		/* filled and not filled is not equal */
		bitmap_zero(test1, i);
		assert(!bitmap_equal(test1, test2, i));

		/* not filled is equal */
		bitmap_zero(test2, i);
		assert(bitmap_equal(test1, test2, i));

		/* randomly filled equal */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			change_bit(t, test1);
			change_bit(t, test2);
		}

		assert(bitmap_equal(test1, test2, i));

		/* changes in overhead bits doesn't change equality */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test1);

				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test2);

				assert(bitmap_equal(test1, test2, i));
			}
		}

		/* modified not equal */
		bitmap_zero(flipped, i);
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			if (test_and_set_bit(t, flipped))
				continue;

			change_bit(t, test2);
			assert(!bitmap_equal(test1, test2, i));
		}
	}
	return 0;
}
