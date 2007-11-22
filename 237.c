// Find all the roots of a polynomial

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DELTA 1.0e-6
#define EPSILON 1.0e-9
#define ABS(x) sqrt(x.r * x.r + x.i * x.i)
#define RANDOM (1.0 + 3.0 * (double) rand() / RAND_MAX)

static struct {
	double r, i;
} a, b, x, y, fa, fb, dx, df, c[1000];

static int n;

main(int argc, char *argv[])
{
	int k, n;

	if (argc < 2) {
		printf("For example, 3x^2+2x+1: roots 3 2 1\n");
		return;
	}

	n = argc - 1;

	for (k = 1; k < argc; k++)
		c[n - k].r = atof(argv[k]);

	monic(n);

	// n is the number of coefficients, n = deg(p) + 1

	// For example, for a cubic we have n = 4. Then in the following loop,
	// k takes on the values 4, 3 and 2. This corresponds to the three
	// roots of the cubic polynomial.

	for (k = n; k > 1; k--) {
		findroot(k);
		if (fabs(a.r) < EPSILON)
			a.r = 0.0;
		if (fabs(a.i) < EPSILON)
			a.i = 0.0;
		printf("%g %g\n", a.r, a.i);
		divpoly(k);
	}
}

// divide the polynomial by its leading coefficient

monic(int n)
{
	int k;
	double t;
	y = c[n - 1];
	t = y.r * y.r + y.i * y.i;
	for (k = 0; k < n - 1; k++) {
		c[k].r = (c[k].r * y.r + c[k].i * y.i) / t;
		c[k].i = (c[k].i * y.r - c[k].r * y.i) / t;
	}
	c[n - 1].r = 1.0;
	c[n - 1].i = 0.0;
}

findroot(int n)
{
	int j, k;
	double t;

	if (ABS(c[0]) < DELTA) {
		a.r = 0.0;
		a.i = 0.0;
		return;
	}

	for (j = 0; j < 100; j++) {

		a.r = RANDOM;
		a.i = RANDOM;

		compute_fa(n);

		b = a;
		fb = fa;

		a.r = RANDOM;
		a.i = RANDOM;

		for (k = 0; k < 1000; k++) {

			compute_fa(n);

			if (ABS(fa) < EPSILON)
				return;

			if (ABS(fa) < ABS(fb)) {
				x = a;
				a = b;
				b = x;
				x = fa;
				fa = fb;
				fb = x;
			}

			// dx = b - a

			dx.r = b.r - a.r;
			dx.i = b.i - a.i;

			// df = fb - fa

			df.r = fb.r - fa.r;
			df.i = fb.i - fa.i;

			// y = dx / df

			t = df.r * df.r + df.i * df.i;

			if (t == 0.0)
				break;

			y.r = (dx.r * df.r + dx.i * df.i) / t;
			y.i = (dx.i * df.r - dx.r * df.i) / t;

			// a = b - y * fb

			a.r = b.r - (y.r * fb.r - y.i * fb.i);
			a.i = b.i - (y.r * fb.i + y.i * fb.r);
		}
	}
	printf("Convergence error\n");
}

compute_fa(int n)
{
	int k;
	double t;

	// x = a

	x.r = a.r;
	x.i = a.i;

	// fa = c0 + c1 * x

	fa.r = c[0].r + c[1].r * x.r - c[1].i * x.i;
	fa.i = c[0].i + c[1].r * x.i + c[1].i * x.r;

	for (k = 2; k < n; k++) {

		// x = a * x

		t = a.r * x.r - a.i * x.i;
		x.i = a.r * x.i + a.i * x.r;
		x.r = t;

		// fa += c[k] * x

		fa.r += c[k].r * x.r - c[k].i * x.i;
		fa.i += c[k].r * x.i + c[k].i * x.r;
	}
}

// divide the polynomial by x - a

// Note that the leading coefficient never changes. Thus a polynomial that
// starts out monic remains monic.

divpoly(int n)
{
	int k;
	for (k = n - 1; k > 0; k--) {
		c[k - 1].r += c[k].r * a.r - c[k].i * a.i;
		c[k - 1].i += c[k].i * a.r + c[k].r * a.i;
	}
	if (ABS(c[0]) > DELTA)
		printf("Residual error\n");
	for (k = 0; k < n - 1; k++) {
		c[k].r = c[k + 1].r;
		c[k].i = c[k + 1].i;
	}
}
