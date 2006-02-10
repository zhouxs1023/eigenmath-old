/* polar(z) = mag(z) * (-1) ^ (arg(z) / pi)

For example polar(exp(i pi/3)) gives the result (-1)^(1/3)

*/

#include "stdafx.h"
#include "defs.h"

void
eval_polar(void)
{
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
}

static char *s[] = {

	"polar(exp(i pi/3))",
	"(-1)^(1/3)",

	"polar(exp(-i pi/3))",
	"-(-1)^(2/3)",
};

void
test_polar(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
