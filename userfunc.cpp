#include "stdafx.h"
#include "defs.h"

extern U *formal_arg[6];

//-----------------------------------------------------------------------------
//
//	Example: f(x) = x^2
//
//	p1	*-------*---------------*
//		|	|		|
//		setq	*-------*	*-------*-------*
//			|	|	|	|	|
//			f	x	power	x	2
//
//-----------------------------------------------------------------------------

#define NAME p2
#define ARGS p3
#define BODY p4
#define BODY2 p5
#define TMP p6

void
define_user_function(void)
{
	int i, n;

	NAME = caadr(p1);
	ARGS = cdadr(p1);
	BODY = caddr(p1);

	if (NAME->k != SYM)
		stop("In function definition, user symbol expected for function name.");

	n = length(ARGS);

	if (n > 6)
		stop("More than 6 formal args in function definition.");

	// subst args in body

	push(BODY);
	TMP = ARGS;
	for (i = 0; i < n; i++) {
		if (car(TMP)->k != SYM)
			stop("In function definition, formal arg is not a user symbol.");
		push(car(TMP));
		push(formal_arg[i]);
		subst();
		TMP = cdr(TMP);
	}
	BODY2 = pop();

	// binding

	NAME->u.sym.binding = BODY;
	NAME->u.sym.binding2 = BODY2;

	// do eval, maybe

	if (car(BODY) == symbol(EVAL)) {

		// remove eval

		NAME->u.sym.binding = cadr(BODY);
		NAME->u.sym.binding2 = cadr(BODY2);

		// build function call with quoted symbols

		push(NAME);
		TMP = ARGS;
		for (i = 0; i < n; i++) {
			push_symbol(QUOTE);
			push(car(TMP));
			list(2);
			TMP = cdr(TMP);
		}
		list(n + 1);

		// eval

		eval();
		BODY = pop();

		// subst args in body

		push(BODY);
		TMP = ARGS;
		for (i = 0; i < n; i++) {
			push(car(TMP));
			push(formal_arg[i]);
			subst();
			TMP = cdr(TMP);
		}
		BODY2 = pop();

		// update bindings

		NAME->u.sym.binding = BODY;
		NAME->u.sym.binding2 = BODY2;
	}

	push(nil);	// return value
}

// Note: Tried doing func eval above using formal_arg and then back subst
// symbols from ARGLIST.
// The problem with this method is that the resulting BODY might be
// denormalized.
// In other words, the terms and factors are not sorted properly.
// This causes problems since add() and multiply() use a merge sort that
// requires sorted (normalized) terms and factors.
// The code above ensures a normalized BODY.

//-----------------------------------------------------------------------------
//
//	Example: f(x,y)
//
//	p1 -> (f x y)
//
//	car(p1) -> f
//
//-----------------------------------------------------------------------------

#define FUNC_NAME p2
#define ACTUAL_ARGLIST p3
#define TMP p6

void
eval_user_function(void)
{
	int h, i;

	h = tos;

	FUNC_NAME = car(p1);
	ACTUAL_ARGLIST = cdr(p1);

	// undefined function?

	if (FUNC_NAME->u.sym.binding == FUNC_NAME) {
		push(FUNC_NAME);
		while (iscons(ACTUAL_ARGLIST)) {
			push(car(ACTUAL_ARGLIST));
			eval();
			ACTUAL_ARGLIST = cdr(ACTUAL_ARGLIST);
		}
		list(tos - h);
		return;
	}

	// is it a function?

	if (FUNC_NAME->u.sym.binding2 == nil)
		stop("Attempt to call non-function");

	// eval actual args in current formal arg context, don't modify formal args yet

	for (i = 0; i < 6; i++) {
		push(car(ACTUAL_ARGLIST));
		eval();
		ACTUAL_ARGLIST = cdr(ACTUAL_ARGLIST);
		push(nil); // make room for saving binding2
	}

	// ok, now it's safe to modify formal args

	for (i = 0; i < 6; i++) {
		TMP = formal_arg[i]->u.sym.binding;
		formal_arg[i]->u.sym.binding = stack[h + 2 * i];
		stack[h + 2 * i] = TMP;
		TMP = formal_arg[i]->u.sym.binding2;
		formal_arg[i]->u.sym.binding2 = stack[h + 2 * i + 1];
		stack[h + 2 * i + 1] = TMP;
	}

	// evaluate user-defined function

	push(FUNC_NAME->u.sym.binding2);
	eval();

	// restore args

	TMP = pop();
	for (i = 0; i < 6; i++) {
		formal_arg[5 - i]->u.sym.binding2 = pop();
		formal_arg[5 - i]->u.sym.binding = pop();
	}
	push(TMP);
}

void
eval_binding2(void)
{
	p1 = cadr(p1);
	if (p1->k != SYM)
		stop("Symbol expected in binding2.");
	push(p1->u.sym.binding2);
}

//-----------------------------------------------------------------------------
//
//	Evaluate an expression with a symbol set to a value.
//
//	Input:		tos-3	expr
//
//			tos-2	symbol
//
//			tos-1	value
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#define F p1
#define X p2
#define VAL p3

void
evalat(void)
{
	int mark;

	save();

	VAL = pop();
	X = pop();

	if (X->k != SYM) {
		push(X);
		push(VAL);
		subst();
		eval();
		restore();
		return;
	}

	push(X);
	mark = save_symbols(1);

	X->u.sym.binding = VAL;
	X->u.sym.binding2 = nil;

	eval();

	restore_symbols(mark);

	restore();
}

static char *s[] = {

	"f=quote(f)",
	"",

	"x=quote(x)",
	"",

	"y=quote(y)",
	"",

	// args of generic functions should be evaluated

	"f(1+2,3*4)",
	"f(3,12)",

	// simple func def

	"f(x)=x^2",
	"",

	"f",
	"x^2",

	"binding2(f)",
	"@1^2",

	// symbols should be quoted in eval

	"x=123",
	"",

	"f(x)=eval(x^3)",
	"",

	"f",
	"x^3",

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

	"binding2(x)",
	"sin(@1)",

	"y",
	"cos(b)",

	"binding2(y)",
	"cos(@1)",

	// clean up

	"f=quote(f)",
	"",

	"x=quote(x)",
	"",

	"y=quote(y)",
	"",
};

void
test_user_func(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
