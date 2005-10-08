/* Returns the real part of an expression, assumes symbols are real

Special cases

*/

#include "stdafx.h"
#include "defs.h"

void
eval_real(void)
{
	push(cadr(p1));
	eval();
	real();
}

/* Return the real part

If the arg is a sum then all terms are converted to rectangular coordinates
and the result is (z + conj(z)) / 2.

Otherwise the result is mag(z) * cosine(arg(z)).

*/

void
real(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(ADD)) {
		push(p1);
		rect();
		p1 = pop();
		push(p1);
		push(p1);
		conjugate();
		add();
		push_integer(2);
		divide();
	} else {
		push(p1);
		mag();
		push(p1);
		arg();
		cosine();
		multiply();
	}
	restore();
}

/* Return the imaginary part

If the arg is a sum then all terms are converted to rectangular coordinates
and the result is (z - conj(z)) / (2i).

Otherwise the result is mag(z) * sin(arg(z)).

*/

void
eval_imag(void)
{
	push(cadr(p1));
	eval();
	imag();
}

void
imag(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(ADD)) {
		push(p1);
		rect();
		p1 = pop();
		push(p1);
		push(p1);
		conjugate();
		subtract();
		push_integer(2);
		divide();
		push(imaginaryunit);
		divide();
	} else {
		push(p1);
		mag();
		push(p1);
		arg();
		sine();
		multiply();
	}
	restore();
}

// converts complex polar to rectangular coordinates

void
eval_rect(void)
{
	push(cadr(p1));
	eval();
	rect();
}

void
rect(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(ADD)) {
		push_integer(0);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			rect();
			add();
			p1 = cdr(p1);
		}
	} else {
		// rect(z) = mag(z) * (cos(arg(z)) + i sin(arg(z)))
		push(p1);
		mag();
		push(p1);
		arg();
		p1 = pop();
		push(p1);
		cosine();
		push(imaginaryunit);
		push(p1);
		sine();
		multiply();
		add();
		multiply();
	}
	restore();
}

#if 0

// returns the angle in complex space

void
eval_arg(void)
{
	push(cadr(p1));
	eval();
	arg();
}

void
arg(void)
{
	save();

	p1 = pop();

	// -1 to a power

	if (car(p1) == symbol(POWER) && equaln(cadr(p1), -1)) {
		push(symbol(PI));
		push(caddr(p1));
		multiply();
		restore();
		return;
	}

	// exponential

	if (car(p1) == symbol(POWER) && cadr(p1) == symbol(E)) {
		push(caddr(p1));
		push(caddr(p1));
		conjugate();
		subtract();
		push_integer(2);
		divide();
		push(imaginaryunit);
		divide();
		restore();
		return;
	}

	// product (sum over angles)

	if (car(p1) == symbol(MULTIPLY)) {
		push_integer(0);
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			argument();
			add();
			p1 = cdr(p1);
		}
		restore();
		return;
	}

	if (car(p1) == symbol(ADD)) {
		// sum of terms
		push(p1);
		rect();
		p1 = pop();
		push(p1);
		real();
		p2 = pop();
		push(p1);
		imag();
		p3 = pop();
		if (iszero(p2)) {
			push(symbol(PI));
			if (isnegative(p3))
				negate();
		} else {
			push(p3);
			push(p2);
			divide();
			arctan();
		}
		restore();
		return;
	}

	push_integer(0);
	restore();
}

#endif