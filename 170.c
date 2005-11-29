/* This program generates a stimulus file to be used while running the DSP */
/* packet receiver code on a simulator. */
/* The output is a sequence of bytes that simulates random packet traffic */
/* in a time slot. */
/* Least significant bit is the first bit sent and received. */
/* This program is derived from program number 165. */

#include <stdio.h>

#include <stdlib.h>

int n_ones, crc;

int crc_table[256];

int n_frames;

main()

{

	int i;

	make_crc_table();

	send_flag();

	send_flag();

	send_flag();

	send_flag();

	send_packet(20);	/* long packet */

	for (;;) {

		send_packet(random(6) + 2);

		if (n_frames > 2000)

		break;

	}

	send_flag();

	send_flag();

	send_flag();

	send_flag();

}

send_packet(n)

int n;

{

	int i;

	crc = 0xffff;

	for (i = 0; i < n; i++)

		send_byte(random(256));

	/* send crc */

	n = ~crc;

	send_byte(n & 0xff);

	send_byte((n >> 8) & 0xff);

	/* send random number of closing flags */

	n = random(4) + 1;

	for (i = 0; i < n; i++)

		send_flag();

}

send_byte(d)

int d;

{

	int i;

	fprintf(stderr, " %02x ", d);

	shift(d);

	for (i = 0; i < 8; i++) {

		send_bit(d & 1);

		d >>= 1;

		if (n_ones == 5)

			send_bit(0);

	}

}

send_flag()

{

	fprintf(stderr, " ** ");

	send_bit(0);

	send_bit(1);

	send_bit(1);

	send_bit(1);

	send_bit(1);

	send_bit(1);

	send_bit(1);

	send_bit(0);

}

int bit_queue, bit_count;

send_bit(d)

int d;

{

	bit_queue <<= 1;

	bit_queue &= 0xff;

	if (d)

		bit_queue |= 1;

	if (++bit_count == 8) {

		bit_count = 0;

		printf("%04x\n", bit_queue);

		n_frames++;

	}

	if (d == 1)

		n_ones++;

	else

		n_ones = 0;

}

make_crc_table()

{

	int crc, i, j;

	for (i = 0; i < 256; i++) {

		crc = i;

		for (j = 0; j < 8; j++)

			if (crc & 1)

				crc = ((crc >> 1) & 0x7fff) ^ 0x8408;

			else

				crc = (crc >> 1) & 0x7fff;

		crc_table[i] = crc;

	}

}

shift(d)

int d;

{

	crc = crc_table[(crc ^ d) & 0xff] ^ ((crc >> 8) & 0xff);

}
