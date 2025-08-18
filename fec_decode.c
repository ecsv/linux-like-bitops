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

void fec_xor_symbol(uint8_t *dst, const uint8_t *src, size_t symbol_size)
{
	size_t i;

	for (i = 0; i < symbol_size; i++)
		dst[i] ^= src[i];
}

int fec_decode_init(struct fec_decode *g, uint8_t frag_index,
		    size_t symbol_size)
{
	size_t i;

	memset(g, 0, sizeof(*g));

	g->frag_index = frag_index & 0x3;
	g->symbol_size = symbol_size;

	/* allocate memory for all receive data + an extra one for temporary
	 * storage during fec_decode_add_symbol()
	 */
	g->symbol_buffer = calloc(FEC_SYMBOLS_PER_GENERATION + 1,
				  g->symbol_size);
	if (!g->symbol_size)
		return -ENOMEM;

	for (i = 0; i < FEC_SYMBOLS_PER_GENERATION; i++)
		g->data[i] = &g->symbol_buffer[(1 + i) * g->symbol_size];

	return 0;
}

void fec_decode_destroy(struct fec_decode *g)
{
	free(g->symbol_buffer);
}

static void fec_decode_add_symbol(struct fec_decode *g,
				  const uint8_t *symbol, size_t n)
{
	DECLARE_BITMAP(parity, FEC_SYMBOLS_PER_GENERATION);
	uint8_t *s = g->symbol_buffer;
	size_t end;
	size_t pos;
	size_t i;

	memcpy(s, symbol, g->symbol_size);
	fec_calculate_parity_row(parity, n);

	/* gaussian elimination - forward elimination */
	for_each_set_bit(i, parity, FEC_SYMBOLS_PER_GENERATION) {
		if (!test_bit(i, g->parity[i].row))
			continue;

		bitmap_xor(parity, parity, g->parity[i].row, FEC_SYMBOLS_PER_GENERATION);
		fec_xor_symbol(s, g->data[i], g->symbol_size);
	}

	if (bitmap_weight(parity, FEC_SYMBOLS_PER_GENERATION) == 0)
		return;

	/* save to position based on least significant set bit */
	pos = find_first_bit(parity, FEC_SYMBOLS_PER_GENERATION);
	bitmap_copy(g->parity[pos].row, parity, FEC_SYMBOLS_PER_GENERATION);
	memcpy(g->data[pos], s, g->symbol_size);

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
			fec_xor_symbol(g->data[i], g->data[pos], g->symbol_size);
		}
	}
}

static int fec_decode_start_generation(struct fec_decode *g)
{
	if (g->generation_seqno == 0) {
		/* for unknown reasons, the fragmentation seqno starts at 1
		 * and not at 0
		 */
		g->generation_seqno = 1;
		return 0;
	}

	/* a generation must not use sequence numbers which require more than
	 * 14 bit (due to limitations in the packet header representation)
	 */
	if (g->generation_seqno + FEC_SEQNO_PER_GENERATION * 2 > GENMASK(13, 0))
		return -ERANGE;

	g->generation_seqno += FEC_SEQNO_PER_GENERATION;
	g->next_symbol = 0;

	memset(g->symbol_buffer, 0,
	       g->symbol_size * (FEC_SYMBOLS_PER_GENERATION + 1));
	memset(g->parity, 0, sizeof(g->parity));

	return 0;
}

int fec_decode_add_packet(struct fec_decode *g,
			  const uint8_t *packet)
{
	uint16_t header = 0;
	uint8_t frag_index;
	size_t n;
	int ret;

	header |= packet[0];
	header |= packet[1] << 8;

	frag_index = (header >> 14) & 0x3;

	if (frag_index != g->frag_index)
		return -EADDRNOTAVAIL;

	n = header & GENMASK(13, 0);

	/* sequence numbers are 1 based. seqno 0 is therefore invalid */
	if (n == 0)
		return -ERANGE;

	/* first symobol, just start the first generation */
	if (g->generation_seqno == 0) {
		ret = fec_decode_start_generation(g);
		if (ret < 0)
			return ret;
	}

	/* packet from previous generation - just ignore it */
	if (n < g->generation_seqno)
		return 0;

	/* there was at least one generation skipped and we can't recover this */
	if (n >= g->generation_seqno + FEC_SEQNO_PER_GENERATION * 2)
		return -ERANGE;

	/* the next generation was started */
	if (n >= g->generation_seqno + FEC_SEQNO_PER_GENERATION) {
		/* but old generation could not be fully decoded yet */
		if (g->next_symbol != FEC_SYMBOLS_PER_GENERATION)
			return -EIO;

		fec_decode_start_generation(g);
	}

	/* add symbol for decoding - but with generation (relative) sequence
	 * number and not fragment/packet sequence number
	 */
	fec_decode_add_symbol(g, &packet[2], n - g->generation_seqno + 1);

	return 0;
}

bool fec_decode_symbol(struct fec_decode *g, uint8_t *symbol)
{
	if (g->next_symbol >= FEC_SYMBOLS_PER_GENERATION)
		return false;

	if (bitmap_weight(g->parity[g->next_symbol].row,
			  FEC_SYMBOLS_PER_GENERATION) != 1)
		return false;

	/* output successfully decoded symbols */
	memcpy(symbol, g->data[g->next_symbol], g->symbol_size);
	g->next_symbol++;

	return true;
}
