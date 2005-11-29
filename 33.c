/* Find rotation matrix to normalize $\vec E$. */

#include <stdio.h>
#include <math.h>

double e1 = 1.0;
double e2 = 2.0;
double e3 = 3.0;

double r[3][3];

main()
{
	double e1hat, e2hat, e3hat;

	printf("\nE = (%g, %g, %g)\n", e1, e2, e3);

	r[0][0] = e2 / sqrt(e1 * e1 + e2 * e2);
	r[0][1] = -e1 / sqrt(e1 * e1 + e2 * e2);
	r[0][2] = 0.0;

	r[1][0] = e1 * e3 / sqrt(e1 * e1 + e2 * e2) / sqrt(e1 * e1 + e2 * e2 + e3 * e3);
	r[1][1] = e2 * e3 / sqrt(e1 * e1 + e2 * e2) / sqrt(e1 * e1 + e2 * e2 + e3 * e3);
	r[1][2] = -sqrt(e1 * e1 + e2 * e2) / sqrt(e1 * e1 + e2 * e2 + e3 * e3);

	r[2][0] = e1 / sqrt(e1 * e1 + e2 * e2 + e3 * e3);
	r[2][1] = e2 / sqrt(e1 * e1 + e2 * e2 + e3 * e3);
	r[2][2] = e3 / sqrt(e1 * e1 + e2 * e2 + e3 * e3);

	e1hat = r[0][0] * e1 + r[0][1] * e2 + r[0][2] * e3;
	e2hat = r[1][0] * e1 + r[1][1] * e2 + r[1][2] * e3;
	e3hat = r[2][0] * e1 + r[2][1] * e2 + r[2][2] * e3;

	printf("\nrotation matrix R = \n");
	printm(r);

	printf("\nRE = (%g, %g, %g)\n", e1hat, e2hat, e3hat);
}

printm(a)
double a[3][3];
{
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%15g", a[i][j]);
		printf("\n");
	}
}
