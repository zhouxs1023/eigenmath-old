// Do the exponential sine function.

#include "stdafx.h"
#include "defs.h"

void
eval_expsin(void)
{
	push(cadr(p1));
	eval();
	expsin();
}

void
expsin(void)
{
	save();

	p1 = pop();

	push(unit_imaginary);
	push(p1);
	multiply();
	exponential();
	push(unit_imaginary);
	divide();
	push_integer(2);
	divide();

	push(unit_imaginary);
	negate();
	push(p1);
	multiply();
	exponential();
	push(unit_imaginary);
	divide();
	push_integer(2);
	divide();

	subtract();

	restore();
}

static char *s[] = {

	"expsin(x)",
	"1/2*i*exp(-i*x)-1/2*i*exp(i*x)",
};

void
test_expsin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
