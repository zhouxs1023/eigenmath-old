/* This program generates a 2047 pattern 6 bits at a time
using both table look-up and a scrambler. */

/*

1. This is the scrambler circuit:

  -------- + ------------------------------------------
 |         |                                           |
[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-


2. Now rotate the whole thing two bits to the left:

  ----------------------------------------------------------
 |                                            |             |
[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<- + <-[ ]<-[ ]<-


3. Now it's almost a divider.  We need to add one more exclusive-or:

  -----------------------------------------------------------
 |                                            |              |
[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<-[ ]<- + <-[ ]<-[ ]<- + <- 0


4. Scrambling is the same as polynomial division.
The divisor is x^11 + x^2 + 1.

              ----------------------------
100000000101 | ???????????000000000000...

The eleven question marks are the starting value. They can have
any value except zero.

*/

#include <stdio.h>

int f[64];

main()

{

	int b, i, j, s;

	/* build table for shifting 6 bits at a time */

	for (i = 0; i < 64; i++) {

		b = i << 5;	/* fill most significant six bits */

		for (j = 0; j < 6; j++) {

			b <<= 1;

			if (b & 0x800)

				b ^= 0x805;	/* x^11 + x^2 + 1 */

		}

		f[i] = b;

	}

	/* compare */

	b = 0x7ff;

	s = 0x7ff;

	for (i = 0; i < 4000; i++) {

		printf(".");

		/* scrambler */

		for (j = 0; j < 6; j++) {

			s <<= 1;

			if ((s & 0xa00) == 0x800 || (s & 0xa00) == 0x200)

				s |= 1;

		}

		s &= 0x7ff;

		/* table look-up */

		b = f[b >> 5] ^ (b << 6); /* see note below */

		b &= 0x7ff;

		if ((s & 0x3f) != (b >> 2 & 0x3f)) {

			printf("fail s=%02x b=%02x\n", s & 0x3f, b >> 2 & 0x3f);

			break;

		}

	}

}

/*

Note

Why f[b >> 5] ^ (b << 6)?

To shift six bits, divide the most significant 6 bits of "b"

by the polynomial.  This is equivalent to the following:

              -------------------
100000000101 | bbbbbb00000000000

But what about the least significant 5 bits?  They must be used

to correct the result.

                     rrrrrrrrrrr
              -------------------
100000000101 | bbbbbb00000000000
                     bbbbb

The second row of b's show where they should go.

They are at the top of the remainder.

  rrrrrrrrrrr
+ bbbbb
  -----------

To get to the top of the remainder shift left 6 bits.

*/
