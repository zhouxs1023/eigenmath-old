#include "stdafx.h"
#include "defs.h"

/*	Example: f(x) = x^2

	p1 -----*-------*---------------*
		|	|		|
		setq	*-------*	*-------*-------*
			|	|	|	|	|
			f	x	power	x	2
*/

#define NAME p2
#define ARGS p3
#define BODY p4

void
define_user_function(void)
{
	NAME = caadr(p1);
	ARGS = cdadr(p1);
	BODY = caddr(p1);

	if (!issymbol(NAME))
		stop("in function definition, user symbol expected for function name");

	NAME->u.sym.binding = BODY;
	NAME->u.sym.arglist = ARGS;

	push(symbol(NIL));	// return value
}

/*	Example: f(x,y)

	p1 -> (f x y)

	car(p1) -> f
*/

#define FNAME p2
#define ACTUAL_ARGS p3
#define FORMAL_ARGS p4

void
eval_user_function(void)
{
	int h = tos;

	FNAME = car(p1);
	ACTUAL_ARGS = cdr(p1);

	// undefined function?

	if (FNAME->u.sym.binding == FNAME) {
		push(FNAME);
		while (iscons(ACTUAL_ARGS)) {
			push(car(ACTUAL_ARGS));
			eval();
			ACTUAL_ARGS = cdr(ACTUAL_ARGS);
		}
		list(tos - h);
		return;
	}

	FORMAL_ARGS = FNAME->u.sym.arglist;

	// argument substitution

	push(FNAME->u.sym.binding);
	while (iscons(FORMAL_ARGS) && iscons(ACTUAL_ARGS)) {
		push(car(FORMAL_ARGS));
		push(car(ACTUAL_ARGS));
		eval();
		subst();
		FORMAL_ARGS = cdr(FORMAL_ARGS);
		ACTUAL_ARGS = cdr(ACTUAL_ARGS);
	}

	eval();
}

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
};

void
test_user_func(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
