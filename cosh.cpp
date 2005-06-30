//	          exp(x) + exp(-x)
//	cosh(x) = ----------------
//	                 2

#include "stdafx.h"
#include "defs.h"
void ycosh(void);
static void yycosh(void);

void
eval_cosh(void)
{
	push(cadr(p1));
	eval();
	ycosh();
}

void
ycosh(void)
{
	save();
	yycosh();
	restore();
}

static void
yycosh(void)
{
	double d;
	p1 = pop();
	if (p1->k == DOUBLE) {
		d = cosh(p1->u.d);
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
		add();
		push_rational(1, 2);
		multiply();
		return;
	}
	if (iszero(p1)) {
		push(one);
		return;
	}
	push_symbol(COSH);
	push(p1);
	list(2);
}

static char *s[] = {

	"cosh(x)",
	"cosh(x)",

	"cosh(0)",
	"1",

	"expomode=1",
	"",

	"cosh(x)",
	"1/2*exp(x)+1/2*exp(-x)",

	"expomode=0",
	"",
};

void
test_cosh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
