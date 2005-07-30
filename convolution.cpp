//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	convolution product
//
//	Input:		tos-2		x	(can be a symbol or expr)
//
//				tos-1		y	(can be a symbol or expr)
//
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void yconvolution(void);

void
eval_convolution(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	convolution();
}

void
convolution(void)
{
	save();
	yconvolution();
	restore();
}

#define X p1
#define Y p2

static void
yconvolution(void)
{
	
	Y = pop();
	X = pop();
	
	if ((X==0)||(Y==0)) {
		push_integer(0);
		return;
	}
	
	if (car(X) == symbol(DIRAC)) {
		push(Y);
		return;
	}

	if (car(Y) == symbol(DIRAC)) {
		push(X);
		return;
	}

	if (car(X) == symbol(DERIVATIVE)) {
		push_symbol(CONVOLUTION);
		push(cadr(X));
		push(Y);
		push(caddr(X));
		derivative();
		list(3);
		return;
	}

	if (car(Y) == symbol(DERIVATIVE)) {
		push_symbol(CONVOLUTION);
		push(cadr(Y));
		push(X);
		push(caddr(Y));
		derivative();
		list(3);
		return;
	}

		
	push_symbol(CONVOLUTION);
	push(X);
	push(Y);
	list(3);
	return;
	
}


static char *s[] = {
	"convolution(dirac(x),f(x))",
	"f(x)",
	
	"eval(convolution(d(dirac(x),x),f(x)))",
	"d(f(x),x)",
	
};

void
test_convolution(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
