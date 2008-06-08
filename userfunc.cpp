// Codes for handling user-defined functions

#include "stdafx.h"
#include "defs.h"

/* For f(x)=x^2 we have p1 pointing to the following data structure.

         _______     _______                 _______ 
p1----->|CONS   |-->|CONS   |-------------->|CONS   |
        |_______|   |_______|               |_______|
            |           |                       |
         ___v___     ___v___     _______     ___v___     _______     _______
        |SETQ   |   |CONS   |-->|CONS   |   |CONS   |-->|CONS   |-->|CONS   |
        |_______|   |_______|   |_______|   |_______|   |_______|   |_______|
                        |           |           |           |           |
                     ___v___     ___v___     ___v___     ___v___     ___v___
                    |SYM f  |   |SYM x  |   |POWER  |   |SYM x  |   |NUM 2  |
                    |_______|   |_______|   |_______|   |_______|   |_______|

(For brevity, cdrs pointing to nil are not shown.)

Hence

	caadr(p1) == f
	cdadr(p1) == (x)
	caddr(p1) == (power x 2)
*/

#define NAME p3
#define ARGS p4
#define BODY p5

void
define_user_function(void)
{
	int h;

	NAME = caadr(p1);
	ARGS = cdadr(p1);
	BODY = caddr(p1);

	if (!issymbol(NAME))
		stop("function name?");

	set_binding_and_arglist(NAME, BODY, ARGS);

	// do eval, maybe

	if (car(BODY) == symbol(EVAL)) {

		// remove eval

		set_binding_and_arglist(NAME, cadr(BODY), ARGS);

		// evaluate the function definition using quoted symbols

		h = tos;
		push(NAME);
		p2 = ARGS;
		while (iscons(p2)) {
			push_symbol(QUOTE);
			push(car(p2));
			list(2);
			p2 = cdr(p2);
		}
		list(tos - h);
		eval();

		// new binding

		BODY = pop();
		set_binding_and_arglist(NAME, BODY, ARGS);
	}

	push(symbol(NIL));	// return value
}

// F is the function body
// A is the formal argument list
// B is the actual argument list

#define F p3
#define A p4
#define B p5

void
eval_user_function(void)
{
	int h;

	// Use "derivative" instead of "d" if no user function "d"

	if (car(p1) == symbol(SYMBOL_D) && get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	F = get_binding(car(p1));
	A = get_arglist(car(p1));
	B = cdr(p1);

	// evaluate actual argument list

	h = tos;
	while (iscons(B)) {
		push(car(B));
		eval();
		B = cdr(B);
	}
	list(tos - h);
	B = pop();

	// undefined function?

	if (F == car(p1)) {
		push(F);
		push(B);
		cons();
		return;
	}

	// save original bindings

	p1 = A;
	while (iscons(p1)) {
		push(get_binding(car(p1)));
		push(get_arglist(car(p1)));
		p1 = cdr(p1);
	}

	// New bindings use quote because
	//
	// 1. The argument has already been evaluated.
	//
	// 2. We want to use the evaluation that was obtained in the calling
	//    context.
	//
	// 3. Prevent circular references. For example, given f(x) = x^2, we
	//    would prefer that f(x + 1) yield x^2 + 2x + 1 instead of halting
	//    due to x = x + 1.

	p1 = A;
	p2 = B;
	while (iscons(p1)) {
		push_symbol(QUOTE);
		push(car(p2));
		list(2);
		set_binding(car(p1), pop());
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	// eval function body

	push(F);
	eval();
	p0 = pop();

	restore_bindings(A);

	push(p0);
}

void
restore_bindings(U *p)
{
	if (iscons(p)) {
		restore_bindings(cdr(p));
		p2 = pop();
		p1 = pop();
		set_binding_and_arglist(car(p), p1, p2);
	}
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

// Check that args are quoted in func defn

	"x=7",
	"",

	"y=8",
	"",

	"p(x,y)=eval(x+y)",
	"",

	"x=quote(x)",
	"",

	"y=quote(y)",
	"",

	"p",
	"x+y",
};

void
test_user_func(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
