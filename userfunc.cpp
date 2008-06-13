// Codes for handling user-defined functions

#include "stdafx.h"
#include "defs.h"

// F is the function body
// A is the formal argument list
// B is the actual argument list

#define F p3
#define A p4
#define B p5
#define C p6

void
eval_user_function(void)
{
	int h = tos;

	// Use "derivative" instead of "d" if no user function "d"

	if (car(p1) == symbol(SYMBOL_D) && get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	F = get_binding(car(p1));
	A = get_arglist(car(p1));
	B = cdr(p1);

	// undefined function?

	if (F == car(p1)) {
		push(F);
		p1 = B;
		while (iscons(p1)) {
			push(car(p1));
			eval();
			p1 = cdr(p1);
		}
		list(tos - h);
		return;
	}

	// evaluate actual argument list

	p1 = A;
	p2 = B;
	while (iscons(p1) && iscons(p2)) {
		push(car(p1));
		push(car(p2));
		eval();
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	// merge current arg list

	p1 = args;
	while (iscons(p1)) {
		if (!find(A, car(p1))) {
			push(car(p1));
			push(cadr(p1));
		}
		p1 = cddr(p1);
	}

	// new list

	C = args;
	list(tos - h);
	args = pop();
	
	// eval function body

	push(F);
	resolve();
	eval();

	args = C;
}

// Resolve function arguments

void
resolve(void)
{
	int h, i;
	save();
	p1 = pop();

	p2 = args;
	while (iscons(p2)) {
		if (equal(p1, car(p2))) {
			push(cadr(p2));
			restore();
			return;
		}
		p2 = cddr(p2);
	}

	if (istensor(p1)) {
		push(p1);
		copy_tensor();
		p1 = pop();
		for (i = 0; i < p1->u.tensor->nelem; i++) {
			push(p1->u.tensor->elem[i]);
			resolve();
			p1->u.tensor->elem[i] = pop();
		}
		push(p1);
		restore();
		return;
	}

	if (iscons(p1)) {
		h = tos;
		push(car(p1)); // Note 1
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			resolve();
			p1 = cdr(p1);
		}
		list(tos - h);
		restore();
		return;
	}

	if (issymbol(p1)) {
		p2 = get_binding(p1);
		push(p2);
		if (p1 != p2)
			resolve();
		restore();
		return;
	}

	push(p1);
	restore();
}

#if SELFTEST

static char *s[] = {

// args of generic functions should be evaluated

	"f(1+2,3*4)",
	"f(3,12)",

// simple func def

	"f(x)=x^2",
	"",

	"f",
	"x^2",

// bindings should be restored

	"x=123",
	"",

	"y=345",
	"",

	"f(x,y)=x^2+y^3",
	"",

	"f(2,3)",
	"31",

	"x",
	"123",

	"y",
	"345",

// as above but this time with function bindings

	"x(a)=sin(a)",
	"",

	"y(b)=cos(b)",
	"",

	"f(x,y)=x^2+y^3",
	"",

	"f(2,3)",
	"31",

	"x",
	"sin(a)",

	"y",
	"cos(b)",

// eval func body

	"x=quote(x)",
	"",

	"p(x)=7+4x",
	"",

	"w(x)=eval(integral(p(x)))",
	"",

	"w-2*x^2-7*x",
	"0",

	"w(5)-w(2)",
	"63",

// are symbols evaluated in argument context?

	"A=a^2",
	"",

	"f(a)=A",
	"",

	"f(x+1)",
	"x^2+2*x+1",
};

void
test_user_func(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
