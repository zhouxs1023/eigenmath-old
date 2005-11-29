/* Find smallest ${|\vec E|^2+|\vec B|^2 \over |\vec E\times\vec B|}$
for random values of $\vec E$ and $\vec B$. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main()
{
	int i;
	double a, b, min = 1000;
	double x1, x2, x3;

	double b1, b2, b3;
	double e1, e2, e3;

	for (i = 0; i < 1000; i++) {

		e1 = random(2001) - 1000;
		e2 = random(2001) - 1000;
		e3 = random(2001) - 1000;

		b1 = random(2001) - 1000;
		b2 = random(2001) - 1000;
		b3 = random(2001) - 1000;

		a = e1 * e1 + e2 * e2 + e3 * e3 + b1 * b1 + b2 * b2 + b3 * b3;

		x1 = e2 * b3 - e3 * b2;
		x2 = e3 * b1 - e1 * b3;
		x3 = e1 * b2 - e2 * b1;

		b = sqrt(x1 * x1 + x2 * x2 + x3 * x3);

		if (b == 0.0)
			continue;

		if (a / b < min)
			min = a / b;

		printf(".");
	}

	printf("%g\n", min);
}
