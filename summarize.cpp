#include "stdafx.h"
#include "defs.h"

static int k;

void
eval_summarize(void)
{
	push(cadr(p1));
	eval();
	summarize();
}

void
summarize(void)
{
	Condense();
	k = 0;
	yysummarize();
}

void
yysummarize(void)
{
	int h;
	save();
	p1 = pop();
	if (iscons(p1) && find(p1, symbol(SYMBOL_X)) == 0)
		symbolize();
	else if (car(p1) == symbol(ADD)) {
		push_integer(0);
		push_integer(0);
		p1 = cdr(p1);
		while (iscons(p1)) {
			if (find(car(p1), symbol(SYMBOL_X))) {
				swap();
				push(car(p1));
				yysummarize();
				add();
				swap();
			} else {
				push(car(p1));
				add();
			}
			p1 = cdr(p1);
		}
		p1 = pop();
		if (iscons(p1))
			symbolize();
		else
			push(p1);
		add();
	} else if (car(p1) == symbol(MULTIPLY)) {
		push_integer(1);
		push_integer(1);
		p1 = cdr(p1);
		while (iscons(p1)) {
			if (find(car(p1), symbol(SYMBOL_X))) {
				swap();
				push(car(p1));
				yysummarize();
				multiply();
				swap();
			} else {
				push(car(p1));
				multiply();
			}
			p1 = cdr(p1);
		}
		p1 = pop();
		if (iscons(p1))
			symbolize();
		else
			push(p1);
		multiply();
	} else if (iscons(p1)) {
		h = tos;
		push(car(p1));
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			yysummarize();
			p1 = cdr(p1);
		}
		list(tos - h);
	} else
		push(p1);
	restore();
}

void
symbolize(void)
{
	switch (++k) {
	case 1:
		p2 = symbol(C1);
		break;
	case 2:
		p2 = symbol(C2);
		break;
	case 3:
		p2 = symbol(C3);
		break;
	case 4:
		p2 = symbol(C4);
		break;
	case 5:
		p2 = symbol(C5);
		break;
	case 6:
		p2 = symbol(C6);
		break;
	default:
		stop("too many symbols");
	}
	p2->u.sym.binding = p1;
	push(p2);
}
