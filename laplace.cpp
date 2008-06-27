// Laplace transform

#include "stdafx.h"
#include "defs.h"

void
eval_laplace(void)
{
	push(cadr(p1));
	eval();
	push(symbol(SYMBOL_T));
	laplace();
}

#define F p3
#define T p4
#define A p5

void
laplace(void)
{
	int h;
	save();

	T = pop();
	F = pop();

	// L[f + g] = L[f] + L[g]

	if (car(F) == symbol(ADD)) {
		p1 = cdr(F);
		h = tos;
		while (iscons(p1)) {
			push(car(p1));
			push(T);
			laplace();
			p1 = cdr(p1);
		}
		add_all(tos - h);
		restore();
		return;
	}

	// L[Af] = A L[f]

	if (car(F) == symbol(MULTIPLY)) {
		push(F);
		push(T);
		partition();
		F = pop();
		A = pop();
	} else {
		push_integer(1);
		A = pop();
	}

	push_symbol(LAPLACE);
	push(F);
	push(T);
	list(3);

	push(A);
	multiply();

	restore();
}

#if SELFTEST

static char *s[] = {

};

void
test_laplace(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
