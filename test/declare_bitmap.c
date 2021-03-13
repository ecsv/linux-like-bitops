// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
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
