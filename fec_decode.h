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
	uint8_t data[FEC_SYMBOLS_PER_GENERATION][FEC_SYMBOL_SIZE];

	/** @next_symbol: next symbol which needs to be finished */
	size_t next_symbol;

	/** @frag_index: fragmentation index for the transmission */
	uint8_t frag_index;

	/** @generation_seqno: first sequence number of the current generation */
	uint16_t generation_seqno;
};

void fec_decode_init(struct fec_decode *g, uint8_t frag_index);
int fec_decode_add_packet(struct fec_decode *g,
			  const uint8_t packet[FEC_PACKET_BYTES]);
bool fec_decode_symbol(struct fec_decode *g, uint8_t symbol[FEC_SYMBOL_SIZE]);

void fec_calculate_parity_row(unsigned long *row, unsigned long symbol_no);
void fec_xor_symbol(uint8_t dst[FEC_SYMBOL_SIZE], const uint8_t src[FEC_SYMBOL_SIZE]);

#endif /* __FEC_DECODE_H__ */
