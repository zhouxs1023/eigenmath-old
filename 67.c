/* An experiment with three-dimensional rotations.  Includes a
routine for computing the inverse of a three by three matrix using
Gaussian elimination. */

#include <stdio.h>
#include <math.h>

float x[3][3], f[3][3], g[3][3];

main()
{
	float costheta, cosphi1, cosphi2;
	float sintheta, sinphi1, sinphi2;

	costheta = 0.3;
	cosphi1 = 0.4;
	cosphi2 = 0.7;

	sintheta = sqrt(1 - costheta * costheta);
	sinphi1 = sqrt(1 - cosphi1 * cosphi1);
	sinphi2 = sqrt(1 - cosphi2 * cosphi2);

	x[0][0] = cosphi2 * cosphi1 - costheta * sinphi1 * sinphi2;
	x[0][1] = -cosphi2 * sinphi1 - costheta * cosphi1 * sinphi2;
	x[0][2] = sinphi2 * sintheta;

	x[1][0] = sinphi2 * cosphi1 + costheta * sinphi1 * cosphi2;
	x[1][1] = -sinphi2 * sinphi1 + costheta * cosphi1 * cosphi2;
	x[1][2] = -cosphi2 * sintheta;

	x[2][0] = sintheta * sinphi1;
	x[2][1] = sintheta * cosphi1;
	x[2][2] = costheta;

	invert();
	check();
}

invert()
{
	int d, i, j;
	float m;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++) {
			f[i][j] = x[i][j];
			if (i == j)
				g[i][j] = 1.0;
			else
				g[i][j] = 0.0;
		}

	for (d = 0; d < 3; d++) {

		m = f[d][d];

		for (j = 0; j < 3; j++) {
			f[d][j] /= m;
			g[d][j] /= m;
		}

		for (i = 0; i < 3; i++) {
			if (i == d)
				continue;
			m = f[i][d];
			for (j = 0; j < 3; j++) {
				f[i][j] -= m * f[d][j];
				g[i][j] -= m * g[d][j];
			}
		}
	}
}

print()
{
	printf("\n");
	printf("%10g%10g%10g          %10g%10g%10g\n", x[0][0], x[0][1], x[0][2], g[0][0], g[0][1], g[0][2]);
	printf("%10g%10g%10g          %10g%10g%10g\n", x[1][0], x[1][1], x[1][2], g[1][0], g[1][1], g[1][2]);
	printf("%10g%10g%10g          %10g%10g%10g\n", x[2][0], x[2][1], x[2][2], g[2][0], g[2][1], g[2][2]);
}

check()
{
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (fabs(fabs(x[i][j]) - fabs(g[j][i])) > 0.01)
				return;
	print();
}
