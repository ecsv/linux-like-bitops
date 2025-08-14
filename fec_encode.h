/* SPDX-License-Identifier: MIT */
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#ifndef __FEC_ENCODE_H__
#define __FEC_ENCODE_H__

#include "fec.h"

#include <stdlib.h>
#include <stdint.h>

struct fec_encode {
	/** @data: (un)coded symbols */
	uint8_t data[FEC_SYMBOLS_PER_GENERATION][FEC_SYMBOL_SIZE];

	/** @next_write_symbol: next symbol to write */
	size_t next_write_symbol;

	/** @next_read_symbol: next symbol to read */
	size_t next_read_symbol;

	/** @frag_index: fragmentation index for the transmission */
	uint8_t frag_index;
};

void fec_encode_init(struct fec_encode *g, uint8_t frag_index);
int fec_encode_add_symbol(struct fec_encode *g,
			  const uint8_t symbol[FEC_SYMBOL_SIZE]);

int fec_encode_get_packet(struct fec_encode *g, uint8_t packet[FEC_PACKET_BYTES]);

#endif /* __FEC_ENCODE_H__ */
