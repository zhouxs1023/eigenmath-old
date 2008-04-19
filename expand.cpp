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

#define A p2
#define B p3
#define F p4
#define P p5
#define Q p6
#define T p7
#define X p8

void
expand(void)
{
	save();

	X = pop();
	F = pop();

	if (istensor(F)) {
		expand_tensor();
		restore();
		return;
	}

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

	// A = denominator (denominator function expands)

	push(F);
	denominator();
	A = pop();

	// quotient

	push(B);
	push(A);
	push(X);
	divpoly();
	Q = pop();

	// remainder B = B - A * quotient(B, A)

	push(B);
	push(A);
	push(Q);
	multiply();
	subtract();
	B = pop();

	// if the remainder is zero then we're done

	if (iszero(B)) {
		push(Q);
		restore();
		return;
	}

	factor_denominator();

	// accumulate fractions

	push(Q);

	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			F = car(p1);
			do_expansion_factor();
			p1 = cdr(p1);
		}
	} else {
			F = A;
			do_expansion_factor();
	}

	restore();
}

void
expand_tensor(void)
{
	int i;
	push(F);
	copy_tensor();
	F = pop();
	for (i = 0; i < F->u.tensor->nelem; i++) {
		push(F->u.tensor->elem[i]);
		push(X);
		expand();
		F->u.tensor->elem[i] = pop();
	}
	push(F);
}

// The correct way to factor
//
//	  2
//	6x  + 5x + 1
//
// is
//	(2x + 1) * (3x + 1)
//
// and that is how factorpoly() works.
//
// However, for this application we would prefer the factorization to be
//
//	6 * (x + 1/2) * (x + 1/3)

void
factor_denominator(void)
{
	push(A);
	push(X);
	factorpoly();
	A = pop();

	push_integer(1);

	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			A = car(p1);
			factor_denominator_1();
			p1 = cdr(p1);
		}
	} else
		factor_denominator_1();

	A = pop();
}

void
factor_denominator_1(void)
{
	int h, n;

	if (find(A, X) == 0) {
		push(A);
		multiply_noexpand();
		return;
	}

	if (car(A) == symbol(POWER)) {

		push(caddr(A));
		n = pop_integer();
		A = cadr(A);

		h = tos;	// T = coeff. of leading term
		push(A);
		push(X);
		coeff();
		T = pop();
		tos = h;

		push(T);
		push_integer(n);
		power();
		multiply_noexpand();

		push(symbol(POWER));
		push(A);
		push(T);
		divide();
		push_integer(n);
		list(3);

		multiply_noexpand();

		return;
	}

	h = tos;	// T = coeff. of leading term
	push(A);
	push(X);
	coeff();
	T = pop();
	tos = h;

	push(T);
	multiply_noexpand();

	push(A);
	push(T);
	divide();
	multiply_noexpand();
}

//	F is the factor, like (x + 3) or (x + 1) ^ 2

void
do_expansion_factor(void)
{
	// constant factors can be ignored, they're already in A

	if (find(F, X) == 0)
		return;

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

	// T = F B/A

	push(F);
	push(A);
	reciprocate();
	multiply_noexpand();
	push(B);
	multiply_noexpand();
	T = pop();

	// get n, base factor and pole

	push(caddr(F));
	n = pop_integer();
	F = cadr(F);
	get_pole();

	// first result

	push(T);
	push(X);
	push(P);
	subst();
	eval();
	push(F);
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

		push(F);		// divide by (X - P) ^ n
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
	get_pole();

	push(F);
	push(A);
	reciprocate();
	multiply_noexpand();	// F is  a sum, do not expand, just cancel in A
	push(B);
	multiply_noexpand();
	push(X);
	push(P);
	subst();
	eval();
	push(F);
	divide();

	// accumulate fractions

	add();
}

// Input:
//
//	F = ax + b
//
// Outputs
//
//	P = -b/a

void
get_pole(void)
{
	push(F);		// P = b
	push(X);
	push_integer(0);
	subst();
	eval();
	P = pop();

	push(F);		// a + b (on stack)
	push(X);
	push_integer(1);
	subst();
	eval();

	push(P);		// -a (on stack)
	swap();
	subtract();

	push(P);		// P = -b/a
	swap();
	divide();
	P = pop();
}

#if SELFTEST

static char *s[] = {

	"expand(1/(x+1)/(x+2))",
	"1/(x+1)-1/(x+2)",

	"expand((2x^3-x+2)/(x^2-2x+1))",
	"4+2*x+5/(x-1)+3/(x^2-2*x+1)",

	"expand(1/x^2/(x-1))",
	"-1/(x^2)-1/x+1/(x-1)",

	// ensure denominators are expanded (result seems preferable that way)

	"q=(x-1)(x-2)^3",
	"",

	"expand(1/q)",
	"1/(x^3-6*x^2+12*x-8)+1/(x-2)-1/(x-1)-1/(x^2-4*x+4)",

	// fractional poles

	"expand(1/(x+1/2)/(x+1/3))",
	"6/(x+1/3)-6/(x+1/2)",

	// expand tensor

	"f=1/(x+1)/(x+2)",
	"",

	"g=1/(x+1)-1/(x+2)",
	"",

	"expand(((f,f),(f,f)))-((g,g),(g,g))",
	"((0,0),(0,0))",
};

void
test_expand(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
