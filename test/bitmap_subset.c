/* Minimal Linux-like bit manipulation helper functions test
 *
 * Copyright (c) 2012-2014, Sven Eckelmann <sven@narfation.org>
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

static bool check_subset(const unsigned long *test1,
			 const unsigned long *test2, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++) {
		if (test_bit(i, test2))
			continue;

		if (test_bit(i, test1))
			return false;
	}

	return true;
}

int main(void)
{
	size_t i;
	size_t j;
	size_t overhead_bits;
	uint16_t t;
	bool subset;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* filled is are equal and therefore subset */
		bitmap_fill(test1, i);
		bitmap_fill(test2, i);
		assert(bitmap_subset(test1, test2, i));

		/* filled is not subset of not filled */
		bitmap_zero(test2, i);
		assert(!bitmap_subset(test1, test2, i));

		/* not filled is subset of filled */
		bitmap_zero(test1, i);
		bitmap_fill(test2, i);
		assert(bitmap_subset(test1, test2, i));

		/* not filled is subset of not filled */
		bitmap_zero(test2, i);
		assert(bitmap_subset(test1, test2, i));

		/* changes in overhead bits don't change subset/superset */
		if (i % BITS_PER_LONG > 0) {
			overhead_bits = BITS_PER_LONG - (i % BITS_PER_LONG);

			for (j = 0; j < TEST_RANDOM; j++) {
				t = get_unsigned16() % overhead_bits;
				change_bit(i + t, test1);

				assert(bitmap_subset(test1, test2, i));
			}
		}

		/* random filled may create subset/superset */
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;
			change_bit(t, test1);

			t = get_unsigned16() % i;
			change_bit(t, test2);

			subset = check_subset(test1, test2, i);
			assert(bitmap_subset(test1, test2, i) == subset);
		}
	}
	return 0;
}
