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
#include <stddef.h>
#include <stdint.h>

#include "common.h"

static int getmsb_bitpos(unsigned long x)
{
	int i = -1;

	while (x) {
		i++;
		x >>= 1;
	}

	return i;
}

int main(void)
{
	size_t i;
	unsigned long x;
	uint8_t bitpos;

	assert(BIT(0) == 0x01UL);
	if (BITS_PER_LONG == 32)
		assert(BIT(BITS_PER_LONG - 1) == 0x80000000UL);
	if (BITS_PER_LONG == 64)
		assert(BIT(BITS_PER_LONG - 1) == 0x8000000000000000UL);

	for (i = 0; i < 1024; i++) {
		bitpos = getnum();
		bitpos %= BITS_PER_LONG;

		x = BIT(bitpos);
		assert(bitpos == getmsb_bitpos(x));
	}

	return 0;
}
