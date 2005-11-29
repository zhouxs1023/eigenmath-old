/* An experimental method of inverting a three by three matrix.
The determinant and adjunct of the matrix are computed explicitly
using inline code instead of loops. */

#include <stdio.h>

double a[3][3] = { -1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };

main()
{
	invert3(a);
	print();
	invert3(a);
	print();
}

/* invert 3x3 matrix */

invert3(a)
double a[3][3];
{
	int i, j;
	double b[3][3], d;

	/* d = determinant of a */

	d =	a[0][0] * a[1][1] * a[2][2] -
		a[0][0] * a[1][2] * a[2][1] +

		a[0][1] * a[1][2] * a[2][0] -
		a[0][1] * a[1][0] * a[2][2] +

		a[0][2] * a[1][0] * a[2][1] -
		a[0][2] * a[1][1] * a[2][0];

	/* check for singularity */

	if (d == 0.0) {
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				a[i][j] = 0.0;
		return;
	}

	/* b = adjunct of a */

	b[0][0] = a[1][1] * a[2][2] - a[1][2] * a[2][1];
	b[0][1] = a[0][2] * a[2][1] - a[0][1] * a[2][2];
	b[0][2] = a[0][1] * a[1][2] - a[0][2] * a[1][1];

	b[1][0] = a[1][2] * a[2][0] - a[1][0] * a[2][2];
	b[1][1] = a[0][0] * a[2][2] - a[0][2] * a[2][0];
	b[1][2] = a[0][2] * a[1][0] - a[0][0] * a[1][2];

	b[2][0] = a[1][0] * a[2][1] - a[1][1] * a[2][0];
	b[2][1] = a[0][1] * a[2][0] - a[0][0] * a[2][1];
	b[2][2] = a[0][0] * a[1][1] - a[0][1] * a[1][0];

	/* a = b divided by determinant */

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			a[i][j] = b[i][j] / d;
}

print()
{
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++)
			printf("%10g", a[i][j]);
		printf("\n");
	}
}
