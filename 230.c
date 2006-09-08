/* Archimedean method for calculating $\pi$. */

#include <stdio.h>
#include <math.h>

main()
{
	int k;
	double a[11], b[11];
	a[0] = 4.0 * sqrt(3.0);
	b[0] = 6.0;
	for (k = 0; k < 10; k++) {
		a[k + 1] = 2.0 * a[k] * b[k] / (a[k] + b[k]);
		b[k + 1] = sqrt(a[k + 1] * b[k]);
		printf("%12.10f < Circumference < %12.10f\n", b[k], a[k]);
	}
	for (k = 0; k < 10; k++)
		printf("%12.10f < $\\pi$ < %12.10f\n", b[k] / 2.0, a[k] / 2.0);
}
