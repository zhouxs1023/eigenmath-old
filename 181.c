/* Simulate a photon using a ``step by step'' algorithm.
Feynman says that the probability for a photon to go from A
to B is P(A to B). Try the following experiment.
A photon can take a step by either
going up (time) or going right (space). The probability is 50/50 on
which way. Take 10,000 steps and see where the photon ends up. */

#include <stdio.h>

#include <stdlib.h>

int a[401];

main()

{

	int i, j;

	for (i = 0; i < 1000; i++) {

		f();

		if (kbhit())

			break;

	}

	for (i = 0; i < 401; i++) {

		printf("%5d %5d .", i - 200, a[i]);

		for (j = 0; j < a[i]; j++)

			printf(".");

		printf("\n");

	}

}

#define O 5000

f()

{

	int i, j;

	long x = 0, t = 0;

	int dx, dt;

	for (i = 0; i < 10; i++)

		for (j = 0; j < 1000; j++)

			if ((rand() ^ j) & 1)

				x++;

			else

				t++;
	dx = x - O;

	dt = t - O;

	fprintf(stderr, "dx = %d, dt = %d\n", dx, dt);

	if (-200 <= dx && dx <= 200)

		a[dx + 200]++;

}
