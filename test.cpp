#include "stdafx.h"
#include "defs.h"

void
eval_test(void)
{
	p2 = cdr(p1);
	while (iscons(p2)) {
		push(car(p2));
		eval();
		p3 = pop();
		if (isnum(p3) && !iszero(p3)) {
			push(cadr(p2));
			eval();
			return;
		}
		p2 = cddr(p2);
	}
	push(p1);
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
		push(p1);
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
		push(p1);
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
		push(p1);
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
		push(p1);
}

static char *s[] = {

	"1==1",
	"1",

	"1==2",
	"0",

	"a==b",
	"testeq(a,b)",

	"1>=1",
	"1",

	"1>=2",
	"0",

	"2>=1",
	"1",

	"a>=b",
	"testge(a,b)",

	"1>1",
	"0",

	"1>2",
	"0",

	"2>1",
	"1",

	"a>b",
	"testgt(a,b)",

	"1<=1",
	"1",

	"1<=2",
	"1",

	"2<=1",
	"0",

	"a<=b",
	"testle(a,b)",

	"1<1",
	"0",

	"1<2",
	"1",

	"2<1",
	"0",

	"a<b",
	"testlt(a,b)",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
