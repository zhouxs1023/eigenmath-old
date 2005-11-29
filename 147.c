/* Error correction using the ATM code $x^8+x^2+x+1$. */

#include <stdio.h>

#include <stdlib.h>

int err1[256], err2[256];

main()

{

	int i, j, crc1, crc2;

	long b;

	/* single bit errors */

	for (i = 0; i < 40; i++) {

		b = (long) rand() << 16 | rand();

		crc1 = crc(b);

		if (i < 32)

			b ^= 1L << i;

		crc2 = crc(b);

		if (i >= 32)

			crc2 ^= 1 << (i - 32);

		err1[crc1 ^ crc2]++;

	}

	/* double bit errors */

	for (i = 0; i < 39; i++)

		for (j = i + 1; j < 40; j++) {

			b = (long) rand() << 16 | rand();

			crc1 = crc(b);

			if (i < 32)

				b ^= 1L << i;

			if (j < 32)

				b ^= 1L << j;

			crc2 = crc(b);

			if (i >= 32)

				crc2 ^= 1 << (i - 32);

			if (j >= 32)

				crc2 ^= 1 << (j - 32);

			err2[crc1 ^ crc2]++;

		}


	/* display */

	for (i = 0; i < 256; i++)

		printf("err1[%d]=%d err2[%d]=%d\n", i, err1[i], i, err2[i]);

}

crc(b)

long b;

{

	int i;

	for (i = 0; i < 32; i++)

		if (b & 0x80000000) {

			b <<= 1;

			b ^= 0x07000000;

		} else

			b <<= 1;

	return (int) (b >> 24) & 0xff;

}
