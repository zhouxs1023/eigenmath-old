#include "stdafx.h"

#include "defs.h"

void mod(void);

void
eval_mod(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	mod();
}

void
mod(void)
{
	int n;

	save();

	p2 = pop();
	p1 = pop();

	if (iszero(p2))
		stop("mod: divide by zero");

	if (!isnum(p1) || !isnum(p2)) {
		push_symbol(MOD);
		push(p1);
		push(p2);
		list(3);
		restore();
		return;
	}

	if (p1->k == DOUBLE) {
		push(p1);
		n = pop_integer();
		if (n == (int) 0x80000000)
			stop("mod: cannot convert float value to integer");
		push_integer(n);
		p1 = pop();
	}

	if (p2->k == DOUBLE) {
		push(p2);
		n = pop_integer();
		if (n == (int) 0x80000000)
			stop("mod: cannot convert float value to integer");
		push_integer(n);
		p2 = pop();
	}

	if (!isinteger(p1) || !isinteger(p2))
		stop("mod: integer arguments expected");

	p3 = alloc();
	p3->k = NUM;
	p3->u.q.a = mmod(p1->u.q.a, p2->u.q.a);
	p3->u.q.b = mint(1);
	push(p3);

	restore();
}

static char *s[] = {

	"mod(2.0,3.0)",
	"2",

	"mod(-2.0,3.0)",
	"-2",

	"mod(2.0,-3.0)",
	"2",

	"mod(-2.0,-3.0)",
	"-2",

	"mod(2,3)",
	"2",

	"mod(-2,3)",
	"-2",

	"mod(2,-3)",
	"2",

	"mod(-2,-3)",
	"-2",

	"mod(a,b)",
	"mod(a,b)",

	"mod(2.0,0.0)",
	"mod: divide by zero",

	"mod(2,0)",
	"mod: divide by zero",

	"mod(1.2,2)",
	"mod: cannot convert float value to integer",

	"mod(1/2,3)",
	"mod: integer arguments expected",

	"mod(15,8.0)",
	"7",
};

void
test_mod(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
