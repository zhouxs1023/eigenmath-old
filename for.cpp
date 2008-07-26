// Evaluate the 'for' function

#include "stdafx.h"
#include "defs.h"

void
eval_for(void)
{
	int i, j, k;

	// 1st arg (quoted)

	p3 = cadr(p1);

	// 2nd arg

	push(caddr(p1));
	eval();
	j = pop_integer();
	if (j == (int) 0x80000000)
		stop("for: 2nd arg?");

	// 3rd arg

	push(cadddr(p1));
	eval();
	k = pop_integer();
	if (k == (int) 0x80000000)
		stop("for: 3rd arg?");

	// stmt list

	p1 = cddddr(p1);

	for (i = j; i <= k; i++) {
		p2 = p1;
		while (iscons(p2)) {
			push(car(p2));
			push(p3);
			push_integer(i);
			subst();
			eval();
			pop();
			p2 = cdr(p2);
		}
	}

	push(symbol(NIL));
}
