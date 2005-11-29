/* Simulate a photon using a ``step by step'' algorithm
and a 127 pattern for the random number generator. */

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

			if ((random2() ^ j) & 1)

				x++;

			else

				t++;
	dx = x - O;

	dt = t - O;

	fprintf(stderr, "dx = %d, dt = %d\n", dx, dt);

	if (-200 <= dx && dx <= 200)

		a[dx + 200]++;

}

char *s = "0000111011110010110010010000001000100110001011101011011000001100110101001110011110110100001010101111101001010001101110001111111";

int x;

random2()

{

	if (++x == 127)

		x = 0;

	return s[x];

}
