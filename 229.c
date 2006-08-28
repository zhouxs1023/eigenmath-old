// Simulate a statistics problem.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M 10
#define N 10000

double m = M, t = 0.0;

main()
{
	int i;
	for (i = 0; i < N; i++)
		t += f();
	printf("%g %g\n", t / N, pow(1 - 1 / m, m - 1) * 1 / m);
}

f()
{
	int i, n;
	for (i = 0; i < M - 1; i++) {
//		n = 1 + (int) (m * rand() / (RAND_MAX + 1.0));
		n = 1 + rand() % M;
		if (n == 1)
			return 0;
	}
//	n = 1 + (int) (m * rand() / (RAND_MAX + 1.0));
	n = 1 + rand() % M;
	if (n == 1)
		return 1;
	else
		return 0;
}
