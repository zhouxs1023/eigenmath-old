/* Simulate the algorithm for finding BONDING full bandwidth */
/* information channel alignment. The information channel may */
/* not be byte aligned. The information message is 16 bytes long. */
/* This simulation tries all 128 possible starting bit positions */
/* and verifies that the algorithm can find alignment. */

#include <stdio.h>

unsigned char buf[32] = {
	0x7f, 0x81, 0x81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0x7f, 0x81, 0x81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

int bit_offset, byte_offset;

main()
{
	int i, j;

	for (i = 0; i < 16; i++)
		for (j = 0; j < 8; j++) {

			if (find_alignment() == 0) {
				printf("failed to find alignment\n");
				printf("i=%d j=%d\n", i, j);
				exit();
			}

			if (i == byte_offset && j == bit_offset)
				;
			else {
				printf("wrong alignment\n");
				printf("i=%d byte_offset=%d\n", i, byte_offset);
				printf("j=%d bit_offset=%d\n", j, bit_offset);
				exit();
			}

			printf(".");

			rotate();
		}

	printf("simulation succeeded\n");
}

/* scan the message buffer looking for the sync pattern 0x7f81 */

/* the pattern may not be byte aligned */

find_alignment()
{
	int i, j;
	unsigned long b;

	/* get the first sixteen bits */

	b = (unsigned long) buf[0] << 16 & 0xff0000;

	b |= (unsigned long) buf[1] << 8 & 0xff00;

	/* scan the first half of the buffer */

	/* 16 x 8 = 128 bits */

	/* the sync pattern may cross the midpoint of the buffer */

	/* when the "for" loop ends, b = buf[16]:buf[17] */

	/* therefore the entire first half of the buffer has been scanned */

	for (i = 0; i < 16; i++) {

		/* get the next eight bits */

		b |= (unsigned long) buf[i + 2] & 0xff;

		/* shift eight bits to find sync pattern */

		for (j = 0; j < 8; j++) {

			/* sync pattern? */

			if ((b & 0xffff00) == 0x7f8100) {

				/* save location of sync pattern */

				byte_offset = i;
				bit_offset = j;

				/* alignment found */

				return 1;
			}

			/* shift */

			b <<= 1;
		}
	}

	/* no alignment found */

	return 0;
}

/* 256 bit rotate */

rotate()
{
	int i, t;

	t = buf[31];

	for (i = 31; i > 0; i--) {
		buf[i] >>= 1;
		if (buf[i - 1] & 1)
			buf[i] |= 0x80;
	}

	buf[0] >>= 1;
	if (t & 1)
		buf[0] |= 0x80;
}
