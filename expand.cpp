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
#define R p7
#define P p8

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

	// get numerator and denominator

	push(F);
	numerator();
	A = pop();

	push(F);
	denominator();
	B = pop();

	// quotient Q

	push(A);
	push(B);
	push(X);
	divpoly();
	Q = pop();

	// remainder R = A - B * quotient(A/B)

	push(A);
	push(B);
	push(Q);
	multiply();
	subtract();
	R = pop();

	// the quotient will figure into it one way or the other

	push(Q);

	// if the remainder is zero then we're done

	if (iszero(R)) {
		restore();
		return;
	}

	// ok, now do the partial fraction expansion of R/B

	push(B);
	push(X);
	factorpoly();
	B = pop();

	// one or more factors

	if (car(B) == symbol(MULTIPLY)) {
		p1 = cdr(B);
		while (iscons(p1)) {
			F = car(p1);
			expand_pole();
			add();
			p1 = cdr(p1);
		}
	} else {
			F = B;
			expand_pole();
			add();
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

void
handle_multiple_poles(void)
{
	stop("handle_multiple_poles");
}

/* We have				example

	R	is the numerator	2s

	B	is the denominator	(s + 1)(s + 2)

	F	is the factor		s + 1

	X	dependent variable	s

Compute

	P	pole			1

		FR/B			2s / (s + 2)

		FR/B at X = -P		-2

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
	push(B);
	reciprocate();
	multiply_noexpand();
	push(R);
	multiply_noexpand();
	push(X);
	push(P);
	negate();
	subst();
	eval();
	push(F);
	divide();
}
