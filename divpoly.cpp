#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Divide polynomials
//
//	Input:		tos-3		Dividend
//
//			tos-2		Divisor
//
//			tos-1		x
//
//	Output:		tos-2		Quotient
//
//			tos-1		Remainder
//
//-----------------------------------------------------------------------------

#include "defs.h"

#define DIVIDEND p1
#define DIVISOR p2
#define X p3
#define Q p4

void
divpoly(void)
{
	int h, i, m, n, x;
	U **dividend, **divisor;

	save();

	X = pop();
	DIVISOR = pop();
	DIVIDEND = pop();

	h = tos;

	dividend = stack + tos;

	push(DIVIDEND);
	push(X);
	m = coeff() - 1;	// m is dividend's power

	divisor = stack + tos;

	push(DIVISOR);
	push(X);
	n = coeff() - 1;	// n is divisor's power

	x = m - n;

	push(_zero);

	while (x >= 0) {

		push(dividend[m]);
		push(divisor[n]);
		divide();
		Q = pop();

		for (i = 0; i <= n; i++) {
			push(dividend[x + i]);
			push(divisor[i]);
			push(Q);
			multiply();
			subtract();
			dividend[x + i] = pop();
		}

		push(Q);
		push(X);
		push_integer(x);
		power();
		multiply();
		add();

		m--;
		x--;
	}

	restore();
}
