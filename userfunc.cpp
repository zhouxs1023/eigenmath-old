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
#define TMP p5

void
define_user_function(void)
{
	int i, n;

	NAME = caadr(p1);
	ARGS = cdadr(p1);
	BODY = caddr(p1);

	if (!issymbol(NAME))
		stop("in function definition, user symbol expected for function name");

	set_binding_and_arglist(NAME, BODY, ARGS);

	// do eval, maybe

	if (car(BODY) == symbol(EVAL)) {

		// remove eval

		set_binding_and_arglist(NAME, cadr(BODY), ARGS);

		// evaluate the function definition using quoted symbols

		push(NAME);
		TMP = ARGS;
		n = length(TMP);
		for (i = 0; i < n; i++) {
			push_symbol(QUOTE);
			push(car(TMP));
			list(2);
			TMP = cdr(TMP);
		}
		list(n + 1);
		eval();

		// new binding

		set_binding_and_arglist(NAME, pop(), ARGS);
	}

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

	// special case for "d"

	if (FNAME == symbol(SYMBOL_D)
	&& get_arglist(symbol(SYMBOL_D)) == symbol(NIL)) {
		eval_derivative();
		return;
	}

	// undefined function?

	if (get_binding(FNAME) == FNAME) {
		push(FNAME);
		while (iscons(ACTUAL_ARGS)) {
			push(car(ACTUAL_ARGS));
			eval();
			ACTUAL_ARGS = cdr(ACTUAL_ARGS);
		}
		list(tos - h);
		return;
	}

	// argument substitution

	push(get_binding(FNAME));

	// replace formal args with placeholders to avoid glare
	// f.e. formal args are A,B and actual args are B,A
	// A gets replaced with B, then all B are replaced with A

	FORMAL_ARGS = get_arglist(FNAME);
	ACTUAL_ARGS = cdr(p1);
	while (iscons(FORMAL_ARGS) && iscons(ACTUAL_ARGS)) {
		push(car(FORMAL_ARGS));
		push(symbol(SECRETX));
		push(car(FORMAL_ARGS));
		list(2);
		subst();
		FORMAL_ARGS = cdr(FORMAL_ARGS);
		ACTUAL_ARGS = cdr(ACTUAL_ARGS);
	}

	// replace placeholders with actual args

	FORMAL_ARGS = get_arglist(FNAME);
	ACTUAL_ARGS = cdr(p1);
	while (iscons(FORMAL_ARGS) && iscons(ACTUAL_ARGS)) {
		push(symbol(SECRETX));
		push(car(FORMAL_ARGS));
		list(2);
		push(car(ACTUAL_ARGS));
		subst();
		FORMAL_ARGS = cdr(FORMAL_ARGS);
		ACTUAL_ARGS = cdr(ACTUAL_ARGS);
	}

	eval();
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

	"x=7",
	"",

	"p(x,y)=eval(x+y)",
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
