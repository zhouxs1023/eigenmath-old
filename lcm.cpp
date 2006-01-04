// Find the least common multiple of two expressions.

#include "stdafx.h"
#include "defs.h"

void
lcm(void)
{
	int x;
	x = expanding;
	save();
	yylcm();
	restore();
	expanding = x;
}

void
yylcm(void)
{
	expanding = 1;

	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	gcd();

	push(p1);
	divide();

	push(p2);
	divide();

	inverse();
}

static char *s[] = {

	"lcm(4,6)",
	"12",

	"lcm(4*x,6*x*y)",
	"12*x*y",
};

void
test_lcm(void)
{
	test(__FILE__, s, sizeof (s) / sizeof (char *));
}
