#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Find the degree of a polynomial
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Result on stack
//
//	Note: Finds the largest numerical power of x. Does not check for
//	weirdness in p(x).
//
//-----------------------------------------------------------------------------

#include "defs.h"

static void degree_f(U *);

#define POLY p1
#define X p2
#define DEGREE p3

void
degree(void)
{
	save();
	X = pop();
	POLY = pop();
	DEGREE = zero;
	degree_f(POLY);
	push(DEGREE);
	restore();
}

static void
degree_f(U *p)
{
	if (equal(p, X)) {
		if (iszero(DEGREE))
			DEGREE = one;
	} else if (car(p) == symbol(POWER)) {
		if (equal(cadr(p), X) && isnum(caddr(p)) && lessp(DEGREE, caddr(p)))
			DEGREE = caddr(p);
	 } else if (iscons(p)) {
		p = cdr(p);
		while (iscons(p)) {
			degree_f(car(p));
			p = cdr(p);
		}
	}
}
