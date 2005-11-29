/* Simulate a DSP algorithm for removing seven bits at a time from a */
/* 16 bit wide fifo. */

#include <stdio.h>

char *str = "Hello world! This is a long string to test all shifter combinations.";

unsigned int fifo[16];

main()
{
	int i, j = 0, n;

	/* DSP registers */

	unsigned int se = 0; /* shifter exponent */

	unsigned int si; /* shifter input */

	unsigned long sr; /* shifter result */

	fill(str); /* initialize fifo */

	n = strlen(str); /* string length */

	si = fifo[j++]; /* read fifo */

	for (i = 0; i < n; i++) {

		se += 7; /* add seven bits */

		sr = (unsigned long) si << se; /* shift */

		if (se >= 16) { /* too few bits? */

			si = fifo[j++]; /* read fifo */

			se -= 16; /* subtract 16 bits */

			sr |= (unsigned long) si << se; /* shift */

		}

		printf("%c", (char) (sr >> 16 & 0x7f));

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
