// SPDX-License-Identifier: MIT
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "fec_encode.h"

#include <errno.h>
#include <stdint.h>
#include <string.h>

#include "bitops.h"
#include "fec_decode.h"
#include "fec.h"

int fec_encode_init(struct fec_encode *g, uint8_t frag_index,
		    size_t symbol_size, uint16_t max_symbols)
{
	size_t i;

	memset(g, 0, sizeof(*g));

	g->frag_index = frag_index & 0x3;
	g->symbol_size = symbol_size;
	g->max_symbols = max_symbols;

	/* allocate memory for all receive data + an extra one for temporary
	 * storage during fec_encode_get_packet()
	 */
	g->symbol_buffer = calloc(FEC_SYMBOLS_PER_GENERATION + 1,
				  g->symbol_size);
	if (!g->symbol_size)
		return -ENOMEM;

	for (i = 0; i < FEC_SYMBOLS_PER_GENERATION; i++)
		g->data[i] = &g->symbol_buffer[(1 + i) * g->symbol_size];

	return 0;
}

void fec_encode_destroy(struct fec_encode *g)
{
	free(g->symbol_buffer);
}

static uint16_t fec_encode_first_symbol_of_generation(const struct fec_encode *g)
{
	uint16_t generation;

	if (g->generation_seqno == 0)
		return 0;

	generation = (g->generation_seqno - 1) / FEC_SEQNO_PER_GENERATION;

	return generation * FEC_SYMBOLS_PER_GENERATION;
}

int fec_encode_add_symbol(struct fec_encode *g,
			  const uint8_t *symbol)
{
	if (g->next_write_symbol >= FEC_SYMBOLS_PER_GENERATION)
		return -ENOBUFS;

	/* already saved all symbols which were requested,
	 * stop to save more
	 */
	if (g->max_symbols != 0) {
		uint16_t symbol_seq;

		symbol_seq = fec_encode_first_symbol_of_generation(g);
		symbol_seq += g->next_write_symbol;

		if (symbol_seq >= g->max_symbols)
			return -ENOBUFS;
	}

	/* when already having send a symbol, don't overwrite it */
	if (g->next_read_symbol > g->next_write_symbol)
		return -ENOBUFS;

	memcpy(g->data[g->next_write_symbol], symbol, g->symbol_size);

	g->next_write_symbol++;

	return 0;
}

static void fec_encode_fill_tx_buf(uint8_t *packet,
				   const uint8_t *symbol,
				   uint8_t frag_index, size_t symbol_size,
				   size_t n)
{
	uint16_t header = 0;

	header |= frag_index << 14;
	header |= n & GENMASK(13, 0);

	packet[0] = header;
	packet[1] = header >> 8;

	memcpy(&packet[2], symbol, symbol_size);
}

int fec_encode_start_generation(struct fec_encode *g)
{
	if (g->generation_seqno == 0) {
		/* for unknown reasons, the fragmentation seqno starts at 1
		 * and not at 0
		 */
		g->generation_seqno = 1;
	} else {
		/* a generation must not use sequence numbers which require more
		 * than 14 bit (due to limitations in the packet header
		 * representation)
		 */
		if (g->generation_seqno + FEC_SEQNO_PER_GENERATION * 2 > GENMASK(13, 0))
			return -ERANGE;

		/* don't start new generation when already submitted all symbols */
		if (g->max_symbols != 0) {
			uint16_t symbol_seqno;

			symbol_seqno = fec_encode_first_symbol_of_generation(g);
			symbol_seqno += FEC_SYMBOLS_PER_GENERATION;

			if (symbol_seqno >= g->max_symbols)
				return -ERANGE;
		}

		g->generation_seqno += FEC_SEQNO_PER_GENERATION;
	}

	g->next_read_symbol = 0;
	g->next_write_symbol = 0;

	memset(g->symbol_buffer, 0,
	       g->symbol_size * (FEC_SYMBOLS_PER_GENERATION + 1));

	return 0;
}

static uint16_t fec_encode_skip_padded_uncoded_symbols(const struct fec_encode *g)
{
	uint16_t generation_first_symbol;

	generation_first_symbol = fec_encode_first_symbol_of_generation(g);

	/* no padding until (at least) next generation) */
	if (g->max_symbols >= generation_first_symbol + FEC_SYMBOLS_PER_GENERATION)
		return 0;

	/* not yet reached padded symbol range of (last) generation */
	if (g->max_symbols > generation_first_symbol + g->next_read_symbol)
		return 0;

	/* next symbol is coded -> so already outside of padded symbol range */
	if (g->next_read_symbol >= FEC_SYMBOLS_PER_GENERATION)
		return 0;

	return FEC_SYMBOLS_PER_GENERATION - g->next_read_symbol;
}

int fec_encode_get_packet(struct fec_encode *g, uint8_t *packet)
{
	DECLARE_BITMAP(parity, FEC_SYMBOLS_PER_GENERATION);
	uint8_t *symbol = g->symbol_buffer;
	size_t i;

	/* don't create packets outside the generation seqno range */
	if (g->next_read_symbol >= FEC_SEQNO_PER_GENERATION)
		return -ENODATA;

	/* skip uncoded symbols which are null-padded because outside of
	 * "allocated" max uncoded symbols of encoder
	 */
	if (g->max_symbols != 0)
		g->next_read_symbol += fec_encode_skip_padded_uncoded_symbols(g);

	/* get symbols to code for current position (1 based) in generation */
	fec_calculate_parity_row(parity, g->next_read_symbol + 1);

	/* combine symbols */
	memset(symbol, 0, g->symbol_size);
	for_each_set_bit(i, parity, FEC_SYMBOLS_PER_GENERATION)
		fec_xor_symbol(symbol, g->data[i], g->symbol_size);

	fec_encode_fill_tx_buf(packet, symbol, g->frag_index, g->symbol_size,
			       g->generation_seqno + g->next_read_symbol);

	g->next_read_symbol++;

	return 0;
}
