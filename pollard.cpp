// Factor using the Pollard rho method

#include "stdafx.h"
#include "defs.h"
static void factor_a(unsigned int *);
static void try_kth_prime(unsigned int **, int);
static void push_factor(unsigned int *, int);
static int factor_b(unsigned int *);

void
factor_number(void)
{
	int h;
	unsigned int *n;

	save();

	p1 = pop();

	n = p1->u.q.a;

	if (MLENGTH(n) == 1 && (n[0] == 0 || n[0] == 1)) {
		push(p1);
		restore();
		return;
	}

	h = tos;

	factor_a(n);

	if (tos - h > 1) {
		list(tos - h);
		push_symbol(MULTIPLY);
		swap();
		cons();
	}

	restore();
}

// factor using table look-up, then switch to rho method if necessary

// From TAOCP Vol. 2 by Knuth, p. 380 (Algorithm A)

static void
factor_a(unsigned int *n)
{
	int k;

	n = mcopy(n);

	if (MSIGN(n) == -1) {
		MSIGN(n) = 1;
		push_integer(-1);
	}

	for (k = 0; k < 10000; k++) {

		try_kth_prime(&n, k);

		// if n is 1 then we're done

		if (MLENGTH(n) == 1 && n[0] == 1) {
			mfree(n);
			return;
		}
	}

	factor_b(n);
}

static void
try_kth_prime(unsigned int **n, int k)
{
	int count;
	unsigned int *d, *q, *r;

	d = mint(primetab[k]);

	count = 0;

	while (1) {

		// if n is 1 then we're done

		if (MLENGTH(*n) == 1 && (*n)[0] == 1) {
			if (count)
				push_factor(d, count);
			else
				mfree(d);
			return;
		}

		mdivrem(&q, &r, *n, d);

		// continue looping while remainder is zero

		if (MLENGTH(r) == 1 && r[0] == 0) {
			count++;
			mfree(r);
			mfree(*n);
			*n = q;
		} else {
			mfree(r);
			break;
		}
	}

	if (count)
		push_factor(d, count);

	// q = n / d

	// if q < d then n < d^2 so n is prime

	if (mcmp(q, d) == -1) {
		push_factor(*n, 1);
		*n = mint(1);
	}

	if (count == 0)
		mfree(d);

	mfree(q);
}

static void
push_factor(unsigned int *d, int count)
{
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = d;
	p1->u.q.b = mint(1);
	push(p1);
	if (count > 1) {
		push_symbol(POWER);
		swap();
		p1 = alloc();
		p1->k = NUM;
		p1->u.q.a = mint(count);
		p1->u.q.b = mint(1);
		push(p1);
		list(3);
	}
}

// From TAOCP Vol. 2 by Knuth, p. 385 (Algorithm B)

static int
factor_b(unsigned int *n)
{
	int k, l;
	unsigned int *g, *one, *t, *x, *xprime;
	unsigned int count;

	count = 0;

	one = mint(1);

	x = mint(5);

	xprime = mint(2);

	k = 1;

	l = 1;

	while (1) {

		if (mprime(n)) {
			p1 = alloc();
			p1->k = NUM;
			p1->u.q.a = n;
			p1->u.q.b = mint(1);
			push(p1);
			mfree(one);
			mfree(x);
			mfree(xprime);
			return 0;
		}

		while (1) {

			if (esc_flag) {
				mfree(one);
				mfree(x);
				mfree(xprime);
				stop("esc");
			}

			// g = gcd(x' - x, n)

			t = msub(xprime, x);
			MSIGN(t) = 1;
			g = mgcd(t, n);
//printf("x=%d x'=%d t=%d n=%d g=%d\n", x[0], xprime[0], t[0], n[0], g[0]);

			mfree(t);

			if (MEQUAL(g, 1)) {
				mfree(g);
				if (--k == 0) {
					mfree(xprime);
					xprime = mcopy(x);
					l *= 2;
					k = l;
				}

				// x = (x ^ 2 + 1) mod n

				t = mmul(x, x);
				mfree(x);
				x = madd(t, one);
				mfree(t);
				t = mmod(x, n);
				mfree(x);
				x = t;

				continue;
			}

			p1 = alloc();
			p1->k = NUM;
			p1->u.q.a = g;
			p1->u.q.b = mint(1);
			push(p1);

			if (mcmp(g, n) == 0) {
				mfree(one);
				mfree(n);
				mfree(x);
				mfree(xprime);
				return -1;
			}

			// n = n / g

			t = mdiv(n, g);
			mfree(n);
			n = t;

			// x = x mod n

			t = mmod(x, n);
			mfree(x);
			x = t;

			// xprime = xprime mod n

			t = mmod(xprime, n);
			mfree(xprime);
			xprime = t;

//printf("n=%d x=%d x'=%d\n", n[0], x[0], xprime[0]);

			break;
		}
	}
}

#if 0

// old method

// starts to get very slow around factor(26!)

// n is freed

static void
factor_integer_f(unsigned int *n)
{
	unsigned int *x, *y;

	if (mprime(n)) {
		p1 = alloc();
		p1->k = NUM;
		p1->u.q.a = n;
		p1->u.q.b = mint(1);
		push(p1);
		return;
	}

	x = mp_factor(n);

	if (MEQUAL(x, 1)) {
		mfree(x);
		p1 = alloc();
		p1->k = NUM;
		p1->u.q.a = n;
		p1->u.q.b = mint(1);
		push(p1);
		return;
	}

	y = mdiv(n, x);

	mfree(n);

	factor_integer_f(x);
	factor_integer_f(y);
}

#endif

#if 0

void
test_factor_timing(void)
{
	int i;
	unsigned int t;
	struct tms tms;
	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = mint(1);
	p1->u.q.b = mint(1);
	t = times(&tms);
	for (i = 0; i < 10000; i++) {
		tos = 0;
		p1->u.q.a[0] = random();
		push(p1);
		factor_number();
	}
	t = times(&tms) - t;
	printf("%d.%02d seconds\n", t / 100, t % 100);


	for (i = 0; i < 10000; i++) {
		tos = 0;
		p1->u.q.a[0] = random();
		push(p1);
		factor_number();
		if (tos > 1) {
			list(tos);
			push_symbol(MULTIPLY);
			swap();
			cons();
		}
		eval();
		p2 = pop();
		if (mcmp(p1->u.q.a, p2->u.q.a) != 0) {
			printf("failed %u %u\n", p1->u.q.a[0], p2->u.q.a[0]);
			Exit(1);
		}
	}
	Exit(1);
}

#endif

void
test_factor_integer_f(int len, int count)
{
	int i, j;

	unsigned int *x;

	x = mnew(len);
	MLENGTH(x) = len;
	MSIGN(x) = 1;

	p1 = alloc();
	p1->k = NUM;
	p1->u.q.a = x;
	p1->u.q.b = mint(1);

	for (i = 0; i < count; i++) {

		for (j = 0; j < len; j++)
			x[j] = rand();

		tos = 0;

		push(p1);

		factor_number();

		eval();

		p2 = pop();

		if (mcmp(p1->u.q.a, p2->u.q.a) != 0) {
			sprintf(logbuf, "failed %u %u\n", p1->u.q.a[0], p2->u.q.a[0]);
			logout(logbuf);
			errout();
		}
	}
}

void
test_factor_integer(void)
{
	int n;

	logout("testing factor_integer\n");

	gc();

	n = mtotal;

	test_factor_integer_f(1, 1000);
	test_factor_integer_f(2, 100);
	test_factor_integer_f(3, 10);

	// check for memory leak

	p1 = nil;
	p2 = nil;

	gc();

	if (mtotal != n) {
		sprintf(logbuf, "memory leak %d %d\n", n, mtotal);
		logout(logbuf);
		errout();
	}

	logout("ok\n");
}
