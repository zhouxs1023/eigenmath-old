/* Generate table for the sequence $x^7+x^6+x^5+1$. */

#include <stdio.h>

main()

{

	int b, i, n, s, t;

	for (b = 0; b < 256; b++) {

		s = b;

		for (i = 0; i < 8; i++) {

			n = 0;

			if (s & 0x80) n++;

			if (s & 0x40) n++;

			if (s & 0x20) n++;

			if (s & 0x01) n++;

			s <<= 1;

			if (n & 1)

				s |= 1;

		}

		s &= 0xff;

		t = b;

		for (i = 0; i < 8; i++) {

			t <<= 1;

			if (t & 0x100)

				t ^= 0x1e1;

		}

		if (s != t)

			printf("error ");

		printf("%d\n", s);

	}

}
