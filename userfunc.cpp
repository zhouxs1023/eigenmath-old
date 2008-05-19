// Codes for handling user-defined functions

#include "stdafx.h"
#include "defs.h"

/* For f(x)=x^2 we have p1 pointing to the following data structure.

      _______     _______                 _______ 
p1-->|CONS   |-->|CONS   |-------------->|CONS   |
     |_______|   |_______|               |_______|
         |           |                       |
      ___V___     ___V___     _______     ___V___     _______     _______
     |SETQ   |   |CONS   |-->|CONS   |   |CONS   |-->|CONS   |-->|CONS   |
     |_______|   |_______|   |_______|   |_______|   |_______|   |_______|
                     |           |           |           |           |
                  ___V___     ___V___     ___V___     ___V___     ___V___
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

	prep_args();
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
		prep_args();
		set_binding_and_arglist(NAME, BODY, ARGS);
	}

	push(symbol(NIL));	// return value
}

// Change formal args to GETARG functions

void
prep_args(void)
{
	int n = 0;
	p2 = ARGS;
	push(BODY);
	while (iscons(p2)) {
		push(car(p2));
		push(symbol(GETARG));
		push_integer(n++);
		list(2);
		subst();
		p2 = cdr(p2);
	}
	BODY = pop();
}

/* For example, this is what p1 points to when the user function wants the 1st
   argument...

                 _______         _______         _______
        p1 ---->|CONS   |------>|CONS   |------>|NIL    |
                |_______|       |_______|       |_______|
                    |               |
                    |               |
                 ___V___         ___V___
                |GETARG |       |NUM 0  |
                |_______|       |_______|
*/

void
eval_getarg(void)
{
	int i, n;
	push(cadr(p1));
	n = pop_integer();
	p1 = args;
	for (i = 0; i < n; i++)
		p1 = cdr(p1); // ok for all n, cdr(nil) = nil, car(nil) = nil
	push(car(p1));
}

/*	Example: f(x,y)

	p1 -> (f x y)

	car(p1) -> f
*/

void
eval_user_function(void)
{
	int h;

	// Use "derivative" instead of "d" if no user function "d"

	if (car(p1) == symbol(SYMBOL_D) && get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	// p2 is the body of the user function

	p2 = get_binding(car(p1));

	// make p3 the argument list

	h = tos;
	p3 = cdr(p1);
	while (iscons(p3)) {
		push(car(p3));
		eval();
		p3 = cdr(p3);
	}
	list(tos - h);
	p3 = pop();

	// undefined function?

	if (p2 == car(p1)) {
		push(p2);
		push(p3);
		cons();
		return;
	}

	// eval function body in arg context

	push(args);
	args = p3;
	push(p2);
	eval();
	swap();
	args = pop();
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
