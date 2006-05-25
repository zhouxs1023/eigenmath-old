// statistical methods class

#include <stdio.h>
#include <math.h>

#define N 15

double x[N] = {5,8,5,4,5,9,7,3,8,8,6,6,7,9,4};
double y[N] = {4,7,5,3,6,7,4,1,5,6,4,3,6,8,1};

main()
{
	int i;
	double X, Y, SXX, SYY, SXY, r, Z, ZLB, ZUB, a, b, t;

	X = 0.0;
	for (i = 0; i < N; i++)
		X += x[i];
	X /= N;

	Y = 0.0;
	for (i = 0; i < N; i++)
		Y += y[i];
	Y /= N;

	SXX = 0.0;
	for (i = 0; i < N; i++)
		SXX += (x[i] - X) * (x[i] - X);
	printf("SXX = %g\n", SXX);

	SYY = 0.0;
	for (i = 0; i < N; i++)
		SYY += (y[i] - Y) * (y[i] - Y);
	printf("SYY = %g\n", SYY);

	SXY = 0.0;
	for (i = 0; i < N; i++)
		SXY += (x[i] - X) * (y[i] - Y);
	printf("SXY = %g\n", SXY);

	r = SXY / sqrt(SXX * SYY);
	printf("r = %g\n", r);

	Z = 0.5 * log((1 + r) / (1 - r));
	printf("Z = %g\n", Z);

	// 95% confidence interval

	ZLB = Z - 1.96 / sqrt(N - 3);
	ZUB = Z + 1.96 / sqrt(N - 3);

	a = (exp(2.0 * ZLB) - 1) / (exp(2.0 * ZLB) + 1);
	b = (exp(2.0 * ZUB) - 1) / (exp(2.0 * ZUB) + 1);

	printf("CI = (%g, %g)\n", a, b);

	t = r * sqrt((N - 2) / (1 - r * r));
	printf("t = %g\n", t);
}
