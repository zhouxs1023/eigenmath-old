// Try to simplify an expression.

#include "stdafx.h"
#include "defs.h"

extern int trigmode;

void simplify(void);
static void simplify_tensor(void);
static int count(U *);
static void f1(void);
static void f2(void);
static void f3(void);
static void f4(void);
static void f5(void);
static void f9(void);

// to here from eval.cpp

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
	else {
		f1();
		f2();
		f3();
		f4();
		f5();
		f9();
		push(p1);
	}
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

// try rationalizing

static void
f1(void)
{
	if (car(p1)->k != ADD)
		return;
	push(p1);
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// try condensing

static void
f2(void)
{
	if (car(p1)->k != ADD)
		return;
	push(p1);
	condense();
	p2 = pop();
	if (count(p2) <= count(p1))
		p1 = p2;
}

// this simplifies forms like (A-B) / (B-A)

static void
f3(void)
{
	push(p1);
	rationalize();
	negate();
	rationalize();
	negate();
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// try expanding denominators

static void
f4(void)
{
	if (iszero(p1))
		return;
	push(p1);
	rationalize();
	inverse();
	rationalize();
	inverse();
	rationalize();
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
}

// simplifies trig forms

static void
f5(void)
{
	if (find(p1, symbol(SIN)) == 0 || find(p1, symbol(COS)) == 0)
		return;

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

// if it's a sum then try to simplify each term

static void
f9(void)
{
	if (car(p1) != symbol(ADD))
		return;
	push_integer(0);
	p2 = cdr(p1);
	while (iscons(p2)) {
		push(car(p2));
		simplify();
		add();
		p2 = cdr(p2);
	}
	p2 = pop();
	if (count(p2) < count(p1))
		p1 = p2;
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

	"simplify((A^2 C^2 + A^2 D^2 + B^2 C^2 + B^2 D^2)/(A^2+B^2)/(C^2+D^2))",
	"1",

	"simplify(d(arctan(y/x),y))",
	"x/(x^2+y^2)",

	"simplify(d(arctan(y/x),x))",
	"-y/(x^2+y^2)",
};

void
test_simplify(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

