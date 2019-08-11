// SPDX-License-Identifier: MIT
/* Minimal Linux-like bit manipulation helper functions test
 *
 * SPDX-FileCopyrightText: 2012-2017, Sven Eckelmann <sven@narfation.org>
 */

#include "../bitops.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include "common.h"

static unsigned int trivial_hweight(uint64_t x)
{
	size_t i;
	unsigned int c = 0;

	for (i = 0; i < BITS_PER_BYTE * sizeof(uint64_t); i++) {
		if ((UINT64_C(1) << i) & x)
			c++;
	}

	return c;
}

int main(void)
{
	size_t i;
	uint64_t x;

	assert(hweight64(0) == 0);
	assert(hweight64(1) == 1);
	assert(hweight64(2) == 1);
	assert(hweight64(3) == 2);
	assert(hweight64(UINT64_C(0xff)) == 8);
	assert(hweight64(UINT64_C(0xffff)) == 16);
	assert(hweight64(UINT64_C(0xffffff)) == 24);
	assert(hweight64(UINT64_C(0xffffffff)) == 32);
	assert(hweight64(UINT64_C(0xffffffffff)) == 40);
	assert(hweight64(UINT64_C(0xffffffffffff)) == 48);
	assert(hweight64(UINT64_C(0xffffffffffffff)) == 56);
	assert(hweight64(UINT64_C(0xffffffffffffffff)) == 64);

	for (i = 0; i < 1024; i++) {
		x = get_unsigned64();

		assert(hweight64(x) == trivial_hweight(x));
	}

	return 0;
}
