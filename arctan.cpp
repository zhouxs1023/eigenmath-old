#include "stdafx.h"

#include "defs.h"

void
eval_arctan(void)
{
	push(cadr(p1));
	eval();
	arctan();
}

void
arctan(void)
{
	double d;

	save();

	p1 = pop();

	if (p1->k == DOUBLE) {
		errno = 0;
		d = atan(p1->u.d);
		if (errno)
			stop("arctan function error");
		push_double(d);
		restore();
		return;
	}

	if (iszero(p1)) {
		push(zero);
		restore();
		return;
	}

	if (car(p1) == symbol(TAN)) {
		push(cadr(p1));
		restore();
		return;
	}

	push_symbol(ARCTAN);
	push(p1);
	list(2);

	restore();
}

static char *s[] = {

	"arctan(x)",
	"arctan(x)",

	"arctan(0)",
	"0",

	"arctan(tan(x))",
	"x",
};

void
test_arctan(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
