/* This program generates a 2047 pattern 7 bits at a time. */

#include <stdio.h>

main()

{

	int b, i, j, s, s2;

	/* compare */

	b = 1;

	s = 1;

	for (i = 0; i < 4000; i++) {

		printf(".");

		/* scrambler */

		for (j = 0; j < 7; j++) {

			s <<= 1;

			if ((s & 0xa00) == 0x800 || (s & 0xa00) == 0x200)

				s |= 1;

		}

		/* divider */

		for (j = 0; j < 7; j++) {

			b <<= 1;

			if (b & 0x800)

				b ^= 0x805;

		}

		/* rotate */

		s2 = s;

		s2 <<= 1;

		if (s2 && 0x800)

			s2 |= 1;

		s2 <<= 1;

		if (s2 && 0x800)

			s2 |= 1;

		s2 &= 0x7ff;

		if (s2 != b) {

			printf("fail s2=%03x b=%03x\n", s2, b);

			break;

		}

	}

}
