/* how many E . B = 0 for positive random integer F? */

#include <stdio.h>
#include <stdlib.h>

#define N 10

main()
{
	int i, n = 0;
	double a[4][4], b1, b2, b3, e1, e2, e3;

	printf("how many E . B = 0 for positive random integer F?\n");

	printf("0 <= F[a][b] <= %d\n", N - 1);

	for (i = 0; i < 10000; i++) {

		e1 = random(N);
		e2 = random(N);
		e3 = random(N);

		/* eliminate cases where E = 0 */

		if (e1 == 0.0 && e2 == 0.0 && e3 == 0.0)
			continue;

		b1 = random(N);
		b2 = random(N);
		b3 = random(N);

		/* eliminate cases where B = 0 */

		if (b1 == 0.0 && b2 == 0.0 && b3 == 0.0)
			continue;

		feb(a, e1, e2, e3, b1, b2, b3);

		if (isedotbzero(a)) {
			printf("\n%d\n", ++n);
			print4(a);
		}
	}

	printf("\n%g\%\n", 100.0 * (double) n / (double) i);
}

isedotbzero(a)
double a[4][4];
{
	double b1, b2, b3, e1, e2, e3, v1, v2, v3, x;

	b1 = a[1][2];
	b2 = a[2][0];
	b3 = a[0][1];

	e1 = a[3][0];
	e2 = a[3][1];
	e3 = a[3][2];

	if (e1 * b1 + e2 * b2 + e3 * b3 == 0.0)
		return 1;
	else
		return 0;
}

feb(a, e1, e2, e3, b1, b2, b3)
double a[4][4], e1, e2, e3, b1, b2, b3;
{
	a[0][0] = 0.0;
	a[0][1] = b3;
	a[0][2] = -b2;
	a[0][3] = e1;

	a[1][0] = -b3;
	a[1][1] = 0.0;
	a[1][2] = b1;
	a[1][3] = e2;

	a[2][0] = b2;
	a[2][1] = -b1;
	a[2][2] = 0.0;
	a[2][3] = e3;

	a[3][0] = e1;
	a[3][1] = e2;
	a[3][2] = e3;
	a[3][3] = 0.0;
}

print4(a)
double a[4][4];
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			printf("%15g", a[i][j]);
		printf("\n");
	}
}
