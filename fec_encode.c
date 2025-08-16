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

int fec_encode_start_generation(struct fec_encode *g)
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
	g->next_read_symbol = 0;
	g->next_write_symbol = 0;

	memset(g->data, 0, sizeof(g->data));

	return 0;
}

int fec_encode_get_packet(struct fec_encode *g, uint8_t packet[FEC_PACKET_BYTES])
{
	DECLARE_BITMAP(parity, FEC_SYMBOLS_PER_GENERATION);
	uint8_t symbol[FEC_SYMBOL_SIZE];
	size_t i;

	/* don't create packets outside the generation seqno range */
	if (g->next_read_symbol >= FEC_SEQNO_PER_GENERATION)
		return -ENODATA;

	/* get symbols to code for current position (1 based) in generation */
	fec_calculate_parity_row(parity, g->next_read_symbol + 1);

	/* combine symbols */
	memset(symbol, 0, FEC_SYMBOL_SIZE);
	for_each_set_bit(i, parity, FEC_SYMBOLS_PER_GENERATION)
		fec_xor_symbol(symbol, g->data[i]);

	fec_encode_fill_tx_buf(packet, symbol, g->frag_index,
			       g->generation_seqno + g->next_read_symbol);

	g->next_read_symbol++;

	return 0;
}
