/* Generate scrambler tables for 56 and 64 kbps. */

#include <stdio.h>

main()

{

	int b, i, j;

	/* 56 kbps */

	printf(" db 07fh\n\n");

	for (i = 1; i < 128; i++) {

		b = i;

		for (j = 0; j < 7; j++) {

			b <<= 1;

			if ((b & 0x90) == 0x80 || (b & 0x90) == 0x10)

				b |= 1;

		}

		printf(" db %03xh\n\n", b & 0x7f);

	}

	/* 64 kbps */

	printf(" db 07fh\n\n");

	for (i = 1; i < 128; i++) {

		b = i;

		for (j = 0; j < 8; j++) {

			b <<= 1;

			if ((b & 0x90) == 0x80 || (b & 0x90) == 0x10)

				b |= 1;

		}

		printf(" db %03xh\n\n", b & 0xff);

	}

}
