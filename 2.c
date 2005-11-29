/* Check to see if a three by three matrix is a rotation. */

#include <stdio.h>
#include <math.h>

double I[3][3] = { M_SQRT_2, -M_SQRT_2, 0.0, M_SQRT_2, M_SQRT_2, 0.0, 0.0, 0.0, 1.0 };
double a[3][3] = { -1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };

main()
{
	printf("%d", isrotation3(I));
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

mult3(a, b)
double a[3][3], b[3][3];
{
	int i, j, k;
	double t[3][3];

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			t[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] + a[i][2] * b[2][j];

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			a[i][j] = t[i][j];
}

/*****************************************************************************/

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

/* is this matrix a rotation? */

isrotation3(a)
double a[3][3];
{
	int i, j;
	double d;

	/* verify that transpose of a times a equals identity */

	/* i is the row index for transpose of a */

	for (i = 0; i < 3; i++)

		/* j is the column index for a */

		for (j = 0; j < 3; j++) {

			/* row of transpose of a times column of a */

			d = a[0][i] * a[0][j] + a[1][i] * a[1][j] + a[2][i] * a[2][j];

			/* diagonal element? */

			if (i == j)

				d -= 1.0;

			if (fabs(d) > 1e-6)

				return 0;
		}

	return 1;
}
