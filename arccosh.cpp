#include "stdafx.h"

#include "defs.h"

static void arccosh(void);

void
eval_arccosh(void)
{
	push(cadr(p1));
	eval();
	arccosh();
}

static void
arccosh(void)
{
	double d;
	save();
	p1 = pop();
	if (p1->k == DOUBLE) {
		d = p1->u.d;
		if (d < 1.0)
			stop("arccosh function argument is less than 1.0");
		d = log(d + sqrt(d * d - 1.0));
		push_double(d);
		restore();
		return;
	}
	if (isplusone(p1)) {
		push(zero);
		restore();
		return;
	}
	push_symbol(ARCCOSH);
	push(p1);
	list(2);
	restore();
}

static char *s[] = {

	"arccosh(1.0)",
	"0",

	"arccosh(1)",
	"0",
};

void
test_arccosh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
