// If odd number of args then last arg is a default case.

#include "stdafx.h"
#include "defs.h"

void
eval_test(void)
{
	p1 = cdr(p1);
	while (p1 != nil) {
		if (cdr(p1) == nil) {
			// default case
			push(car(p1));
			eval();
			return;
		}
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (isnum(p2) && !iszero(p2)) {
			push(cadr(p1));
			eval();
			return;
		}
		p1 = cddr(p1);
	}
	push(symbol(YVOID));
}

void
eval_testeq(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p2 = pop();
	p1 = pop();
	if (equal(p1, p2))
		push(one);
	else
		push(zero);
}

void
eval_testge(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(zero);
		else
			push(one);
	else
		push(symbol(YVOID));
}

void
eval_testgt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	swap();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(one);
		else
			push(zero);
	else
		push(symbol(YVOID));
}

void
eval_testle(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	swap();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(zero);
		else
			push(one);
	else
		push(symbol(YVOID));
}

void
eval_testlt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(one);
		else
			push(zero);
	else
		push(symbol(YVOID));
}

static char *s[] = {

	"1==1",
	"1",

	"1==2",
	"0",

	"a==b",
	"void",

	"1>=1",
	"1",

	"1>=2",
	"0",

	"2>=1",
	"1",

	"a>=b",
	"void",

	"1>1",
	"0",

	"1>2",
	"0",

	"2>1",
	"1",

	"a>b",
	"void",

	"1<=1",
	"1",

	"1<=2",
	"1",

	"2<=1",
	"0",

	"a<=b",
	"void",

	"1<1",
	"0",

	"1<2",
	"1",

	"2<1",
	"0",

	"a<b",
	"void",

	"test(0,A,B)",
	"B",

	"test(1,A,B)",
	"A",

	"test(0,A,0,B)",
	"void",

	"test(0,A,0,B,C)",
	"C",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
