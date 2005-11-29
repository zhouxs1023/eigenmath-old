/* Yet another DSP algorithm for removing 7 bits at a time
from a 16 bit wide fifo. */

/* Alternate algorithm (see T1.C) that should be faster when implemented in */

/* a DSP with multifunction instructions. */

#include <stdio.h>

char *str = "Hello world! This is a long string to test all shifter combinations.";

unsigned int fifo[16];

main()
{
	int i, j = 0, n;

	/* DSP registers */

	unsigned int se; /* shifter exponent */

	unsigned int si; /* shifter input */

	unsigned long sr; /* shifter result */

	fill(str); /* initialize fifo */

	n = strlen(str); /* string length */

	se = 8;

	si = fifo[j++]; /* read fifo */

	for (i = 0; i < n; i++) {

		sr = (unsigned long) si << se; /* shift */

		if (se + 7 >= 24) { /* too few bits? */

			si = fifo[j++]; /* read fifo */

			se -= 16; /* subtract 23 bits */

			sr |= (unsigned long) si << se; /* shift */

		}

		se += 7; /* add seven bits */

		printf("%c", (char) (sr >> 17 & 0x7f));

	}
}

/* fill fifo 7 bits at a time */

fill(s)
char *s;
{
	int b, c, i, j = 0, k = 15;
	while (*s) {
		c = *s++;
		for (i = 6; i >= 0; i--) {
			b = c >> i & 1; /* get next bit */
			fifo[j] |= b << k; /* put next bit */
			if (--k < 0) {
				k = 15;
				j++;
			}
		}
	}
}
