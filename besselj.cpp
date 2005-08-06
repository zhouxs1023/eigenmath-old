//-----------------------------------------------------------------------------
//
//	Bessel J function
//
//	Input:	tos-2		x	(can be a symbol or expr)
//
//			tos-1		n
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void __besselj(void);
extern int isnegativeterm(U *);

void
eval_besselj(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	besselj();
}

void
besselj(void)
{
	save();
	__besselj();
	restore();
}

#define X p1
#define N p2

void
__besselj(void)
{
	double d;
	int n;

	N = pop();
	X = pop();

	if (isdouble(X) && (isinteger(N))) {
		push(N);
		n = pop_integer();
		d = jn(n,X->u.d);
		push_double(d);
		return;
	}

	if (isnegativeterm(X)) {
		push(X);
		negate();
		push(N);
		power();
		push(X);
		push(N);
		negate();
		power();
		multiply();
		push_symbol(BESSELJ);
		push(X);
		negate();
		push(N);
		list(3);
		multiply();
		return;
	}

	if (isnegativeterm(N)) {
		push_integer(-1);
		push(N);
		power();
		push_symbol(BESSELJ);
		push(X);
		push(N);
		negate();
		list(3);
		multiply();
		return;
	}

	push_symbol(BESSELJ);
	push(X);
	push(N);
	list(3);
	return;

}


static char *s[] = {

	"besselj(x,n)",
	"besselj(x,n)",

};

void
test_besselj(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
