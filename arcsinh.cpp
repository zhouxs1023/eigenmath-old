#include "stdafx.h"

#include "defs.h"

static void arcsinh(void);

void
eval_arcsinh(void)
{
	push(cadr(p1));
	eval();
	arcsinh();
}

static void
arcsinh(void)
{
	double d;
	save();
	p1 = pop();
	if (p1->k == DOUBLE) {
		d = p1->u.d;
		d = log(d + sqrt(d * d + 1.0));
		push_double(d);
		restore();
		return;
	}
	if (iszero(p1)) {
		push(_zero);
		restore();
		return;
	}
	push_symbol(ARCSINH);
	push(p1);
	list(2);
	restore();
}

static char *s[] = {

	"arcsinh(0.0)",
	"0",

	"arcsinh(0)",
	"0",
};

void
test_arcsinh(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
