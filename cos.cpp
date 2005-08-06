#include "stdafx.h"
#include "defs.h"
static void ycosine(void);

void
eval_cos(void)
{
	push(cadr(p1));
	eval();
	cosine();
}

void
cosine(void)
{
	save();
	ycosine();
	restore();
}

static void
ycosine(void)
{
	int n;
	double d;

	p1 = pop();

	if (isdouble(p1)) {
		d = cos(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	if (expomode == 1) {
		push(p1);
		expcos();
		return;
	}

	// cosine function is symmetric, cos(-x) = cos(x)

	if (isnegative(p1)) {
		push(p1);
		negate();
		p1 = pop();
	}

	// multiply by 180/pi

	push(p1);
	push_integer(180);
	multiply();
	push_symbol(PI);
	divide();

	n = pop_integer();

	if (n < 0) {
		push_symbol(COS);
		push(p1);
		list(2);
		return;
	}

	switch (n % 360) {
	case 0:
		push_integer(1);
		break;
	case 60:
		push_rational(1, 2);
		break;
	case 90:
		push_integer(0);
		break;
	case 120:
		push_rational(-1, 2);
		break;
	case 180:
		push_integer(-1);
		break;
	case 240:
		push_rational(-1, 2);
		break;
	case 270:
		push_integer(0);
		break;
	case 300:
		push_rational(1, 2);
		break;
	default:
		push_symbol(COS);
		push(p1);
		list(2);
		break;
	}
}

static char *s[] = {

	"expomode=0",
	"",

	"cos(-pi)",		// -180 degrees
	"-1",

	"cos(-5/6*pi)",		// -150 degrees
	"cos(5/6*pi)",

	"cos(-3/4*pi)",		// -135 degrees
	"cos(3/4*pi)",

	"cos(-pi*2/3)",		// -120 degrees
	"-1/2",

	"cos(-pi/2)",		// -90 degrees
	"0",

	"cos(-pi/3)",		// -60 degrees
	"1/2",

	"cos(-1/4*pi)",		// -45 degrees
	"cos(1/4*pi)",

	"cos(-1/6*pi)",		// -30 degrees
	"cos(1/6*pi)",

	"cos(0)",		// 0 degrees
	"1",

	"cos(1/6*pi)",		// 30 degrees
	"cos(1/6*pi)",

	"cos(1/4*pi)",		// 45 degrees
	"cos(1/4*pi)",

	"cos(pi/3)",		// 60 degrees
	"1/2",

	"cos(pi/2)",		// 90 degrees
	"0",

	"cos(pi*2/3)",		// 120 degrees
	"-1/2",

	"cos(3/4*pi)",		// 135 degrees
	"cos(3/4*pi)",

	"cos(5/6*pi)",		// 150 degrees
	"cos(5/6*pi)",

	"cos(pi)",		// 180 degrees
	"-1",

	"expomode=1",
	"",

	"cos(x)",
	"1/2*exp(-i*x)+1/2*exp(i*x)",

	"expomode=0",
	"",

	// cosine function is symmetric

	"cos(-x)",
	"cos(x)",

	"cos(b-a)",
	"cos(a-b)",
};

void
test_cos(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
