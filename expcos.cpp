// Do the exponential cosine function.

#include "stdafx.h"
#include "defs.h"

void
eval_expcos(void)
{
	push(cadr(p1));
	eval();
	expcos();
}

void
expcos(void)
{
	save();

	p1 = pop();

	push(unit_imaginary);
	push(p1);
	multiply();
	exponential();
	push_integer(2);
	divide();

	push(unit_imaginary);
	negate();
	push(p1);
	multiply();
	exponential();
	push_integer(2);
	divide();

	add();

	restore();
}

static char *s[] = {

	"expcos(x)",
	"1/2*exp(-i*x)+1/2*exp(i*x)",
};

void
test_expcos(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
