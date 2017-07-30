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
#include <stddef.h>

#include "common.h"

#define MAX_TEST_BITS 500
#define TEST_RANDOM 500

static DECLARE_BITMAP(test1, MAX_TEST_BITS);
static DECLARE_BITMAP(test2, MAX_TEST_BITS);

static void check_shift(unsigned long *bitmap, const unsigned long *src,
			size_t n, size_t bits)
{
	size_t i, j;
	bool b1;
	bool b2;

	if (n > bits)
		n = bits;

	for (i = 0, j = bits - 1; i + n < bits; i++, j--) {
		b1 = test_bit(j - n, src);
		b2 = test_bit(j, bitmap);
		assert(b1 == b2);
	}

	for (i = bits - n, j = n - 1; i < bits; i++, j--) {
		b2 = test_bit(j, bitmap);
		assert(!b2);
	}
}

int main(void)
{
	size_t i;
	size_t j;
	bool t;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		/* randomly assign bits */
		bitmap_zero(test1, i);
		for (j = 0; j < i; j++) {
			t = !!(getnum() & 0x1);

			if (t)
				set_bit(j, test1);
		}

		for (j = 0; j < i; j++) {
			bitmap_shift_left(test2, test1, j, i);
			check_shift(test2, test1, j, i);
		}
	}
	return 0;
}
