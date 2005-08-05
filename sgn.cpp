//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	sgn sign function
//
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void ysgn(void);
static int iscomplexnumber(U *);
extern int isnegativeterm(U *);

void
eval_sgn(void)
{
	push(cadr(p1));
	eval();
	sgn();
}

void
sgn(void)
{
	save();
	ysgn();
	restore();
}

#define X p1

void
ysgn(void)
{
	
	X = pop();

	
	if (p1->k == DOUBLE) {
		if (p1->u.d > 0) 
			{push_integer(1);
			 return;}
		else 
			if (p1->u.d == 0) 
				{push_integer(1);
				 return;}
			else
				{push_integer(-1);
				 return;}
	}

	if (p1->k == NUM) {
		if (MSIGN(mmul(p1->u.q.a,p1->u.q.b)) == -1) 
			{push_integer(-1);
			 return;}
		else 
			if (MZERO(mmul(p1->u.q.a,p1->u.q.b))) 
				{push_integer(0);
				 return;}
			else
				{push_integer(1);
				 return;}
	}

	if (iscomplexnumber(X)){
		push_integer(-1);
		push(X);
		absval();
		power();
		push(X);
		multiply();
		return;
	}
	
	
	if (isnegativeterm(X)) {
		push_symbol(SGN);
		push(X);
		negate();
		list(2);
		push_integer(-1);
		multiply();
		return;
	}
	
/*	push_integer(2);
	push(X);
	heaviside();
	multiply();
	push_integer(-1);
	add();	*/
	
	push_symbol(SGN);
	push(X);
	list(2);
}


static int
iscomplex(U *p)
{
	if (car(p) == symbol(POWER) && isminusone(cadr(p)))
		return 1;

	if (iscons(p)) {
		while (iscons(p)) {
			if (iscomplex(car(p)))
				return 1;
			p = cdr(p);
		}
	}

	return 0;
}

static int
iscomplexnumber(U *p)
{
	if (iscomplex(p) && !issymbolic(p))
		return 1;
	else
		return 0;
}


static char *s[] = {


	"sgn(-3)",
	"-1",
	

	"sgn(0)",
	"0",
	
	"sgn(3)",
	"1",

};

void
test_sgn(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}