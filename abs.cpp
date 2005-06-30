#include "stdafx.h"
#include "defs.h"
extern void conjugate(void);
extern int isnegativeterm(U *);
static void absval_f(void);
static void absval_tensor(void);
static int iscomplexnumber(U *);

void
eval_abs(void)
{
	push(cadr(p1));
	eval();
	absval();
}

void
absval(void)
{
	save();
	absval_f();
	restore();
}

static void
absval_f(void)
{
	p1 = pop();

	if (p1->k == TENSOR) {
		absval_tensor();
		return;
	}

	if (isnum(p1)) {
		push(p1);
		if (isnegativenumber(p1))
			negate();
		return;
	}

	if (iscomplexnumber(p1)) {
		push(p1);
		push(p1);
		conjugate();
		multiply();
		push_rational(1, 2);
		power();
		return;
	}

	if (isnegativeterm(p1) || (car(p1) == symbol(ADD) && isnegativeterm(cadr(p1)))) {
		push(p1);
		negate();
		p1 = pop();
	}

	push_symbol(ABS);
	push(p1);
	list(2);
}

static void
absval_tensor(void)
{
	if (p1->u.tensor->ndim != 1)
		stop("abs(tensor) with tensor rank > 1");
	push(p1);
	push(p1);
	conjugate();
	inner();
	push_rational(1, 2);
	power();
}

// returns 1 if there's -1 to a power somewhere

static int
iscomplex(U *p)
{
	if (car(p) == symbol(POWER) && isminusone(cadr(p)))
		return 1;

	if (iscons(p)) {
		while (iscons(p)) {
			if (iscomplex(car(p)))
				return 1;
			p = cdr(p);
		}
	}

	return 0;
}

static int
iscomplexnumber(U *p)
{
	if (iscomplex(p) && !issymbolic(p))
		return 1;
	else
		return 0;
}

static char *s[] = {

	"abs(2)",
	"2",

	"abs(2.0)",
	"2",

	"abs(-2)",
	"2",

	"abs(-2.0)",
	"2",

	"abs(a)",
	"abs(a)",

	"abs(-a)",
	"abs(a)",

	"abs(2*a)",
	"abs(2*a)",

	"abs(-2*a)",
	"abs(2*a)",

	"abs(2.0*a)",
	"abs(2*a)",

	"abs(-2.0*a)",
	"abs(2*a)",

	"abs(a-b)+abs(b-a)",
	"2*abs(a-b)",

	"abs(3 + 4 i)",
	"5",

	"abs((2,3,4))",
	"29^(1/2)",
};

void
test_abs(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
