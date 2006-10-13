/*	Name:		sum

	Syntax:		sum(var, minval, maxval, expr)

	Input:		cadr(p1)	var

			caddr(p1)	minval

			cadddr(p1)	maxval

			caddddr(p1)	expr

	Output:		Sum on stack

	Notes:		The original value of var is restored before the sum
			function returns.

			Expr is quoted. It is not evaluated until var is
			assigned minval.
*/

#include "stdafx.h"
#include "defs.h"

void
eval_sum(void)
{
	push(cadr(p1)); /* index expr is quoted */
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	push(caddddr(p1));
	sum();
}

#define F p1
#define X p2

void
sum(void)
{
	int h, i, j, k;

	F = pop();
	k = pop_integer();
	j = pop_integer();
	X = pop();

	if (j == (int) 0x80000000)
		stop("2nd arg in sum function: integer expected");

	if (k == (int) 0x80000000)
		stop("3rd arg in sum function: integer expected");

	h = tos;

	for (i = j; i <= k; i++) {
		push(F);
		push(X);
		push_integer(i);
		subst();
		eval();
	}

	add_all(tos - h);
}

static char *s[] = {

	"sum(n,0,10,(-1/3)^n)",
	"44287/59049",

	/* ensure index expr is quoted */

	"sum(i,1,3,i)",
	"6",
};

void
test_sum(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
