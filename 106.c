/* Create a table for counting bits in a byte. */

#include <stdio.h>

main()
{
	int i, j, m, n;

	for (i = 0; i < 256; i++) {

		m = i;

		n = 0;

		for (j = 0; j < 8; j++) {

			if (m & 1)
				n++;

			m >>= 1;
		}

		printf(" db %d\n", n);
	}
}
