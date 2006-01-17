//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	tchebychevT polynomial T(x,n+1)=2*x*T(n,x)-T(x,n-1) T(x,0)=1,T(x,1)=x
//
//	Input:		tos-2		x	(can be a symbol or expr)
//
//			tos-1		n
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

void
eval_tchebychevT(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	tchebychevT();
}

void
tchebychevT(void)
{
	save();
	yytchebychevT();
	restore();
}

#define X p1
#define N p2
#define Y p3
#define Y1 p4
#define Y0 p5

void
yytchebychevT(void)
{
	int n;

	N = pop();
	X = pop();

	push(N);
	n = pop_integer();

	if (n < 0) {
		push_symbol(TCHEBYCHEVT);
		push(X);
		push(N);
		list(3);
		return;
	}
	if (n == 0) {
		push_integer(1);
		return;
	}
	if (issymbol(X))
		yytchebychevT2(n-1);
	else {
		Y = X;			// do this when X is an expr
		X = symbol(SECRETX);
		yytchebychevT2(n-1);
		X = Y;
		push(symbol(SECRETX));
		push(X);
		subst();
		eval();
	}
}

void
yytchebychevT2(int n)
{
	int i;

	push(X);
	push_integer(1);
	Y1 = pop();
	for (i = 0; i < n; i++) {
		Y0 = Y1;

		Y1 = pop();

		push(X);
		push(Y1);
		multiply();
		
		push_integer(2);
		multiply();

		push(Y0);

		subtract();

	}
}

static char *s[] = {


	"tchebychevT(x,n)",
	"tchebychevT(x,n)",

	"tchebychevT(x,0)-1",
	"0",

	"tchebychevT(x,1)-x",
	"0",

	"tchebychevT(x,2)-(2x^2-1)",
	"0",

	"tchebychevT(x,3)-(4*x^3-3*x)",
	"0",

	"tchebychevT(x,4)-(8*x^4-8*x^2+1)",
	"0"

};

void
test_tchebychevT(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
