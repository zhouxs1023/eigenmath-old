#include "stdafx.h"

#include "defs.h"

static void __tangent(void);

void
eval_tan(void)
{
	push(cadr(p1));
	eval();
	tangent();
}

void
tangent(void)
{
	save();
	__tangent();
	restore();
}

static void
__tangent(void)
{
	int n;
	double d;

	p1 = pop();

	if (p1->k == DOUBLE) {
		d = tan(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	if (expomode == 1) {
		push(unit_imaginary);
		push(p1);
		multiply();
		exponential();
		p2 = pop();
		push(unit_imaginary);
		push(p1);
		multiply();
		negate();
		exponential();
		p3 = pop();
		push(p3);
		push(p2);
		subtract();
		push(unit_imaginary);
		multiply();
		push(p2);
		push(p3);
		add();
		divide();
		return;
	}

	// multiply by 180/pi

	push(p1);
	push_integer(180);
	multiply();
	push_symbol(PI);
	divide();

	n = pop_integer();

	if (n == (int) 0x80000000) {
		push_symbol(TAN);
		push(p1);
		list(2);
		return;
	}

	switch(n % 360) {
	case -315:
		push_integer(1);
		break;
	case -225:
		push_integer(-1);
		break;
	case -180:
		push_integer(0);
		break;
	case -135:
		push_integer(1);
		break;
	case -45:
		push_integer(-1);
		break;
	case 0:
		push_integer(0);
		break;
	case 45:
		push_integer(1);
		break;
	case 135:
		push_integer(-1);
		break;
	case 180:
		push_integer(0);
		break;
	case 225:
		push_integer(1);
		break;
	case 315:
		push_integer(-1);
		break;
	default:
		push_symbol(TAN);
		push(p1);
		list(2);
		break;
	}
}

static char *s[] = {

	"tan(x)",
	"tan(x)",

	"tan(-2 pi)",
	"0",

	"tan(-7 pi/4)",
	"1",

	"tan(-5 pi/4)",
	"-1",

	"tan(-pi)",
	"0",

	"tan(-3 pi/4)",
	"1",

	"tan(-pi/4)",
	"-1",

	"tan(0)",
	"0",

	"tan(pi/4)",
	"1",

	"tan(3 pi/4)",
	"-1",

	"tan(pi)",
	"0",

	"tan(5 pi/4)",
	"1",

	"tan(7 pi/4)",
	"-1",

	"tan(2 pi)",
	"0",

	"tan(-2 pi)+tan(float(-2 pi))",
	"0",

	"tan(-7 pi/4)+tan(float(-7 pi/4))",
	"2",

	"tan(-5 pi/4)+tan(float(-5 pi/4))",
	"-2",

	"tan(-pi)+tan(float(-pi))",
	"0",

	"tan(-3 pi/4)+tan(float(-3 pi/4))",
	"2",

	"tan(-pi/4)+tan(float(-pi/4))",
	"-2",

	"tan(0)+tan(0.0)",
	"0",

	"tan(pi/4)+tan(float(pi/4))",
	"2",

	"tan(3 pi/4)+tan(float(3 pi/4))",
	"-2",

	"tan(pi)+tan(float(pi))",
	"0",

	"tan(5 pi/4)+tan(float(5 pi/4))",
	"2",

	"tan(7 pi/4)+tan(float(7 pi/4))",
	"-2",

	"tan(2 pi)+tan(float(2 pi))",
	"0",

	"expomode=1",
	"",

	"tan(x)",
	"i*exp(-i*x)/(exp(-i*x)+exp(i*x))-i*exp(i*x)/(exp(-i*x)+exp(i*x))",

	"expomode=0",
	"",
};

void
test_tan(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
