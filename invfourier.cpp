//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	Inverse Fourier transform
//
//	The formalism used is : invfourier(f(x),x)=fourier(f(-x),x)/(2*pi)
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void yinvfourier(void);

void
eval_invfourier(void)
{
	push(cadr(p1));
	eval();
	if (caddr(p1) == symbol(NIL))
		guess();
	else {
		push(caddr(p1));
		eval(); 
	}
	invfourier();
}

void
invfourier(void)
{
	save();
	yinvfourier();
	eval();
	restore();
}

static void
yinvfourier(void)
{
	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	push(p2);
	negate();
	subst();
	eval();
	push(p2);
	fourier();
	push_symbol(PI);
	push_integer(2);
	multiply();
	divide();
}

