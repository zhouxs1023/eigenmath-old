/* arg(z) Argument (angle) of complex z

	z		arg(z)
	-		------

	(-1)^a		a pi

	exp(a + i b)	b

	a b		arg(a) + arg(b)

	a + i b		arctan(b/a)

Result by quadrant

	z		arg(z)
	-		------

	1 + i		1/4 pi

	1 - i		-1/4 pi

	-1 + i		3/4 pi

	-1 - i		-3/4 pi
*/

#include "stdafx.h"
#include "defs.h"

void
eval_arg(void)
{
	push(cadr(p1));
	eval();
	arg();
}

#define RE p2
#define IM p3

void
arg(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(POWER) && equaln(cadr(p1), -1)) {
		// -1 to a power
		push(symbol(PI));
		push(caddr(p1));
		multiply();
	} else if (car(p1) == symbol(POWER) && cadr(p1) == symbol(E)) {
		// exponential
		push(caddr(p1));
		imag();
	} else if (car(p1) == symbol(MULTIPLY)) {
		// product of factors
		push_integer(0);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			arg();
			add();
			p1 = cdr(p1);
		}
	} else if (car(p1) == symbol(ADD)) {
		// sum of terms
		push(p1);
		rect();
		p1 = pop();
		push(p1);
		real();
		RE = pop();
		push(p1);
		imag();
		IM = pop();
		if (iszero(RE)) {
			push(symbol(PI));
			if (isnegative(IM))
				negate();
		} else {
			push(IM);
			push(RE);
			divide();
			arctan();
			if (isnegative(RE)) {
				push_symbol(PI);
				if (isnegative(IM))
					subtract();	// quadrant 1 -> 3
				else
					add();		// quadrant 4 -> 2
			}
		}
	} else
		// pure real
		push_integer(0);
	restore();
}

static char *s[] = {

	"arg(1+i)",
	"1/4*pi",

	"arg(1-i)",
	"-1/4*pi",

	"arg(-1+i)",
	"3/4*pi",

	"arg(-1-i)",
	"-3/4*pi",

	"arg((-1)^(1/3))",
	"1/3*pi",

	"arg(1+exp(i*pi/3))",
	"1/6*pi",

	"arg((-1)^(1/6)*exp(i*pi/6))",
	"1/3*pi",

	"arg(a)",
	"0",

	"arg(a*exp(b+i*pi/5))",
	"1/5*pi",
};

void
test_arg(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

