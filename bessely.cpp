//-----------------------------------------------------------------------------
//
//	Bessel Y function
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
static void __bessely(void);
extern int isnegativeterm(U *);

void
eval_bessely(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	bessely();
}

void
bessely(void)
{
	save();
	__bessely();
	restore();
}

#define X p1
#define N p2

void
__bessely(void)
{
	double d;
	int n;

	N = pop();
	X = pop();

	push(N);
	n = pop_integer();

	if ((X->k == DOUBLE) && (isinteger(N))) {
		d = yn(n,X->u.d);
		push_double(d);
		return;
	}
	
	if (isnegativeterm(N)) {
		push_integer(-1);
		push(N);
		power();
		push_symbol(BESSELY);
		push(X);
		push(N);
		negate();
		list(3);
		multiply();
		return;
	}


	
	push_symbol(BESSELY);
	push(X);
	push(N);
	list(3);
	return;

}


static char *s[] = {

	"bessely(x,n)",
	"bessely(x,n)",


};

void
test_bessely(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
