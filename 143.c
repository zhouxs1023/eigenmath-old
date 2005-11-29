/* Generate shifter table for 127 pattern. */

#include <stdio.h>

int index[128], value[128];

main()

{

	int b, i;

	b = 0xfe;

	/* scrambler values */

	for (i = 0; i < 127; i++) {

		value[i] = b & 0xff;

		b <<= 1;

		if ((b & 0x90) == 0x80 || (b & 0x90) == 0x10)

			b |= 1;

	}

	/* look-up table */

	for (i = 0; i < 127; i++)

		index[value[i] & 0x7f] = i;

	for (i = 0; i < 128; i++)

		printf("\tdb\t0%02xh,0%02xh\n", index[i], value[i]);

}
