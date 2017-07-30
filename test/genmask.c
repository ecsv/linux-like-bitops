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

static int getlsb_bitpos(unsigned long x)
{
	unsigned int i;

	for (i = 0; i < BITS_PER_LONG; i++) {
		if (BIT(i) & x)
			return i;
	}

	return -1;
}

int main(void)
{
	size_t i;
	unsigned long x;
	uint8_t bitpos_h, bitpos_l, t;

	assert(GENMASK(0, 0) == 0x01UL);
	assert(GENMASK(1, 0) == 0x03UL);
	assert(GENMASK(15, 8) == 0xff00UL);
	if (BITS_PER_LONG == 32)
		assert(GENMASK(BITS_PER_LONG - 1, 1) == 0xfffffffeUL);
	if (BITS_PER_LONG == 64)
		assert(GENMASK(BITS_PER_LONG - 1, 1) == 0xfffffffffffffffeUL);

	for (i = 0; i < 102400; i++) {
		bitpos_h = getnum();
		bitpos_h %= BITS_PER_LONG;
		bitpos_l = getnum();
		bitpos_l %= BITS_PER_LONG;

		if (bitpos_h < bitpos_l) {
			t = bitpos_l;
			bitpos_l = bitpos_h;
			bitpos_h = t;
		}

		t = bitpos_h - bitpos_l + 1;
		if (t >= BITS_PER_LONG)
			bitpos_h--;

		x = GENMASK(bitpos_h, bitpos_l);
		t = bitpos_h - bitpos_l + 1;
		assert(bitpos_h == getmsb_bitpos(x));
		assert(hweight_long(x) == t);
		assert(bitpos_l == getlsb_bitpos(x));
	}

	return 0;
}
