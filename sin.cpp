// Do the sine function.

#include "stdafx.h"
#include "defs.h"

static void sine_f(void);

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
	sine_f();
	restore();
}

static void
sine_f(void)
{
	int n, u;
	double d;

	p1 = pop();

	if (p1->k == DOUBLE) {
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

	// multiply by 180/pi

	push(p1);
	push_integer(180);
	multiply();
	push_symbol(PI);
	divide();

	n = pop_integer();

	if (n == (int) 0x80000000) {
		push_symbol(SIN);
		push(p1);
		list(2);
		return;
	}

	if (n < 0) {
		n = -n;
		u = -1;
	} else
		u = 1;

	switch (n % 360) {
	case 0:
		push_integer(0);
		break;
	case 30:
		push_rational(u, 2);
		break;
	case 90:
		push_integer(u);
		break;
	case 150:
		push_rational(u, 2);
		break;
	case 180:
		push_integer(0);
		break;
	case 210:
		push_rational(-u, 2);
		break;
	case 270:
		push_integer(-u);
		break;
	case 330:
		push_rational(-u, 2);
		break;
	default:
		push_symbol(SIN);
		push(p1);
		list(2);
		break;
	}
}

static char *s[] = {

	"expomode=0",
	"",

	"sin(-pi)",		// -180 degrees
	"0",

	"sin(-pi*5/6)",		// -150 degrees
	"-1/2",

	"sin(-3/4*pi)",		// -135 degrees
	"sin(-3/4*pi)",

	"sin(-2/3*pi)",		// -120 degrees
	"sin(-2/3*pi)",

	"sin(-pi/2)",		// -90 degrees
	"-1",

	"sin(-1/3*pi)",		// -60 degrees
	"sin(-1/3*pi)",

	"sin(-1/4*pi)",		// -45 degrees
	"sin(-1/4*pi)",

	"sin(-pi/6)",		// -30 degrees
	"-1/2",

	"sin(0)",		// 0 degrees
	"0",

	"sin(pi/6)",		// 30 degrees
	"1/2",

	"sin(1/4*pi)",		// 45 degrees
	"sin(1/4*pi)",

	"sin(1/3*pi)",		// 60 degrees
	"sin(1/3*pi)",

	"sin(pi/2)",		// 90 degrees
	"1",

	"sin(2/3*pi)",		// 120 degrees
	"sin(2/3*pi)",

	"sin(3/4*pi)",		// 135 degrees
	"sin(3/4*pi)",

	"sin(pi*5/6)",		// 150 degrees
	"1/2",

	"sin(pi)",		// 180 degrees
	"0",

	"sin(pi*7/6)",		// 210 degrees
	"-1/2",

	"expomode=1",
	"",

	"sin(x)",
	"1/2*i*exp(-i*x)-1/2*i*exp(i*x)",

	"expomode=0",
	"",
};

void
test_sin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
