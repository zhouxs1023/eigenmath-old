// Evaluate the 'for' function

#include "stdafx.h"
#include "defs.h"

void
eval_for(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	push(cddddr(p1));
	for_function();
}

/*	Input:		tos-4		Index expression

			tos-3		Initial value

			tos-2		Final value

			tos-1		Statement list

	Output:		Result on stack
*/

#define A p1
#define B p2
#define T p3

void
for_function(void)
{
	int i, j, k;

	save();

	B = pop();
	k = pop_integer();
	j = pop_integer();
	A = pop();

	if (j == (int) 0x80000000)
		stop("2nd arg of \"for\" function: integer expected");

	if (k == (int) 0x80000000)
		stop("3rd arg of \"for\" function: integer expected");

	for (i = j; i <= k; i++) {
		T = B;
		while (iscons(T)) {
			push(car(T));
			push(A);
			push_integer(i);
			subst();
			eval();
			pop();
			T = cdr(T);
		}
	}

	push(symbol(NIL));

	restore();
}

static char *s[] = {

	"x=0",
	"",

	"y=2",
	"",

	"for(k,1,9,x=sqrt(2+x),y=2y/x)",
	"",

	"float(y)",
	"3.14159",
};

void
test_for(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
