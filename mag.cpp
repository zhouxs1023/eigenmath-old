/* mag(z) Magnitude of complex z

	Input		Output

	(-1) ^ A	1

	exp(A + B i)	exp(A)

	A B		mag(A) * mag(B)

	A + B i		sqrt(A^2 + B^2)

Handles mixed polar and rectangular forms, e.g. 1 + exp(i pi/3)

*/

#include "stdafx.h"
#include "defs.h"

void
eval_mag(void)
{
	push(cadr(p1));
	eval();
	mag();
}

void
mag(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(POWER) && equaln(cadr(p1), -1))
		// -1 to a power
		push_integer(1);
	else if (car(p1) == symbol(POWER) && cadr(p1) == symbol(E)) {
		// exponential
		push(caddr(p1));
		real();
		exponential();
	} else if (car(p1) == symbol(MULTIPLY)) {
		// product
		push_integer(1);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			mag();
			multiply();
			p1 = cdr(p1);
		}
	} else if (car(p1) == symbol(ADD)) {
		// sum
		push(p1);
		rect(); // convert polar terms, if any
		p1 = pop();
		push(p1);
		real();
		push_integer(2);
		power();
		push(p1);
		imag();
		push_integer(2);
		power();
		add();
		push_rational(1, 2);
		power();
		simplify_trig();
	} else
		// default (all real)
		push(p1);
	restore();
}
