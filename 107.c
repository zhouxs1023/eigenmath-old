/* This program makes a time slot mask look up table. */

#include <stdio.h>

main()
{
	int i, j;

	for (i = 0; i < 32; i++) {

		printf("; bandwidth = %d\n", i);

		for (j = 0; j < 32; j++)
			f(0, i, j);
	}

	for (i = 0; i < 32; i++) {

		printf("; bandwidth = %d\n", i);

		for (j = 0; j < 32; j++)
			f(1, i, j);
	}
}

int ts[32];

/*
	u		use time slot 16

	n		number of time slots

	b		base time slot
*/

f(u, n, b)
int u, n, b;
{
	int i, x;

	/* clear */

	for (i = 0; i < 32; i++)
		ts[i] = 0;

	/* flag time slots */

	x = b;

	for (i = 0; i < n; i++) {

		if (x == 0)
			x++;

		if (x == 16 && u == 0)
			x++;

		ts[x] = 1;

		x = (x + 1) % 32;
	}

	/* print masks */

	printf(" db ");

	for (i = 0; i < 8; i++)
		printf("%d", ts[31 - i]);

	printf("b,");

	for (i = 0; i < 8; i++)
		printf("%d", ts[23 - i]);

	printf("b,");

	for (i = 0; i < 8; i++)
		printf("%d", ts[15 - i]);

	printf("b,");

	for (i = 0; i < 8; i++)
		printf("%d", ts[7 - i]);

	printf("b\n");
}
