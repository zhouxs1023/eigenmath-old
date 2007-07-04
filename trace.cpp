//	Compute the trace of a matrix (sum of diagonal elements)

#include "stdafx.h"
#include "defs.h"
static void ytrace(void);

void
eval_trace(void)
{
	push(cadr(p1));
	eval();
	trace();
}

void
trace(void)
{
	save();
	ytrace();
	restore();
}

static void
ytrace(void)
{
	int i, n;

	p1 = pop();

	// trace of scalar is ok even though contract(scalar) is not

	if (isnum(p1)) {
		push(p1);
		return;
	}

	if (!istensor(p1)) {
		push_symbol(TRACE);
		push(p1);
		list(2);
		return;
	}

	if (p1->u.tensor->ndim != 2 || p1->u.tensor->dim[0] != p1->u.tensor->dim[1])
		stop("trace(m): m is not a square matrix");

	n = p1->u.tensor->dim[0];

	for (i = 0; i < n; i++)
		push(p1->u.tensor->elem[n * i + i]);

	add_all(n);
}

#if SELFTEST

static char *s[] = {

	"trace(A)",
	"trace(A)",

	"trace(0)",
	"0",

	"trace(1)",
	"1",

	"trace(((a,b),(c,d)))",
	"a+d",

	"trace((a,b))",
	"Stop: trace(m): m is not a square matrix",
};

void
test_trace(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif