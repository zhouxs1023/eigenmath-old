//-----------------------------------------------------------------------------
//
//	Name:		sum
//
//	Syntax:		sum(var, minval, maxval, expr)
//
//	Input:		cadr(p1)	var
//
//			caddr(p1)	minval
//
//			cadddr(p1)	maxval
//
//			caddddr(p1)	expr
//
//	Output:		Sum on stack
//
//	Notes:		The original value of var is restored before the sum
//			function returns.
//
//			Expr is quoted. It is not evaluated until var is
//			assigned minval.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void sum(void);

#define F p1
#define X p2

// here from eval.c

void
eval_sum(void)
{
	push(cadr(p1));
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	push(caddddr(p1));
	sum();
}

static void
sum(void)
{
	int i, j, k;

	F = pop();
	k = pop_integer();
	j = pop_integer();
	X = pop();

	if (!issymbol(X))
		stop("1st arg in sum function: symbol expected");

	if (j == (int) 0x80000000)
		stop("2nd arg in sum function: integer expected");

	if (k == (int) 0x80000000)
		stop("3rd arg in sum function: integer expected");

	push_integer(0);

	for (i = j; i <= k; i++) {
		push(F);
		push(X);
		push_integer(i);
		evalat();
		add();
	}
}

static char *s[] = {

	"n=quote(n)",
	"",

	"sum(n,1,3,n)",
	"6",

	"n",
	"n",

	"sum(n,1,100,isprime(n))",
	"25",
};

void
test_sum(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
