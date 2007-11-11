// A more refined root finder.

#include <stdio.h>
#include <math.h>

#define ABS(x) sqrt(x.r * x.r + x.i * x.i)

static struct {
	double r, i;
} a, b, m, x, fa, fb, dx, df, c[100];

static int n;

main()
{
	int k;

	c[0].r = 1.0;
	c[0].i = 0.0;

	c[1].r = 2.0;
	c[1].i = 0.0;

	c[2].r = 3.0;
	c[2].i = 0.0;

	k = findroot(3);

	printf("%d %g %g %g\n", k, a.r, a.i, ABS(fa));
}

findroot(int n)
{
	int i;
	double t;

	a.r = 0.0;
	a.i = 1.0;

	compute_fa(n);

	b = a;
	fb = fa;

	a.r = 1.0;
	a.i = 0.0;

	for (i = 0; i < 100; i++) {

		compute_fa(n);

		if (ABS(fa) < 1.0e-10)
			return i; // ok

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

		// m = dx / df

		t = df.r * df.r + df.i * df.i;

		if (t == 0.0)
			break;

		m.r = (dx.r * df.r + dx.i * df.i) / t;
		m.i = (dx.i * df.r - dx.r * df.i) / t;

		// a = b - m * fb

		a.r = b.r - (m.r * fb.r - m.i * fb.i);
		a.i = b.i - (m.r * fb.i + m.i * fb.r);
	}

	return -1; // fail
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
