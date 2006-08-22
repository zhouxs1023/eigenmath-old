//	           exp(2 x) - 1
//	tanh(x) = --------------
//	           exp(2 x) + 1

#include "stdafx.h"
#include "defs.h"

void
eval_tanh(void)
{
	double d;
	push(cadr(p1));
	eval();
	p1 = pop();
	if (car(p1) == symbol(ARCTANH)) {
		push(cadr(p1));
		return;
	}
	if (isdouble(p1)) {
		d = tanh(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}
	if (exp_flag) {
		push(p1);
		push_integer(2);
		multiply();
		exponential();
		p1 = pop();
		push(p1);
		push_integer(1);
		subtract();
		push(p1);
		push_integer(1);
		add();
		divide();
		return;
	}
	if (iszero(p1)) {
		push(zero);
		return;
	}
	push_symbol(TANH);
	push(p1);
	list(2);
}

static char *s[] = {

	"tanh(x)",
	"tanh(x)",

	"tanh(0)",
	"0",

	"circexp(tanh(x))",
	"-1/(1+exp(2*x))+exp(2*x)/(1+exp(2*x))",

	"tanh(arctanh(x))",
	"x",
};

void
test_tanh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
