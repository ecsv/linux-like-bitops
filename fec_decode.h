/* SPDX-License-Identifier: MIT */
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#ifndef __FEC_DECODE_H__
#define __FEC_DECODE_H__

#include "fec.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "bitops.h"

struct fec_decode {
	/** @parity: Control/parity matrix for each symbol */
	struct {
		DECLARE_BITMAP(row, FEC_SYMBOLS_PER_GENERATION);
	} parity[FEC_SYMBOLS_PER_GENERATION];

	/** @data: (un)coded symbols. see control matrix for state */
	uint8_t *data[FEC_SYMBOLS_PER_GENERATION];

	/** @symbol_buffer: buffer storing data for @data */
	uint8_t *symbol_buffer;

	/** @next_symbol: next symbol which needs to be finished */
	size_t next_symbol;

	/** @frag_index: fragmentation index for the transmission */
	uint8_t frag_index;

	/** @symbol_size: size of symbol in @data/@symbol_buffer */
	size_t symbol_size;

	/** @generation_seqno: first sequence number of the current generation */
	uint16_t generation_seqno;

	/**
	 * @maximum_symbols: maximum symbols the decoder is allowed to decode
	 *  Can be set to 0 to decode all decodeable symbols
	 */
	uint16_t max_symbols;
};

int fec_decode_init(struct fec_decode *g, uint8_t frag_index,
		    size_t symbol_size, uint16_t max_symbols);
void fec_decode_destroy(struct fec_decode *g);
int fec_decode_add_packet(struct fec_decode *g,
			  const uint8_t *packet);
bool fec_decode_symbol(struct fec_decode *g, uint8_t *symbol);

void fec_calculate_parity_row(unsigned long *row, unsigned long symbol_no);
void fec_xor_symbol(uint8_t *dst, const uint8_t *src, size_t symbol_size);

#endif /* __FEC_DECODE_H__ */
