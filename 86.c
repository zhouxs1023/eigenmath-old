/* $N$ is the number of octets.  For what $N$ does the Bonding CRC yield
the same value when bit 8 is always 0 and when bit 8
is always 1?  This program provides the answer: when $N$ is
a multiple of 15. */

#include <stdio.h>

#include <stdlib.h>

int data[256];

int crc0;

int crc1;

#define N 1

main()

{

	int i;

	randomize();

	for (i = 1; i < 256; i++)

		test(i);

}

test(n)

int n;

{

	int i, j, b;

	for (i = 0; i < 10; i++) {

		crc0 = 0;

		crc1 = 0;

		for (j = 0; j < n; j++) {

			b = random(256);

			shift0(b);

			shift1(b);

		}

		if (crc0 != crc1)

			return;

	}

	printf("n = %d\n", n);

}

shift0(b)

int b;

{

	int i;

	b &= 0xfe;

	crc0 ^= b;

	for (i = 0; i < 8; i++) {

		crc0 <<= 1;

		if (crc0 & 0x100)

			crc0 ^= 0x130;

	}

}

shift1(b)

int b;

{

	int i;

	b |= 0x01;

	crc1 ^= b;

	for (i = 0; i < 8; i++) {

		crc1 <<= 1;

		if (crc1 & 0x100)

			crc1 ^= 0x130;

	}

}
