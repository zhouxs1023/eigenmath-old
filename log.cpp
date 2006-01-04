// natural logarithm

#include "stdafx.h"
#include "defs.h"

void
eval_log(void)
{
	push(cadr(p1));
	eval();
	logarithm();
}

void
logarithm(void)
{
	save();
	yylog();
	restore();
}

void
yylog(void)
{
	double d;

	p1 = pop();

	if (isdouble(p1)) {
		d = p1->u.d;
		if (d < 0.0) {
			d = log(-d);
			push_double(d);
			push(imaginaryunit);
			push_symbol(PI);
			multiply();
			add();
		} else {
			d = log(d);
			push_double(d);
		}
		return;
	}

	if (equal(p1, one)) {
		push_integer(0);
		return;
	}

	if (p1 == symbol(E)) {
		push(one);
		return;
	}

	if (car(p1) == symbol(POWER)) {
		push(caddr(p1));
		push(cadr(p1));
		logarithm();
		multiply();
		return;
	}

	push_symbol(LOG);
	push(p1);
	list(2);
}

static char *s[] = {

	"log(1)",
	"0",

	"log(exp(1))",
	"1",

	"log(exp(x))",
	"x",

	"exp(log(x))",
	"x",

	"log(x^2)",
	"2*log(x)",

	"log(1/x)",
	"-log(x)",

	"log(a^b)",
	"b*log(a)",

	"log(2)",
	"log(2)",

	"log(2.0)",
	"0.693147",

	"float(log(2))",
	"0.693147",
};

void
test_log(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
