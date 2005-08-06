#include "stdafx.h"

#include "defs.h"

static void arctanh(void);

void
eval_arctanh(void)
{
	push(cadr(p1));
	eval();
	arctanh();
}

static void
arctanh(void)
{
	double d;
	save();
	p1 = pop();
	if (isdouble(p1)) {
		d = p1->u.d;
		if (d < -1.0 || d > 1.0)
			stop("arctanh function argument is not in the interval [-1,1]");
		d = log((1.0 + d) / (1.0 - d)) / 2.0;
		push_double(d);
		restore();
		return;
	}
	if (iszero(p1)) {
		push(zero);
		restore();
		return;
	}
	push_symbol(ARCTANH);
	push(p1);
	list(2);
	restore();
}

static char *s[] = {

	"arctanh(0.0)",
	"0",

	"arctanh(0)",
	"0",
};

void
test_arctanh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
