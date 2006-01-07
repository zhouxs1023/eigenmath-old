#include "stdafx.h"
#include "defs.h"

void
eval_factor(void)
{
	push(cadr(p1));
	eval();
	if (isinteger(stack[tos - 1])) {
		factor_number(); // see pollard.cpp
		return;
	}
	p1 = cddr(p1);
	push(car(p1));
	eval();
	factorpoly(); // factorpoly handles omitted 2nd arg (2nd arg = nil)
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		factorpoly();
		p1 = cdr(p1);
	}
}

// for factoring small integers (2^32 or less)

void
factor_small_number(void)
{
	int d, expo, i, n;

	save();

	n = pop_integer();

	if (n == (int) 0x80000000)
		stop("number too big to factor");

	if (n < 0)
		n = -n;

	for (i = 0; i < MAXPRIMETAB; i++) {

		d = primetab[i];

		if (d > n / d)
			break;

		expo = 0;

		while (n % d == 0) {
			n /= d;
			expo++;
		}

		if (expo) {
			push_integer(d);
			push_integer(expo);
		}
	}

	if (n > 1) {
		push_integer(n);
		push_integer(1);
	}

	restore();
}

static char *s[] = {

	"factor(0)",
	"0",

	"factor(1)",
	"1",

	"factor(2)",
	"2",

	"factor(3)",
	"3",

	"factor(4)",
	"2^2",

	"factor(5)",
	"5",

	"factor(6)",
	"2*3",

	"factor(7)",
	"7",

	"factor(8)",
	"2^3",

	"factor(9)",
	"3^2",

	"factor(10)",
	"2*5",

	"factor(100!)",
	"2^97*3^48*5^24*7^16*11^9*13^7*17^5*19^5*23^4*29^3*31^3*37^2*41^2*43^2*47^2*53*59*61*67*71*73*79*83*89*97",

	"factor(2*(2^30-35))",
	"2*1073741789",

	// x is the 10,000th prime

	// Prime factors greater than x^2 are found using the Pollard rho method

	"x=104729",
	"",

	"factor(2*(x^2+6))",
	"2*10968163447",

	"factor((x^2+6)^2)",
	"10968163447*10968163447",	// FIXME should be 10968163447^2

	"factor((x^2+6)*(x^2+60))",
	"10968163501*10968163447",	// FIXME sort order

	"x=quote(x)",
	"",
};

void
test_factor_number(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
