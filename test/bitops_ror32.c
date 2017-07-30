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
#include <stdint.h>

#include "common.h"

#define TEST_RANDOM 50000

static void check_rotate(uint32_t x, size_t n, uint32_t r)
{
	size_t i;
	bool b1;
	bool b2;

	for (i = 0; i < sizeof(x) * BITS_PER_BYTE; i++) {
		b1 = !!(x & (1UL << ((i + n) % (sizeof(x) * BITS_PER_BYTE))));
		b2 = !!(r & (1UL << i));

		assert(b1 == b2);
	}
}

int main(void)
{
	size_t i;
	size_t j;
	uint32_t x;
	uint32_t r;

	for (i = 0; i < TEST_RANDOM; i++) {
		x = get_unsigned32();

		for (j = 1; j < BITS_PER_BYTE * sizeof(x); j++) {
			r = bitops_ror32(x, j);
			check_rotate(x, j, r);
		}
	}

	return 0;
}
