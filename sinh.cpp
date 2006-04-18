//	          exp(x) - exp(-x)
//	sinh(x) = ----------------
//	                 2

#include "stdafx.h"
#include "defs.h"

void
eval_sinh(void)
{
	push(cadr(p1));
	eval();
	ysinh();
}

void
ysinh(void)
{
	save();
	yysinh();
	restore();
}

void
yysinh(void)
{
	double d;
	p1 = pop();
	if (car(p1) == symbol(ARCSINH)) {
		push(cadr(p1));
		return;
	}
	if (isdouble(p1)) {
		d = sinh(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}
	if (expomode == 1) {
		push(p1);
		exponential();
		push(p1);
		negate();
		exponential();
		subtract();
		push_rational(1, 2);
		multiply();
		return;
	}
	if (iszero(p1)) {
		push(zero);
		return;
	}
	push_symbol(SINH);
	push(p1);
	list(2);
}

static char *s[] = {

	"sinh(x)",
	"sinh(x)",

	"sinh(0)",
	"0",

	"expomode=1",
	"",

	"sinh(x)",
	"1/2*exp(x)-1/2*exp(-x)",

	"expomode=0",
	"",

	"sinh(arcsinh(x))",
	"x",
};

void
test_sinh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
