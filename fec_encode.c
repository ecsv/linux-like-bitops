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

void fec_encode_init(struct fec_encode *g, uint8_t frag_index)
{
	memset(g, 0, sizeof(*g));

	g->frag_index = frag_index & 0x3;
}

int fec_encode_add_symbol(struct fec_encode *g,
			  const uint8_t symbol[FEC_SYMBOL_SIZE])
{
	if (g->next_write_symbol >= FEC_SYMBOLS_PER_GENERATION)
		return -ENOBUFS;

	/* when already having send a symbol, don't overwrite it */
	if (g->next_read_symbol > g->next_write_symbol)
		return -ENOBUFS;

	memcpy(g->data[g->next_write_symbol], symbol, FEC_SYMBOL_SIZE);

	g->next_write_symbol++;

	return 0;
}

static void fec_encode_fill_tx_buf(uint8_t packet[FEC_PACKET_BYTES],
				   const uint8_t symbol[FEC_SYMBOL_SIZE],
				   uint8_t frag_index, size_t n)
{
	uint16_t header = 0;

	header |= frag_index << 14;
	header |= n & GENMASK(13, 0);

	packet[0] = header;
	packet[1] = header >> 8;

	memcpy(&packet[2], symbol, FEC_SYMBOL_SIZE);
}

int fec_encode_get_packet(struct fec_encode *g, uint8_t packet[FEC_PACKET_BYTES])
{
	DECLARE_BITMAP(parity, FEC_SYMBOLS_PER_GENERATION);
	uint8_t symbol[FEC_SYMBOL_SIZE];
	size_t i;

	/* don't create packets when seq number would require more than 14 bits */
	if (g->next_read_symbol >= GENMASK(13, 0))
		return -ENODATA;

	/* get symbols to code for current sequence number (1 based) */
	fec_calculate_parity_row(parity, g->next_read_symbol + 1);

	/* combine symbols */
	memset(symbol, 0, FEC_SYMBOL_SIZE);
	for_each_set_bit(i, parity, FEC_SYMBOLS_PER_GENERATION)
		fec_xor_symbol(symbol, g->data[i]);

	fec_encode_fill_tx_buf(packet, symbol, g->frag_index,
			       g->next_read_symbol + 1);

	g->next_read_symbol++;

	return 0;
}
