//-----------------------------------------------------------------------------
//
//	Name:		product
//
//	Syntax:		product(var, minval, maxval, expr)
//
//	Input:		cadr(p1)	var
//
//			caddr(p1)	minval
//
//			cadddr(p1)	maxval
//
//			caddddr(p1)	expr
//
//	Output:		Product on stack
//
//	Notes:		The original value of var is restored before the
//			product function returns.
//
//			Expr is quoted. It is not evaluated until var is
//			assigned minval.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void product(void);

#define F p1
#define X p2

// here from eval.c

void
eval_product(void)
{
	push(cadr(p1));
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	push(caddddr(p1));
	product();
}

static void
product(void)
{
	int i, j, k;

	F = pop();
	k = pop_integer();
	j = pop_integer();
	X = pop();

	if (!issymbol(X))
		stop("1st arg in product function: symbol expected");

	if (j == (int) 0x80000000)
		stop("2nd arg in product function: integer expected");

	if (k == (int) 0x80000000)
		stop("3rd arg in product function: integer expected");

	push_integer(1);

	for (i = j; i <= k; i++) {
		push(F);
		push(X);
		push_integer(i);
		evalat();
		multiply();
	}
}

static char *s[] = {

	"n=quote(n)",
	"",

	"product(n,1,4,n)",
	"24",

	"n",
	"n",
};

void
test_product(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
