// choose(n, k) = n! / k! / (n - k)!

// Result vanishes for k < 0 or k > n. (A=B, p. 19)

#include "stdafx.h"
#include "defs.h"

void
eval_choose(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	choose();
}

// For example...
//
//	push(N)
//	push(K)
//	choose()

#define N p1
#define K p2

void
choose(void)
{
	save();

	K = pop();
	N = pop();

	if (choose_check_args() == 0) {
		push_integer(0);
		restore();
		return;
	}

	push(N);
	factorial();

	push(K);
	factorial();

	divide();

	push(N);
	push(K);
	subtract();
	factorial();

	divide();

	restore();
}

int
choose_check_args(void)
{
	if (isnum(N) && lessp(N, zero))
		return 0;
	else if (isnum(K) && lessp(K, zero))
		return 0;
	else if (isnum(N) && isnum(K) && lessp(N, K))
		return 0;
	else
		return 1;
}

#if SELFTEST

static char *s[] = {

	"choose(12,6)",
	"924",

	"choose(n,k)",
	"n!/(k!*(-k+n)!)",

	"choose(0,k)",
	"1/(k!*(-k)!)",

	"choose(n,0)",
	"1",

	"choose(-1,k)",
	"0",

	"choose(n,-1)",
	"0",
};

void
test_choose(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
