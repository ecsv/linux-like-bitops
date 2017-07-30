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
#include <string.h>

#define MAX_TEST_BITS 500

static DECLARE_BITMAP(test, MAX_TEST_BITS);

static bool is_filled(const unsigned long *bitmap, size_t bits)
{
	size_t i;
	size_t j;

	for (i = 0; i < bits; i += BITS_PER_LONG) {
		for (j = 0; j < BITS_PER_LONG; j++) {
			if (i + j < bits) {
				if (!(bitmap[i / BITS_PER_LONG] & (1UL << j)))
					return false;
			} else {
				if (bitmap[i / BITS_PER_LONG] & (1UL << j))
					return false;
			}
		}
	}

	return true;
}

int main(void)
{
	size_t i;

	for (i = 1; i < MAX_TEST_BITS; i++) {
		memset(test, 0xff, sizeof(test));
		bitmap_fill(test, i);
		assert(is_filled(test, i));

		memset(test, 0x0, sizeof(test));
		bitmap_fill(test, i);
		assert(is_filled(test, i));
	}
	return 0;
}
