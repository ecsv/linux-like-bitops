// SPDX-License-Identifier: MIT
/* FEC (systematic block code) test with fixed size symbols and fixed size
 * generation
 *
 * SPDX-FileCopyrightText: Sven Eckelmann <sven@narfation.org>
 */

#include "fec.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "bitops.h"
#include "fec_decode.h"
#include "fec_encode.h"
#include "fec.h"

#define DEBUG_OUTPUT

/* This is just a proof-of-concept. The actual code is in {decode,encode}.{c,h}
 *
 * see "LoRaWAN® Fragmented Data Block Transport Specification TS004-2.0.0" for
 * details.
 *
 * But this implementation was modified for devices which don't have any
 * flash to store the (un)coded symbols and have a very limited amount of
 * memory. Using a single generation for all symbols is beneficial for the
 * reliability (efficiency) of the recovery process (of lost symbols) but needs
 * a lot more CPU time and (much worse for us) memory.
 *
 * Instead of only having one generation of uncoded and coded symbols, it splits
 * up the sequence number range in multiple generations. One part of such a
 * sequence sub-range is just for uncoded packets and the rest of the sub-range
 * is for coded packets. If not the whole sub-range is used (by transmitting
 * coded packets), the missing sequence numbers are just skipped during the
 * transmission. For the decoder, this looks like lost coded symbols - which
 * are hopefully not needed for the recovery process.
 */

#define TEST_LENGTH 195
#define TEST_SYMBOL_SIZE 200U

static uint8_t input[TEST_LENGTH][TEST_SYMBOL_SIZE];
static uint8_t output[TEST_LENGTH][TEST_SYMBOL_SIZE];
static size_t output_pos;

#define TEST_FRAG_INDEX 1

static void print_symbol(const uint8_t symbol[])
{
	size_t i;

	for (i = 0; i < TEST_SYMBOL_SIZE; i++)
		printf("%02x", symbol[i]);
}

#ifdef DEBUG_OUTPUT
static void print_parity_row(const unsigned long *parity)
{
	size_t i;

	putc('|', stdout);
	for (i = 0; i < FEC_SYMBOLS_PER_GENERATION; i++)
		putc(test_bit(i, parity) ? '1' : ' ', stdout);

	putc('|', stdout);
}
#endif

static void create_input_fragments(void)
{
	size_t i;
	size_t j;

	for (i = 0; i < TEST_LENGTH; i++) {
		uint8_t *symbol = input[i];

		for (j = 0; j < TEST_SYMBOL_SIZE; j++)
			symbol[j] = rand();
	}
}

static void compare_input_output(void)
{
	size_t i;

	for (i = 0; i < TEST_LENGTH; i++) {
		if (memcmp(input[i], output[i], TEST_SYMBOL_SIZE) == 0)
			continue;

		printf("Found difference between symbols %zu: ", i);
		print_symbol(input[i]);
		printf(" ");
		print_symbol(output[i]);
		printf("\n");
	}
}

static int fill_encoder(struct fec_encode *encoder, size_t start_pos)
{
	size_t i;
	int ret;

	for (i = 0; i < FEC_SYMBOLS_PER_GENERATION && (i + start_pos) < TEST_LENGTH; i++) {
		ret = fec_encode_add_symbol(encoder, input[start_pos + i]);
		if (ret < 0)
			return ret;
	}

	return 0;
}

static int simulate_generation_transfer(struct fec_encode *encoder,
					struct fec_decode *decoder)
{
	uint8_t packet[TEST_SYMBOL_SIZE + FEC_HEADER_BYTES];
	size_t i;
	int ret;

	for (i = 0; i < FEC_TOTAL_SYMBOLS_PER_GENERATION; i++) {
		ret = fec_encode_get_packet(encoder, packet);
		if (ret < 0) {
			fprintf(stderr, "Failed to create packet\n");
			return ret;
		}

		/* simulate loss */
		if (random() % 10 == 0)
			continue;

		ret = fec_decode_add_packet(decoder, packet);
		if (ret < 0) {
			fprintf(stderr, "Failed to add packet to decoder\n");
			return ret;
		}

		while (output_pos < TEST_LENGTH) {
			if (!fec_decode_symbol(decoder, output[output_pos]))
				break;

			output_pos++;
		}
	}

#ifdef DEBUG_OUTPUT
	/* check the decoder state */
	if (decoder->next_symbol != FEC_SYMBOLS_PER_GENERATION &&
	    output_pos < TEST_LENGTH) {
		for (i = 0; i < FEC_SYMBOLS_PER_GENERATION; i++) {
			printf("Decoder symbol %zu: ", i);
			print_parity_row(decoder->parity[i].row);
			printf(" ");
			print_symbol(decoder->data[i]);
			printf("\n");
		}
	}
#endif

	return 0;
}

static void simulate_block_transfer(void)
{
	struct fec_decode decoder;
	struct fec_encode encoder;
	size_t i;
	int ret;

	/* sender */
	fec_encode_init(&encoder, TEST_FRAG_INDEX, TEST_SYMBOL_SIZE);

	/* receiver */
	fec_decode_init(&decoder, TEST_FRAG_INDEX, TEST_SYMBOL_SIZE);

	for (i = 0; i < TEST_LENGTH; i += FEC_SYMBOLS_PER_GENERATION) {
		ret = fec_encode_start_generation(&encoder);
		if (ret < 0) {
			fprintf(stderr, "Failed to start encoder generation\n");
			break;
		}

		ret = fill_encoder(&encoder, i);
		if (ret < 0) {
			fprintf(stderr, "Failed to fill encoder\n");
			break;
		}

		ret = simulate_generation_transfer(&encoder, &decoder);
		if (ret < 0)
			break;
	}

	fec_decode_destroy(&decoder);
	fec_encode_destroy(&encoder);
}

int main(void)
{
	srand(time(NULL) + getpid());

	/* create our dummy input */
	create_input_fragments();

	/* simulate a transmission - will write to the output buffer */
	simulate_block_transfer();

	/* check if everything was received correctly */
	compare_input_output();

	return 0;
}
