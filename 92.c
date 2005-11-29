/* CCITT CRC-16 algorithm with least significant bit sent first. */

/* Notes on CCITT-16: */

/* 1. CRC is initialized to all ones per CCITT. */

/* 2. CRC is inverted before it is transmitted per CCITT. */

#include <stdio.h>

#include <stdlib.h>

int f[256];

main()

{

	int i, j;

	unsigned crc, t;

	/* generate table */

	for (i = 0; i < 256; i++) {

		crc = i;

		for (j = 0; j < 8; j++)

			if (crc & 1)

				crc = (crc >> 1) ^ 0x8408;

			else

				crc >>= 1;

		f[i] = crc;

	}

	/* check */

	randomize();

	crc = 0xffff;	/* per ccitt */

	for (i = 0; i < 1000; i++)

		crc = shift(crc, random(256));

	printf("~crc = %04x\n", ~crc);

	/* result should always be f0b8 per ccitt */

	t = ~crc;

	crc = shift(crc, t & 0xff);

	crc = shift(crc, (t >> 8) & 0xff);

	printf("remainder = %04x\n", crc);

	if (crc == 0xf0b8)

		printf("that is correct\n");

	else

		printf("that is incorrect\n");

}

shift(crc, d)

int crc, d;

{

	return f[(crc ^ d) & 0xff] ^ ((crc >> 8) & 0xff);

	/* if crc and d are unsigned you can used the following expression */

	/* return f[(crc ^ d) & 0xff] ^ (crc >> 8) */

}
