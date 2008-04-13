#include "stdafx.h"
#include "defs.h"

void
eval_expand(void)
{
	// 1st arg

	push(cadr(p1));
	eval();

	// 2nd arg

	push(caddr(p1));
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		guess();
	else
		push(p2);

	expand();
}

#define F p2
#define X p3
#define A p4
#define B p5
#define Q p6
#define P p7
#define T p8

void
expand(void)
{
	save();

	X = pop();
	F = pop();

	// if sum of terms then sum over the expansion of each term

	if (car(F) == symbol(ADD)) {
		push_integer(0);
		p1 = cdr(F);
		while (iscons(p1)) {
			push(car(p1));
			push(X);
			expand();
			add();
			p1 = cdr(p1);
		}
		restore();
		return;
	}

	// B = numerator

	push(F);
	numerator();
	B = pop();

	// A = denominator

	push(F);
	denominator();
	A = pop();

	// quotient Q

	push(B);
	push(A);
	push(X);
	divpoly();
	Q = pop();

	// remainder B = B - A * quotient(B/A)

	push(B);
	push(A);
	push(Q);
	multiply();
	subtract();
	B = pop();

	// start the accumulator

	push(Q);

	// if the remainder is zero then we're done

	if (iszero(B)) {
		restore();
		return;
	}

	// now do the partial fraction expansion of B/A

	push(A);
	push(X);
	factorpoly();
	A = pop();

	// one or more factors

	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			F = car(p1);
			expand_pole();
			p1 = cdr(p1);
		}
	} else {
			F = A;
			expand_pole();
	}

	restore();
}

//	F	is a factor like (x + 3) or (x + 1)^2

void
expand_pole(void)
{
	if (car(F) == symbol(POWER))
		handle_multiple_poles();
	else
		handle_single_pole();
}

/* For example

	B	=	1

	A	=	(s+1)(s+2)^3

	F	=	(s+2)^3

*/

void
handle_multiple_poles(void)
{
	int i, n;

	// n = exponent of factor F

	push(caddr(F));
	n = pop_integer();

	// P = negative of pole

	push(X);
	push(cadr(F));
	subtract();
	P = pop();

	// T = F B/A

	push(F);
	push(A);
	reciprocate();
	multiply_noexpand();
	push(B);
	multiply_noexpand();
	T = pop();

	// first result

	push(T);
	push(X);
	push(P);
	subst();
	eval();
	push(cadr(F));
	push_integer(n);		// expand the denominator
	power();
	reciprocate();
	multiply_noexpand();
	add();

	for (i = 1; i < n; i++) {

		push(T);		// T = dT / dX
		push(X);
		derivative();
		T = pop();

		push(T);		// evaluate T at P
		push(X);
		push(P);
		subst();
		eval();

		push(cadr(F));		// divide by (X - P) ^ n
		push_integer(n - i);
		power();
		reciprocate();
		multiply_noexpand();

		push_integer(i);	// multiply by 1 / n!
		factorial();
		reciprocate();
		multiply_noexpand();

		add();			// accumulate
	}
}

/* We have				example

	B	is the numerator	2s

	A	is the denominator	(s + 1)(s + 2)

	F	is the factor		s + 1

	X	dependent variable	s

Compute

	P	pole			1

		FB/A			2s / (s + 2)

		FB/A at X = -P		-2

		divide by F		-2 / (s + 1)

Return the result on the stack. */

void
handle_single_pole(void)
{
	push(F);
	push(X);
	subtract();
	P = pop();
	push(F);
	push(A);
	reciprocate();
	multiply_noexpand();
	push(B);
	multiply_noexpand();
	push(X);
	push(P);
	negate();
	subst();
	eval();
	push(F);
	divide();

	// accumulate

	add();
}

#if SELFTEST

static char *s[] = {

	"expand(1/(x+1)/(x+2))",
	"1/(x+1)-1/(x+2)",

	"expand((2x^3-x+2)/(x^2-2x+1))",
	"4+2*x+5/(x-1)+3/(x^2-2*x+1)",

	// ensure denominators are expanded (result seems preferable that way)

	"q=(x-1)(x-2)^3",
	"",

	"expand(1/q)",
	"1/(x^3-6*x^2+12*x-8)+1/(x-2)-1/(x-1)-1/(x^2-4*x+4)",
};

void
test_expand(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
