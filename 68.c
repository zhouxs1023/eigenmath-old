/* Another experiment with three-dimensional rotations. */

#include <stdio.h>
#include <math.h>

float x[3][3], f[3][3], g[3][3];

main()
{
	int i;
	float a, b;

	a = -0.4;

	b = -sqrt(1.0 - a * a - a * a);

	x[0][0] = a;
	x[0][1] = b;
	x[0][2] = a;

	x[1][0] = b;
	x[1][1] = a;
	x[1][2] = b;

	x[2][0] = a;
	x[2][1] = b;
	x[2][2] = a;

	for (i = 0; i < 512; i++) {

		if (i % 1 == 0)
			x[0][0] *= -1.0;

		if (i % 2 == 0)
			x[0][1] *= -1.0;

		if (i % 4 == 0)
			x[0][2] *= -1.0;

		if (i % 8 == 0)
			x[1][0] *= -1.0;

		if (i % 16 == 0)
			x[1][1] *= -1.0;

		if (i % 32 == 0)
			x[1][2] *= -1.0;

		if (i % 64 == 0)
			x[2][0] *= -1.0;

		if (i % 128 == 0)
			x[2][1] *= -1.0;

		if (i % 256 == 0)
			x[2][2] *= -1.0;

		invert();
/*		print(); */

		check();
	}
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
