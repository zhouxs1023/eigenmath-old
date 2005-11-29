/* Verify the algorithm for computing delay equalization. */

#include <stdio.h>

#include <stdlib.h>

main()

{

	randomize();

	for (;;) {

		test();

		printf(".");

		if (kbhit())

			break;

	}

}

test()

{

	int a[32], d[32], i, m, n, x;

	/* number of channels */

	n = random(32) + 1;

	/* generate random delay */

	for (i = 0; i < n; i++)

		d[i] = rand() & 0x1fff;		/* 13 bits */

	/* add a random offset and multiframe slip */

	x = rand();

	for (i = 0; i < n; i++)

		a[i] = (d[i] + x + i) & 0x3fff;

	/* compute equalization */

	reckon_equalizer_settings(n, a);

	/* find max delay */

	m = d[0];

	for (i = 1; i < n; i++)

		if (d[i] > m)

			m = d[i];

	/* verify equalizer settings */

	for (i = 0; i < n; i++)

		if (a[i] < -8191 || a[i] > 0 || d[i] - m != a[i])

			break;

	if (i != n) {

		printf("fail\n");

		printf("x = %d\n", x);

		printf("d[] =");

		for (i = 0; i < n; i++)

			printf(" %d", d[i]);

		printf("\n");

		printf("a[] =");

		for (i = 0; i < n; i++)

			printf(" %d", a[i]);

		printf("\n");

		exit(1);

	}

}

reckon_equalizer_settings(n, a)

int n, a[];

{

	int i, m;

	/* subtract multiframe channel slip */

	for (i = 0; i < n; i++)

		a[i] = (a[i] - i) & 0x3fff;

	/* find the smallest value */

	m = a[0];

	for (i = 1; i < n; i++)

		if (a[i] < m)

			m = a[i];

	/* subtract to normalize */

	for (i = 0; i < n; i++)

		a[i] -= m;

	/* extend sign */

	for (i = 0; i < n; i++)

		if (a[i] & 0x2000)

			a[i] |= 0xc000;

	/* normalize again */

	m = a[0];

	for (i = 1; i < n; i++)

		if (a[i] > m)

			m = a[i];

	for (i = 0; i < n; i++)

		a[i] -= m;

}
