/* Print a generator table for the sequence $x^8+x^7+x^2+x^1+1$. */

#include <stdio.h>

int p[256], t[256];

main()

{

	int d, i, r;

	for (d = 0; d < 256; d++) {

		/* divisor method */

		r = d;

		for (i = 0; i < 8; i++) {

			r <<= 1;

			/* if remainder is big enough then subtract */

			if (r & 0x100)

				r ^= 0x187;

		}

		printf("%d ", r);

		p[d] = r;

	}

	printf("\n");

	/* test */

	d = 1;

	for (i = 0; i < 255; i++) {

		t[d]++;

		d = p[d];

	}

	for (i = 1; i < 256; i++)

		if (t[i] != 1)

			printf("fail\n");

}
