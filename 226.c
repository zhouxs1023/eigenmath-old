// statistical methods class

#include <stdio.h>
#include <math.h>

#define N 14

double x[N] = {99,104,151,129,142,111,74,91,119,121,135,147,83,125};
double y[N] = {3.4,5.2,6.4,4.7,5.8,3.6,2.1,2.9,4.2,3.9,5.4,4.9,1.8,4.5};

main()
{
	int i;
	double X, Y, SXX, SYY, SXY, r, Z, ZLB, ZUB, a, b, t;
	double beta0, beta1;

	X = 0.0;
	for (i = 0; i < N; i++)
		X += x[i];
	X /= N;
	printf("X = %g\n", X);

	Y = 0.0;
	for (i = 0; i < N; i++)
		Y += y[i];
	Y /= N;
	printf("Y = %g\n", Y);

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

	beta1 = SXY /SXX;
	beta0 = Y - beta1 * X;

	printf("beta0 = %g\n", beta0);
	printf("beta1 = %g\n", beta1);

	printf("coeff. of determination = %g\n", SXY * SXY / SXX / SYY);

	printf("SSR = %g\n", SXY * SXY / SXX);
	printf("SSE = %g\n", SYY - SXY * SXY / SXX);
	printf("SST = %g\n", SYY);
}
