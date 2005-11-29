/* Generate CRC-4 table for Bonding.
The generator polynomial is $x^4+x+1$.
This algorithm places the CRC result
in the most signficant four bits of a byte. */

#include <stdio.h>

main()

{

	int b, i, j;

	for (i = 0; i < 256; i++) {

		b = i;

		for (j = 0; j < 8; j++) {

			b <<= 1;

			if (b & 0x100)

				b ^= 0x130;

		}

		printf("%04x\n", b);

	}

}
