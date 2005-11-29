/* Generate look up table for ANSI loop back pattern.
The table index is the starting pattern and the
table element is the result of shifting eight times. */

#include <stdio.h>

main()

{

	int b, i, j;

	for (i = 0; i < 128; i++) {

		b = i;

		for (j = 0; j < 8; j++) {

			b <<= 1;

			if ((b & 0x90) == 0x80 || (b & 0x90) == 0x10)

				b |= 1;

		}

		printf("\tdb\t%d\n", b & 0xff);

	}

}
