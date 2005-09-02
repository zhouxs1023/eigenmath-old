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
		if (p2 == symbol(YYTRUE)) {
			push(cadr(p1));
			eval();
			return;
		}
		p1 = cddr(p1);
	}
	push(symbol(YYVOID));
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
		push(symbol(YYTRUE));
	else
		push(symbol(YYFALSE));
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
			push(symbol(YYFALSE));
		else
			push(symbol(YYTRUE));
	else
		push(symbol(YYVOID));
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
			push(symbol(YYTRUE));
		else
			push(symbol(YYFALSE));
	else
		push(symbol(YYVOID));
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
			push(symbol(YYFALSE));
		else
			push(symbol(YYTRUE));
	else
		push(symbol(YYVOID));
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
			push(symbol(YYTRUE));
		else
			push(symbol(YYFALSE));
	else
		push(symbol(YYVOID));
}

void
eval_not(void)
{
	push(cadr(p1));
	eval_predicate();
	p1 = pop();
	if (p1 == symbol(YYTRUE))
		push(symbol(YYFALSE));
	else if (p1 == symbol(YYFALSE))
		push(symbol(YYTRUE));
	else
		push(symbol(YYVOID));
}

void
eval_and(void)
{
	p2 = symbol(YYTRUE);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p3 = pop();
		if (p3 == symbol(YYFALSE)) {
			push(p3);
			return;
		}
		if (p3 != symbol(YYTRUE))
			p2 = symbol(YYVOID);
		p1 = cdr(p1);
	}
	push(p2);
}

void
eval_or(void)
{
	p2 = symbol(YYFALSE);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval_predicate();
		p3 = pop();
		if (p3 == symbol(YYTRUE)) {
			push(p3);
			return;
		}
		if (p3 != symbol(YYFALSE))
			p2 = symbol(YYVOID);
		p1 = cdr(p1);
	}
	push(p2);
}

static char *s[] = {

	"1==1",
	"1",

	"1==2",
	"0",

	"a==b",
	"void",

	"1>=1",
	"true",

	"1>=2",
	"false",

	"2>=1",
	"true",

	"a>=b",
	"void",

	"1>1",
	"false",

	"1>2",
	"false",

	"2>1",
	"true",

	"a>b",
	"void",

	"1<=1",
	"true",

	"1<=2",
	"true",

	"2<=1",
	"false",

	"a<=b",
	"void",

	"1<1",
	"false",

	"1<2",
	"true",

	"2<1",
	"false",

	"a<b",
	"void",

	"test(false,A,B)",
	"B",

	"test(true,A,B)",
	"A",

	"test(false,A,false,B)",
	"void",

	"test(false,A,false,B,C)",
	"C",

	"not(true)",
	"false",

	"not(false)",
	"true",

	"not(0)",
	"void",

	"not(a=a)",
	"false",

	"and(true,true)",
	"true",

	"and(true,false)",
	"false",

	"and(true,void)",
	"void",

	"and(false,void)",
	"false",

	"or(true,false)",
	"true",

	"or(false,false)",
	"false",

	"or(false,void)",
	"void",

	"or(true,void)",
	"true",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
