// Return the leading coefficient of a polynomial.
//
// Example
//
//	push(P);
//	push(X);
//	leading();
//	K = pop();
//
// The result is undefined if P is not a polynomial.

#include "stdafx.h"
#include "defs.h"

void
eval_leading(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p1 = pop();
	if (p1 == symbol(NIL))
		guess();
	else
		push(p1);
	leading();
}

#define P p1
#define X p2
#define N p3

void
leading(void)
{
	save();

	X = pop();
	P = pop();

	push(P);
	push(X);
	degree();
	N = pop();

	push(P);
	push(X);
	push(N);
	power();
	divide();

	push(X);
	filter();

	restore();
}
