/* It appears that when computing Bonding CRC,
the result is the same when bit 8 is always 0 and
when bit 8 is always 1.
This programs tests this hypothesis. */

#include <stdio.h>

#include <stdlib.h>

int data[256];

int crc;

#define N 1

main()

{

	int i;

	randomize();

	for (i = 0; i < N; i++)

		data[i] = random(256);

	crc = 0;

	for (i = 0; i < N; i++)

		shift(data[i]);

	printf("crc = %02x, bit 8 = x\n", crc);

	crc = 0;

	for (i = 0; i < N; i++)

		shift(data[i] & 0xfe);

	printf("crc = %02x, bit 8 = 0\n", crc);

	crc = 0;

	for (i = 0; i < N; i++)

		shift(data[i] | 0x01);

	printf("crc = %02x, bit 8 = 1\n", crc);

}

shift(b)

int b;

{

	int i;

	crc ^= b;

	for (i = 0; i < 8; i++) {

		crc <<= 1;

		if (crc & 0x100)

			crc ^= 0x130;

	}

}
