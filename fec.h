/* SPDX-License-Identifier: MIT */
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#ifndef __FEC_H__
#define __FEC_H__

#define FEC_SYMBOLS_PER_GENERATION 32U
#define FEC_CODED_SYMBOLS_PER_GENERATION (FEC_SYMBOLS_PER_GENERATION / 2)
#define FEC_TOTAL_SYMBOLS_PER_GENERATION (FEC_SYMBOLS_PER_GENERATION + \
	FEC_CODED_SYMBOLS_PER_GENERATION)
#define FEC_SEQNO_PER_GENERATION (FEC_SYMBOLS_PER_GENERATION * 2)

#define FEC_HEADER_BYTES 2U

#endif /* __FEC_H__ */
