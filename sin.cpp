#include "stdafx.h"
#include "defs.h"
static void ysine(void);

void
eval_sin(void)
{
	push(cadr(p1));
	eval();
	sine();
}

void
sine(void)
{
	save();
	ysine();
	restore();
}

static void
ysine(void)
{
	int n;
	double d;

	p1 = pop();

	if (isdouble(p1)) {
		d = sin(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	if (expomode == 1) {
		push(p1);
		expsin();
		return;
	}

	// sine function is antisymmetric, sin(-x) = -sin(x)

	if (isnegative(p1)) {
		push(p1);
		negate();
		sine();
		negate();
		return;
	}

	// multiply by 180/pi

	push(p1);
	push_integer(180);
	multiply();
	push_symbol(PI);
	divide();

	n = pop_integer();

	if (n < 0) {
		push_symbol(SIN);
		push(p1);
		list(2);
		return;
	}

	switch (n % 360) {
	case 0:
		push_integer(0);
		break;
	case 30:
		push_rational(1, 2);
		break;
	case 45:
		push_rational(1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 60:
		push_rational(1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 90:
		push_integer(1);
		break;
	case 120:
		push_rational(1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 135:
		push_rational(1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 150:
		push_rational(1, 2);
		break;
	case 180:
		push_integer(0);
		break;
	case 210:
		push_rational(-1, 2);
		break;
	case 270:
		push_integer(-1);
		break;
	case 330:
		push_rational(-1, 2);
		break;
	default:
		push_symbol(SIN);
		push(p1);
		list(2);
		break;
	}
}

static char *s[] = {

	"sin(-pi)",			// -180 degrees
	"0",

	"sin(-pi*5/6)",			// -150 degrees
	"-1/2",

	"sin(-3/4*pi)+sqrt(2)/2",	// -135 degrees
	"0",

	"sin(-2/3*pi)+sqrt(3)/2",	// -120 degrees
	"0",

	"sin(-pi/2)",			// -90 degrees
	"-1",

	"sin(-1/3*pi)+sqrt(3)/2",	// -60 degrees
	"0",

	"sin(-1/4*pi)+sqrt(2)/2",	// -45 degrees
	"0",

	"sin(-pi/6)",			// -30 degrees
	"-1/2",

	"sin(0)",			// 0 degrees
	"0",

	"sin(pi/6)",			// 30 degrees
	"1/2",

	"sin(1/4*pi)-sqrt(2)/2",	// 45 degrees
	"0",

	"sin(1/3*pi)-sqrt(3)/2",	// 60 degrees
	"0",

	"sin(pi/2)",			// 90 degrees
	"1",

	"sin(2/3*pi)-sqrt(3)/2",	// 120 degrees
	"0",

	"sin(3/4*pi)-sqrt(2)/2",	// 135 degrees
	"0",

	"sin(pi*5/6)",			// 150 degrees
	"1/2",

	"sin(pi)",			// 180 degrees
	"0",

	"sin(pi*7/6)",			// 210 degrees
	"-1/2",

	// sine function is antisymmetric

	"sin(-x)",
	"-sin(x)",

	"sin(b-a)",
	"-sin(a-b)",
};

void
test_sin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
