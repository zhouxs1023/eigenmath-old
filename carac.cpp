//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	Characteristic set function carac(x,a,b)
//
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

void
eval_carac(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	carac();
}

void
carac(void)
{
	save();
	yycarac();
	restore();
}

#define X p1
#define A p2
#define B p3

void
yycarac(void)
{
	B = pop();
	A = pop();
	X = pop();
	
	if (isdouble(X) && isdouble(A) && isdouble(B)) {
		if (X->u.d > B->u.d && X->u.d < A->u.d) 
			{push_integer(1);
			 return;}
		else 
			if (X->u.d == B->u.d || X->u.d == A->u.d) 
				{push_rational(1, 2);
				 return;}
			else
				{push_integer(0);
				 return;}
	}

	if (isrational(X) && isrational(A) && isrational(B)) {
		if (MSIGN(msub(mmul(X->u.q.b,A->u.q.a),mmul(X->u.q.a,A->u.q.b))) == -1
			&&
			MSIGN(msub(mmul(X->u.q.a,B->u.q.b),mmul(X->u.q.b,B->u.q.a))) == -1) 
			{push_integer(1);
			 return;}
		else 
			if (MZERO(msub(mmul(X->u.q.b,A->u.q.a),mmul(X->u.q.a,A->u.q.b)))
				||
				MZERO(msub(mmul(X->u.q.a,B->u.q.b),mmul(X->u.q.b,B->u.q.a)))) 
				{push_rational(1, 2);
				 return;}
			else
				{push_integer(0);
				 return;}
	}

	if (expomode == 1) {
		push(X);
		push_integer(-1);
		push(A);
		multiply();		
		add();
		heaviside();
		push(X);
		push_integer(-1);
		push(B);
		multiply();		
		add();
		heaviside();
		push_integer(-1);
		multiply();
		add();
		return;
	}

		
	if (isnegativeterm(X)) {
		push_symbol(CARAC);
		push(X);
		negate();
		push(A);
		push(B);
		list(4);
		return;
	}
	 
	push_symbol(CARAC);
	push(X);
	push(A);
	push(B);
	list(4);
}

static char *s[] = {

	
	"carac(-2,-1,1)",
	"0",
	
	"carac(-1,-1,1)",
	"1/2",

	"carac(0,-1,1)",
	"1",
	
	"carac(1,-1,1)",
	"1/2",
	
	"carac(2,-1,1)",
	"0",


};

void
test_carac(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

