// If odd number of args then last arg is a default case.

#include "stdafx.h"
#include "defs.h"

void
eval_test(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		if (cdr(p1) == nil) {
			push(car(p1)); // default case
			eval();
			return;
		}
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (!iszero(p2)) {
			push(cadr(p1));
			eval();
			return;
		}
		p1 = cddr(p1);
	}
	push_integer(0);
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
	if (iszero(p1))
		p1 = zero; // in case p1 is a zero tensor
	if (iszero(p2))
		p2 = zero; // in case p2 is a zero tensor
	if (cmp_expr(p1, p2) == 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testge(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p2 = pop();
	p1 = pop();
	if (cmp_expr(p1, p2) >= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testgt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p2 = pop();
	p1 = pop();
	if (cmp_expr(p1, p2) > 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testle(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p2 = pop();
	p1 = pop();
	if (cmp_expr(p1, p2) <= 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_testlt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	p2 = pop();
	p1 = pop();
	if (cmp_expr(p1, p2) < 0)
		push_integer(1);
	else
		push_integer(0);
}

void
eval_not(void)
{
	push(cadr(p1));
	eval_predicate();
	p1 = pop();
	if (iszero(p1))
		push_integer(1);
	else
		push_integer(0);
}

void
eval_and(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (iszero(p2)) {
			push_integer(0);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(1);
}

void
eval_or(void)
{
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p2 = pop();
		if (!iszero(p2)) {
			push_integer(1);
			return;
		}
		p1 = cdr(p1);
	}
	push_integer(0);
}

static char *s[] = {

	"1==1",
	"1",

	"1==2",
	"0",

	"a==b",
	"0",

	"1>=1",
	"1",

	"1>=2",
	"0",

	"2>=1",
	"1",

	"a>=b",
	"0",

	"1>1",
	"0",

	"1>2",
	"0",

	"2>1",
	"1",

	"a>b",
	"0",

	"1<=1",
	"1",

	"1<=2",
	"1",

	"2<=1",
	"0",

	"a<=b",
	"1",

	"1<1",
	"0",

	"1<2",
	"1",

	"2<1",
	"0",

	"a<b",
	"1",

	"test(0,A,B)",
	"B",

	"test(1,A,B)",
	"A",

	"test(0,A,0,B)",
	"0",

	"test(0,A,0,B,C)",
	"C",

	"not(1)",
	"0",

	"not(0)",
	"1",

	"not(a=a)",
	"0",

	"and(1,1)",
	"1",

	"and(1,0)",
	"0",

	"or(1,0)",
	"1",

	"or(0,0)",
	"0",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
