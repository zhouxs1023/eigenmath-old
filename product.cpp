/*	Name:		product

	Syntax:		product(var, minval, maxval, expr)

	Input:		cadr(p1)	var

			caddr(p1)	minval

			cadddr(p1)	maxval

			caddddr(p1)	expr

	Output:		Product on stack

	Notes:		The original value of var is restored before the
			product function returns.

			Expr is quoted. It is not evaluated until var is
			assigned minval.
*/

#include "stdafx.h"
#include "defs.h"

#define F p1
#define X p2

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

void
product(void)
{
	int h, i, j, k;

	F = pop();
	k = pop_integer();
	j = pop_integer();
	X = pop();

	if (j == (int) 0x80000000)
		stop("2nd arg in product function: integer expected");

	if (k == (int) 0x80000000)
		stop("3rd arg in product function: integer expected");

	h = tos;

	for (i = j; i <= k; i++) {
		push(F);
		push(X);
		push_integer(i);
		subst();
		eval();
	}

	multiply_all(tos - h);
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
