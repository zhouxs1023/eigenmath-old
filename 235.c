// First try at a  polynomial root finder.

#include <stdio.h>
#include <math.h>

#define ADD(a, b, c)				\
do {						\
	a.x = b.x + c.x;			\
	a.y = b.y + c.y;			\
} while (0);

#define SUB(a, b, c)				\
do {						\
	a.x = b.x - c.x;			\
	a.y = b.y - c.y;			\
} while (0);

#define MUL(a, b, c)				\
do {						\
	a.x = b.x * c.x - b.y * c.y;		\
	a.y = b.x * c.y + b.y * c.x;		\
} while (0);

#define MULADD(a, b, c)				\
do {						\
	a.x += b.x * c.x - b.y * c.y;		\
	a.y += b.x * c.y + b.y * c.x;		\
} while (0);

#define DIV(a, b, c)				\
do {						\
	t = c.x * c.x + c.y * c.y;		\
	a.x = (b.x * c.x + b.y * c.y) / t;	\
	a.y = (b.y * c.x - b.x * c.y) / t;	\
} while (0);

#define ABS(a) sqrt(a.x * a.x + a.y * a.y)

struct {double x, y;} a, b, x, y, z, fa, fb, fx, c0 = {1.0, 0.0}, c1 = {2.0, 0.0}, c2 = {3.0, 0.0};

double t;

f()
{
	fx = c0;
	MULADD(fx, c1, x);
	MUL(y, x, x);
	MULADD(fx, c2, y);
}

main()
{
	int i;

	a.x = 1.0;
	a.y = 0.0;

	b.x = 0.0;
	b.y = 1.0;

//	MUL(x, a, b);
//	printf("%g %g\n", x.x, x.y);

	x = a;
	f();
	fa = fx;

	x = b;
	f();
	fb = fx;

	for (i = 0; i < 100; i++) {

		if (ABS(fa) < ABS(fb)) {
			x = a;
			a = b;
			b = x;
			x = fa;
			fa = fb;
			fb = x;
		}

		SUB(x, a, b);
		SUB(y, fb, fa);
		DIV(z, x, y);
		MUL(x, z, fb);
		ADD(a, b, x);
		x = a;
		f();
		fa = fx;
		printf("%d %g %g %g %g %g\n", i, a.x, a.y, fa.x, fa.y, ABS(fa));
		if (ABS(fa) < 1.0e-10)
			break;
	}
}
