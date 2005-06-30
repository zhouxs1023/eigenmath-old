#include "stdafx.h"
#include "defs.h"
extern void sort_stack(int);
void roots(void);
static void roots2(void);
static void roots3(void);
static void iguess(void);
static void mini_solve(void);
#define POLY p1
#define X p2
#define A p3
#define B p4
#define C p5
#define Y p6

void
eval_roots(void)
{
	// A == B -> A - B

	p2 = cadr(p1);

	if (car(p2) == symbol(SETQ) || car(p2) == symbol(TESTEQ)) {
		push(cadr(p2));
		eval();
		push(caddr(p2));
		eval();
		subtract();
	} else {
		push(p2);
		eval();
		p2 = pop();
		if (car(p2) == symbol(SETQ) || car(p2) == symbol(TESTEQ)) {
			push(cadr(p2));
			eval();
			push(caddr(p2));
			eval();
			subtract();
		} else
			push(p2);
	}

	if (caddr(p1) == nil) {
		p1 = pop();
		iguess();
		push(p1);
		push(p2);
	} else {
		push(caddr(p1));
		eval();
	}

	roots();
}

static void
iguess(void)
{
	p2 = symbol(SYMBOL_X);
	if (find(p1,p2))
		return;
	p2 = symbol(SYMBOL_Y);
	if (find(p1,p2))
		return;
	p2 = symbol(SYMBOL_Z);
	if (find(p1,p2))
		return;
	p2 = symbol(SYMBOL_T);
	if (find(p1,p2))
		return;
	p2 = symbol(SYMBOL_R);
	if (find(p1,p2))
		return;
	p2 = symbol(SYMBOL_X);
}

void
roots(void)
{
	int h, i, n;
	h = tos - 2;
	roots2();
	n = tos - h;
	if (n == 0) {
		printstr("roots: Sorry, the argument is not factorable over integers, no roots found.\n");
		push(nil);
		return;
	}
	if (n == 1)
		return;
	sort_stack(n);
	save();
	p1 = alloc_tensor(n);
	p1->u.tensor->ndim = 1;
	p1->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[i] = stack[h + i];
	tos = h;
	push(p1);
	restore();
}

static void
roots2(void)
{
	save();

	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	factorpoly();

	p1 = pop();

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			roots3();
			p1 = cdr(p1);
		}
	} else {
		push(p1);
		push(p2);
		roots3();
	}

	restore();
}

static void
roots3(void)
{
	save();
	p2 = pop();
	p1 = pop();
	if (car(p1) == symbol(POWER) && ispoly(cadr(p1), p2) && isposint(caddr(p1))) {
		push(cadr(p1));
		push(p2);
		mini_solve();
	} else if (ispoly(p1, p2)) {
		push(p1);
		push(p2);
		mini_solve();
	}
	restore();
}

//-----------------------------------------------------------------------------
//
//	Input:		stack[tos - 2]		polynomial
//
//			stack[tos - 1]		dependent symbol
//
//	Output:		stack			roots on stack
//
//						(input args are popped first)
//
//-----------------------------------------------------------------------------

static void
mini_solve(void)
{
	int n;

	save();

	X = pop();
	POLY = pop();

	push(POLY);
	push(X);

	n = coeff();

	// AX + B, X = -B/A

	if (n == 2) {
		A = pop();
		B = pop();
		push(B);
		push(A);
		divide();
		negate();
		restore();
		return;
	}

	// AX^2 + BX + C, X = (-B +/- (B^2 - 4AC)^(1/2)) / (2A)

	if (n == 3) {
		A = pop();
		B = pop();
		C = pop();
		push(B);
		push(B);
		multiply();
		push_integer(4);
		push(A);
		multiply();
		push(C);
		multiply();
		subtract();
		push_rational(1, 2);
		power();
		Y = pop();
		push(Y);			// 1st root
		push(B);
		subtract();
		push(A);
		divide();
		push_rational(1, 2);
		multiply();
		push(Y);			// 2nd root
		push(B);
		add();
		negate();
		push(A);
		divide();
		push_rational(1, 2);
		multiply();
		restore();
		return;
	}

	tos -= n;

	restore();
}

static char *s[] = {

	"roots(x)",
	"0",

	"roots(x^2)",
	"0",

	"roots(x^3)",
	"0",

	"roots(2 x)",
	"0",

	"roots(2 x^2)",
	"0",

	"roots(2 x^3)",
	"0",

	"roots(6+11*x+6*x^2+x^3)",
	"(-3,-2,-1)",

	"roots(a*x^2+b*x+c)",
	"(-b/(2*a)-(-4*a*c+b^2)^(1/2)/(2*a),-b/(2*a)+(-4*a*c+b^2)^(1/2)/(2*a))",

	"roots(3+7*x+5*x^2+x^3)",
	"(-3,-1)",

	"roots(x^3+x^2+x+1)",
	"(-1,-i,i)",

	"roots(x^4+1)",
	"roots: Sorry, the argument is not factorable over integers, no roots found.",

	"roots(0)",
	"roots: Sorry, the argument is not factorable over integers, no roots found.",

	"roots(x^2==1)",
	"(-1,1)",

	"roots(3 x + 12 == 24)",
	"4",

	"roots(3/x + 12 = 24)",
	"1/4",
};

void
test_roots(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
