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

static void mark_bitrange(unsigned long *bitmap, size_t start, size_t bits)
{
	size_t i;

	for (i = 0; i < bits; i++)
		set_bit(start + i, bitmap);
}

int main(void)
{
	size_t i;
	size_t j;
	uint16_t t;
	size_t restbits;
	uint16_t r;

	for (i = 1; i < MAX_TEST_BITS; i++) {

		bitmap_zero(test1, i);
		bitmap_set(test1, 0, i);
		bitmap_fill(test2, i);
		assert(bitmap_equal(test1, test2, i));

		bitmap_zero(test2, i);
		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			bitmap_zero(test1, i);
			bitmap_set(test1, t, 0);
			assert(bitmap_equal(test1, test2, i));
		}

		bitmap_zero(test1, i);
		bitmap_zero(test2, i);

		for (j = 0; j < TEST_RANDOM; j++) {
			t = get_unsigned16() % i;

			restbits = i - t;
			if (!restbits)
				r = 0;
			else
				r = get_unsigned16() % restbits;

			bitmap_set(test1, t, r);
			mark_bitrange(test2, t, r);

			assert(bitmap_equal(test1, test2, i));
		}
	}
	return 0;
}
