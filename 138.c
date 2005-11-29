/* Generate 48 tables of 127 pattern. */

#include <stdio.h>

main()

{

	int i;

	printf("ycode segment code\n");

	printf(" rseg ycode\n");

	printf(" public y_table\n");

	printf("y_table:\n");

	for (i = 1; i <= 24; i++) {

		generate7(i);

		generate8(i);

	}

	printf(" end\n");

}

generate7(n)

int n;

{

	int a, i, j;

	printf("; shift %d x 7 bits\n", n);

	printf(" db 07fh\n");

	for (i = 1; i < 128; i++) {

		a = i;

		for (j = 0; j < n; j++)

			a = shift7(a);

		printf(" db %03xh\n", a);

	}

}

shift7(a)

int a;

{

	int i;

	for (i = 0; i < 7; i++) {

		if ((a & 9) == 8 || (a & 9) == 1)

			a |= 0x80;

		a >>= 1;

	}

	return a;

}

generate8(n)

int n;

{

	int a, i, j;

	printf("; shift %d x 8 bits\n", n);

	printf(" db 0feh\n");

	for (i = 1; i < 128; i++) {

		a = 2 * i;

		for (j = 0; j < n; j++)

			a = shift8(a);

		printf(" db %03xh\n", a);

	}

}

/*

This is for 64 kbps.

The temporal order of the bits is:

  t   t   t   t   t   t   t   t
   8   7   6   5   4   3   2   1

---------------------------------
|   |   |   |   |   |   |   |   |
---------------------------------

The rightmost bit is sent first.

This is why the taps are shifted left (0x12 instead of 0x09).

        ---------------------------------
------->|   |   |   |   |   |   |   |   |
|       ---------------------------------
|                     |           |
--------------------- + <----------

*/

shift8(a)

int a;

{

	int i;

	for (i = 0; i < 8; i++) {

		if ((a & 0x12) == 0x10 || (a & 0x12) == 0x02)

			a |= 0x100;

		a >>= 1;

	}

	return a;

}
