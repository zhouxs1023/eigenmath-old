#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Binomial coefficient
//
//	Input:		tos-2		n
//
//			tos-1		k
//
//	Output:		Binomial coefficient on stack
//
//	binomial(n, k) = n! / k! / (n - k)!
//
//	The binomial coefficient vanishes for k < 0 or k > n. (A=B, p. 19)
//
//-----------------------------------------------------------------------------

#include "defs.h"

static void __binomial(void);
static int check_args(void);

void
binomial(void)
{
	save();
	__binomial();
	restore();
}

#define N p1
#define K p2

static void
__binomial(void)
{
	K = pop();
	N = pop();

	if (check_args() == 0) {
		push(_zero);
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
}

static int
check_args(void)
{
	if (isnum(N) && lessp(N, _zero))
		return 0;
	else if (isnum(K) && lessp(K, _zero))
		return 0;
	else if (isnum(N) && isnum(K) && lessp(N, K))
		return 0;
	else
		return 1;
}

char *s[] = {

	"binomial(12,6)",
	"924",

	"binomial(n,k)",
//	"1/(factorial(k))*factorial(n)*1/(factorial(-k+n))",
//	"factorial(n)/(factorial(k)*factorial(-k+n))",
	"n!/(k!*(-k+n)!)",

	"binomial(0,k)",
//	"1/(factorial(k))*1/(factorial(-k))",
//	"1/(factorial(k)*factorial(-k))",
	"1/(k!*(-k)!)",

	"binomial(n,0)",
	"1",

	"binomial(-1,k)",
	"0",

	"binomial(n,-1)",
	"0",
};

void
test_binomial(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
