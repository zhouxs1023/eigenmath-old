/* Experimental Bonding frame search algorithm. */

#include <stdio.h>

/* reference clock in thousands */

#define F 10752

main()

{

	int i;

	for (i = 1; i < 25; i++) {

		f(56 * i);

		f(64 * i);

	}

}

f(f)

int f;

{

	int d, i, k, m;

	printf("f = %d, n = ", f);

	f = 2 * f;

	m = F / f;

	k = m;

	printf("%d ", m);

	for (i = 1; i < 200; i++) {

		/* total period of ref clock = k / F */

		/* total period of dte clock = i / f */

		/* choose counter to make equal */

		d = (long) F * i - (long) f * k;

		if (d == 0)

			break;

		if (d < 0) {

			k += m;

			printf("%d ", m);

		}

		if (d > 0) {

			k += m + 1;

			printf("%d ", m + 1);

		}

	}

	if (i < 200)

		printf("i = %d\n", i);

	else

		printf("i = %d fail\n", i);

}
