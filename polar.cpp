/* polar(z) = mag(z) * (-1) ^ (arg(z) / pi)

For example polar(exp(i pi/3)) gives the result (-1)^(1/3)

*/

#include "stdafx.h"
#include "defs.h"

void
eval_polar(void)
{
#if 1
	push(cadr(p1));
	eval();
	p1 = pop();
	push(p1);
	mag();
	push_integer(-1);
	push(p1);
	arg();
	push(symbol(PI));
	divide();
	power();
	multiply();
#else
	push(cadr(p1));
	eval();
	p1 = pop();
	push(p1);
	mag();
	push(symbol(E));
	push(p1);
	arg();
	push(imaginaryunit);
	multiply();
	power();
	multiply();
#endif
}

static char *s[] = {

	"polar(exp(i pi/3))",
	"(-1)^(1/3)",

	"polar(exp(-i pi/3))",
	"-(-1)^(2/3)",

	"rect(polar(3+4*i))",	// needs sin(arctan(x)) and cos(arctan(x))
	"3+4*i",
};

void
test_polar(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
