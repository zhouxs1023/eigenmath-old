#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	simplify
//
//	Attempts to simplify an expression.
//
//-----------------------------------------------------------------------------

#include "defs.h"

void simplify(void);
static void simplify_tensor(void);
static void simplify_cons(void);
static int count(U *);
static void try_rationalizing(void);
static void try_factoring(void);
static void try_numden(void);
static void try_sin_cos(void);

void
eval_simplify(void)
{
	push(cadr(p1));
	eval();
	simplify();
}

// expression on stack

void
simplify(void)
{
	save();
	p1 = pop();
	if (istensor(p1))
		simplify_tensor();
	else if (iscons(p1))
		simplify_cons();
	else
		push(p1);
	restore();
}

static void
simplify_tensor(void)
{
	int i;
	p2 = alloc_tensor(p1->u.tensor->nelem);
	p2->u.tensor->ndim = p1->u.tensor->ndim;
	for (i = 0; i < p1->u.tensor->ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];
	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(p1->u.tensor->elem[i]);
		simplify();
		p2->u.tensor->elem[i] = pop();
	}
	push(p2);
	check_tensor();
}

static void
simplify_cons(void)
{
	int h = tos;
	push(car(p1));
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		simplify();
		p1 = cdr(p1);
	}
	list(tos - h);
	eval();
	p1 = pop();
	try_rationalizing();
	try_numden();
	try_factoring();
	try_sin_cos();
	push(p1);
}

static int
count(U *p)
{
	int n;
	if (iscons(p)) {
		n = 0;
		while (iscons(p)) {
			n += count(car(p)) + 1;
			p = cdr(p);
		}
	} else
		n = 1;
	return n;
}

//-----------------------------------------------------------------------------
//
//	For all these "try" functions...
//
//	Input:		p1		expression
//
//	Output:		p1		simplified expression
//
//			p2-p8		clobbered
//
//-----------------------------------------------------------------------------

static void
try_rationalizing(void)
{
	if (car(p1)->k != ADD)
		return;
	push(p1);
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

extern void condense(void);

static void
try_factoring(void)
{
	if (car(p1)->k != ADD)
		return;
	push(p1);
	condense();
	p2 = pop();
	if (count(p2) <= count(p1))
		p1 = p2;
}

// Example: (A-B)/(B-A) -> -1

static void
try_numden(void)
{
	int h, sign;

	if (car(p1)->k != MULTIPLY)
		return;

	// any add terms?

	p2 = cdr(p1);
	while (iscons(p2)) {
		if (caar(p2)->k == ADD)
			break;
		p2 = cdr(p2);
	}

	if (p2 == nil)
		return;

	// negate add terms

	h = tos;
	sign = 1;
	p2 = cdr(p1);
	push_symbol(MULTIPLY);
	while (iscons(p2)) {
		p3 = car(p2);
		push(p3);
		if (car(p3)->k == ADD) {
			negate_expand();
			sign *= -1;
		}
		p2 = cdr(p2);
	}

	list(tos - h);
	eval_noexpand();
	if (sign == -1)
		negate(); // use current expanding mode

	p2 = pop();

	if (count(p2) < count(p1))
		p1 = p2;
}

extern int trigmode;

static void
try_sin_cos(void)
{
	if (find(p1, symbol(SIN)) == 0 || find(p1, symbol(COS)) == 0)
		return;

//	push(p1);
//	expand();
//	p2 = pop();

	p2 = p1;

	trigmode = 1;
	push(p2);
	eval();
	p3 = pop();

	trigmode = 2;
	push(p2);
	eval();
	p4 = pop();

	trigmode = 0;

	if (count(p4) < count(p3))
		p3 = p4;

	if (count(p3) < count(p1))
		p1 = p3;
}

static char *s[] = {

	"simplify(A)",
	"A",

	"simplify(A+B)",
	"A+B",

	"simplify(A B)",
	"A*B",

	"simplify(A^B)",
	"A^B",

	"simplify(A/(A+B)+B/(A+B))",
	"1",

	"simplify((A-B)/(B-A))",
	"-1",

	"A=((A11,A12),(A21,A22))",
	"",

	"simplify(det(A) inv(A) - adj(A))",
	"0",

	"A=quote(A)",
	"",

	// this shows need for <= in try_factoring

	"x*(1+a)",
	"x+a*x",

	"simplify(last)",
	"x*(1+a)",

	"simplify(-3 exp(-1/3 r + i phi) cos(theta) / sin(theta)"
	" + 3 exp(-1/3 r + i phi) cos(theta) sin(theta)"
	" + 3 exp(-1/3 r + i phi) cos(theta)^3 / sin(theta))",
	"0",
};

void
test_simplify(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
