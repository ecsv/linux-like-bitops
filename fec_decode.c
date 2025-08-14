// SPDX-License-Identifier: MIT
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "fec_decode.h"

#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "bitops.h"
#include "fec.h"

static uint32_t fec_prbs23(uint32_t x)
{
	uint32_t b0 = x & 1;
	uint32_t b1 = (x & 32) >> 5;

	return (x >> 1) | ((b0 ^ b1) << 22);
}

void fec_calculate_parity_row(unsigned long *row, unsigned long symbol_no)
{
	unsigned int m = 0;
	uint32_t x;

	bitmap_clear(row, 0, FEC_SYMBOLS_PER_GENERATION - 1);

	/* it is odd but symbol_no starts at 1 and not 0 */
	if (symbol_no <= FEC_SYMBOLS_PER_GENERATION) {
		set_bit(symbol_no - 1, row);
		return;
	}

	/* ensure better random values by not using 2 ** n rings */
	if (hweight_long(FEC_SYMBOLS_PER_GENERATION) == 1)
		m = 1;

	/* per line seed */
	x = symbol_no;
	x *= 1001;
	x += 1;

	/* set half of the bits to 1 */
	uint32_t nb_coeff = 0;

	while (nb_coeff < FEC_SYMBOLS_PER_GENERATION / 2) {
		uint32_t r;

		/* multiple rounds can happen when m == 1 */
		do {
			x = fec_prbs23(x);
			r = x % (FEC_SYMBOLS_PER_GENERATION + m);
		} while (r >= FEC_SYMBOLS_PER_GENERATION);

		if (test_bit(r, row) != 0)
			continue;

		set_bit(r, row);
		nb_coeff++;
	}
}

void fec_xor_symbol(uint8_t dst[FEC_SYMBOL_SIZE], const uint8_t src[FEC_SYMBOL_SIZE])
{
	size_t i;

	for (i = 0; i < FEC_SYMBOL_SIZE; i++)
		dst[i] ^= src[i];
}

void fec_decode_init(struct fec_decode *g, uint8_t frag_index)
{
	memset(g, 0, sizeof(*g));

	g->frag_index = frag_index & 0x3;
}

static void fec_decode_add_symbol(struct fec_decode *g,
				  const uint8_t symbol[FEC_SYMBOL_SIZE], size_t n)
{
	DECLARE_BITMAP(parity, FEC_SYMBOLS_PER_GENERATION);
	uint8_t s[FEC_SYMBOL_SIZE];
	size_t end;
	size_t pos;
	size_t i;

	memcpy(s, symbol, FEC_SYMBOL_SIZE);
	fec_calculate_parity_row(parity, n);

	/* gaussian elimination - forward elimination */
	for_each_set_bit(i, parity, FEC_SYMBOLS_PER_GENERATION) {
		if (!test_bit(i, g->parity[i].row))
			continue;

		bitmap_xor(parity, parity, g->parity[i].row, FEC_SYMBOLS_PER_GENERATION);
		fec_xor_symbol(s, g->data[i]);
	}

	if (bitmap_weight(parity, FEC_SYMBOLS_PER_GENERATION) == 0)
		return;

	/* save to position based on least significant set bit */
	pos = find_first_bit(parity, FEC_SYMBOLS_PER_GENERATION);
	bitmap_copy(g->parity[pos].row, parity, FEC_SYMBOLS_PER_GENERATION);
	memcpy(g->data[pos], s, FEC_SYMBOL_SIZE);

	/* gaussian elimination - back substitution */
	for (end = 0; end < FEC_SYMBOLS_PER_GENERATION; end++) {
		pos = FEC_SYMBOLS_PER_GENERATION - end - 1;
		if (bitmap_weight(g->parity[pos].row, FEC_SYMBOLS_PER_GENERATION) != 1)
			continue;

		for (i = 0; i < pos; i++) {
			if (!test_bit(pos, g->parity[i].row))
				continue;

			bitmap_xor(g->parity[i].row, g->parity[i].row,
				   g->parity[pos].row, FEC_SYMBOLS_PER_GENERATION);
			fec_xor_symbol(g->data[i], g->data[pos]);
		}
	}
}

int fec_decode_add_packet(struct fec_decode *g,
			  const uint8_t packet[FEC_PACKET_BYTES])
{
	uint16_t header = 0;
	uint8_t frag_index;
	size_t n;

	header |= packet[0];
	header |= packet[1] << 8;

	frag_index = (header >> 14) & 0x3;

	if (frag_index != g->frag_index)
		return -EADDRNOTAVAIL;

	n = header & GENMASK(13, 0);

	fec_decode_add_symbol(g, &packet[2], n);

	return 0;
}

bool fec_decode_symbol(struct fec_decode *g, uint8_t symbol[FEC_SYMBOL_SIZE])
{
	if (g->next_symbol >= FEC_SYMBOLS_PER_GENERATION)
		return false;

	if (bitmap_weight(g->parity[g->next_symbol].row,
			  FEC_SYMBOLS_PER_GENERATION) != 1)
		return false;

	/* output successfully decoded symbols */
	memcpy(symbol, g->data[g->next_symbol], FEC_SYMBOL_SIZE);
	g->next_symbol++;

	return true;
}
