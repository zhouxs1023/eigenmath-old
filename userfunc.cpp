// Evaluate a user defined function

#include "stdafx.h"
#include "defs.h"

#define F p3 // F is the function body
#define A p4 // A is the formal argument list
#define B p5 // B is the calling argument list
#define S p6 // S is the argument substitution list

void
eval_user_function(void)
{
	int h;

	// Use "derivative" instead of "d" if there is no user function "d"

	if (car(p1) == symbol(SYMBOL_D) && get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	F = get_binding(car(p1));
	A = get_arglist(car(p1));
	B = cdr(p1);

	// Undefined function?

	if (F == car(p1)) {
		h = tos;
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

	// Create the argument substitution list S

	p1 = A;
	p2 = B;
	h = tos;
	while (iscons(p1) && iscons(p2)) {
		push(car(p1));
		push(car(p2));
		eval();
		p1 = cdr(p1);
		p2 = cdr(p2);
	}
	list(tos - h);
	S = pop();

	// Evaluate the function body

	push(F);
	push(S);
	rewrite(0);
	eval();
}

// Example:
//
//	push(F)
//	push(S)
//	rewrite()
//	p1 = pop()
//
// where F is an expression and S is a substitution list.

int
rewrite(int flag)
{
	int h, n;
	save();

	p2 = pop();
	p1 = pop();

	if (istensor(p1)) {
		n = rewrite_tensor(flag);
		restore();
		return n;
	}

	if (iscons(p1)) {
		n = 0;
		h = tos;
		push(car(p1)); // Do not rewrite function name
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			n += rewrite(flag);
			p1 = cdr(p1);
		}
		list(tos - h);
		restore();
		return n;
	}

	// If not a symbol then done

	if (!issymbol(p1)) {
		push(p1);
		restore();
		return 0;
	}

	// Try for an argument substitution first

	p3 = p2;
	while (iscons(p3)) {
		if (p1 == car(p3)) {
			push(cadr(p3));
			restore();
			return 1;
		}
		p3 = cddr(p3);
	}

	// Get the symbol's binding, try again

	p3 = get_binding(p1);
	push(p3);
	if (p1 != p3) {
		push(p2); // subst. list
		n = rewrite(flag);
		if (n == 0 && flag == 0) {
			pop();
			push(p1); // restore if not rewritten with arg
		}
	}

	restore();
	return n;
}

int
rewrite_tensor(int flag)
{
	int i, n = 0;
	push(p1);
	copy_tensor();
	p1 = pop();
	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(p1->u.tensor->elem[i]);
		push(p2);
		n += rewrite(flag);
		p1->u.tensor->elem[i] = pop();
	}
	push(p1);
	return n;
}

#if SELFTEST

static char *s[] = {

// args of template functions should be evaluated

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

// Are symbols evaluated in argument context?

	"A=a^2",
	"",

	"f(a)=A",
	"",

	"f(x+1)",
	"x^2+2*x+1",

// Check the scope of variables

	"f(x)=g(x)",
	"",

	"g(a)=x^a",
	"",

	"f(2)",
	"x^2",
};

void
test_user_func(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
