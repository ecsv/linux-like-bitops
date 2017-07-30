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

DECLARE_BITMAP(test1, 1);
DECLARE_BITMAP(test_long, BITS_PER_LONG);
DECLARE_BITMAP(test_long1, BITS_PER_LONG + 1);
DECLARE_BITMAP(test_double, BITS_PER_LONG * 2);
DECLARE_BITMAP(test_double1, BITS_PER_LONG * 2 + 1);

DECLARE_BITMAP(test127, 127);
DECLARE_BITMAP(test9034, 9034);

int main(void)
{
	assert(sizeof(test1) == sizeof(unsigned long));
	assert(sizeof(test_long) == sizeof(unsigned long));
	assert(sizeof(test_long1) == 2 * sizeof(unsigned long));
	assert(sizeof(test_double) == 2 * sizeof(unsigned long));
	assert(sizeof(test_double1) == 3 * sizeof(unsigned long));

	assert(sizeof(test127) * BITS_PER_BYTE >= 127);
	assert(sizeof(test9034) * BITS_PER_BYTE >= 9034);

	return 0;
}
