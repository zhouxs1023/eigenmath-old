#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Put polynomial coefficients on the stack
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Returns number of coefficients on stack
//
//			tos-n		Coefficient of x^0
//
//			tos-1		Coefficient of x^(n-1)
//
//-----------------------------------------------------------------------------

#include "defs.h"

void
coeff_cooked(void)
{
	int h, i, n;
	save();
	p2 = pop();
	p1 = pop();
	h = tos;
	if (p2 == Nil) {
		push(p1);
		variables();
		p2 = pop();
		if (p2->u.tensor->nelem == 0) {
			p2 = alloc_tensor(1);
			p2->u.tensor->ndim = 1;
			p2->u.tensor->dim[0] = 1;
			p2->u.tensor->elem[0] = p1;
			push(p2);
			restore();
			return;
		}
		push(p1);
		push(p2->u.tensor->elem[0]);
	} else {
		push(p1);
		push(p2);
	}
	coeff();
	n = tos - h;
	p1 = alloc_tensor(n);
	p1->u.tensor->ndim = 1;
	p1->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[i] = stack[i];
	tos = h;
	push(p1);
	restore();
}

int
coeff(void)
{
	int h, n;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	for (;;) {

		push(p1);	// FIXME faster to use evalat
		push(p2);
		push(zero);
		subst();
		eval();

		p3 = pop();
		push(p3);

		push(p1);
		push(p3);
		subtract();

		p1 = pop();

		if (equal(p1, zero)) {
			n = tos - h;
			restore();
			return n;
		}

		push(p1);
		push(p2);
		divide();
		p1 = pop();
	}
}

static char *s[] = {

	"coeff(3*x^2+2*x+1)",
	"(1,2,3)",

	"coeff((x+1)*(y+1))",
	"(1+y,1+y)",

	"coeff((x+1)*(y+1),y)",
	"(1+x,1+x)",
};

void
test_coeff(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
