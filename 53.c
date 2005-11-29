/* Compute $|\vec E|^2+|\vec B|^2 \over |\vec E\times\vec B|$ for random
values of $\vec E$ and $\vec B$ with $E^1=B^1=0$. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main()
{
	int i;
	double a, b;
	double x1, x2, x3;

	double b1, b2, b3;
	double e1, e2, e3;

	for (i = 0; i < 100; i++) {

		e1 = 0.0;
		e2 = random(201) - 10;
		e3 = random(201) - 10;

		b1 = 0.0;
		b2 = random(201) - 10;
		b3 = random(201) - 10;

		a = e1 * e1 + e2 * e2 + e3 * e3 + b1 * b1 + b2 * b2 + b3 * b3;

		x1 = e2 * b3 - e3 * b2;
		x2 = e3 * b1 - e1 * b3;
		x3 = e1 * b2 - e2 * b1;

		b = sqrt(x1 * x1 + x2 * x2 + x3 * x3);

		if (b == 0.0)
			continue;

		printf(" %g ", a / b);
	}
}
