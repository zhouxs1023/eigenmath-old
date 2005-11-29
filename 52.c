/* Compute $E^2 B^1 - E^1 E^2 \over E^1 B^2 + B^1 B^3$ and
$E^1 B^3 - B^1 B^2 \over E^1 E^2 + E^3 B^1$ for random values
of $\vec E$ and $\vec B$. */

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

		e1 = random(21) - 10;
		e2 = random(21) - 10;
		e3 = random(21) - 10;

		b1 = random(21) - 10;
		b2 = random(21) - 10;
		b3 = random(21) - 10;

		a = (e2 * b1 - e1 * e3) / (e1 * b2 + b1 * b3);

		b = (e1 * b3 - b1 * b2) / (e1 * e2 + e3 * b1);

		printf("%g %g\n ", a, b);
	}
}
