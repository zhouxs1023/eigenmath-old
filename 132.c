/* Generate table for DDS frame alignment. */

#include <stdio.h>

char s[100];

main()

{

	int b, i, j;

	for (i = 0; i < 64; i++) {

		b = i;

		for (j = 0; j < 6; j++) {

			if (b == 0x2c)

				break;

			b <<= 1;

			if (b & 0x40)

				b ^= 0x41;

		}

		if (j < 6) {

			binary(i);

			printf("\tdb\t1\t; %s\n", s);

		} else

			printf("\tdb\t0\n", i + 1);

	}

}

binary(n)

int n;

{

	int i;

	for (i = 0; i < 6; i++)

		if (n & 1 << (5 - i))

			s[i] = '1';

		else

			s[i] = '0';

	s[6] = 0;

}
