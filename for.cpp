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

#if SELFTEST

static char *s[] = {

	"x=0",
	"",

	"y=2",
	"",

	"for(k,1,9,x=sqrt(2+x),y=2y/x)",
	"",

	"float(y)",
	"3.14159",

	/* ensure index expr is quoted */

	"for(i,1,3,j=i)",
	"",

	"j",
	"3",
};

void
test_for(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
