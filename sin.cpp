// Sine function of numerical and symbolic arguments

#include "stdafx.h"
#include "defs.h"

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
	yysine();
	restore();
}

void
yysine(void)
{
	int n;
	double d;

	p1 = pop();

	if (car(p1) == symbol(ARCSIN)) {
		push(cadr(p1));
		return;
	}

	if (isdouble(p1)) {
		d = sin(p1->u.d);
		if (fabs(d) < 1e-10)
			d = 0.0;
		push_double(d);
		return;
	}

	// sin(arctan(x)) = x / sqrt(1 + x^2)

	// see p. 173 of the CRC Handbook of Mathematical Sciences

	if (car(p1) == symbol(ARCTAN)) {
		push(cadr(p1));
		push_integer(1);
		push(cadr(p1));
		push_integer(2);
		power();
		add();
		push_rational(-1, 2);
		power();
		multiply();
		return;
	}
	if (exp_flag) {
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
		push(symbol(SIN));
		push(p1);
		list(2);
		return;
	}

	switch (n % 360) {
	case 0:
	case 180:
		push_integer(0);
		break;
	case 30:
	case 150:
		push_rational(1, 2);
		break;
	case 210:
	case 330:
		push_rational(-1, 2);
		break;
	case 45:
	case 135:
		push_rational(1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 225:
	case 315:
		push_rational(-1, 2);
		push_integer(2);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 60:
	case 120:
		push_rational(1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 240:
	case 300:
		push_rational(-1, 2);
		push_integer(3);
		push_rational(1, 2);
		power();
		multiply();
		break;
	case 90:
		push_integer(1);
		break;
	case 270:
		push_integer(-1);
		break;
	default:
		push(symbol(SIN));
		push(p1);
		list(2);
		break;
	}
}

static char *s[] = {

	"sin(x)",
	"sin(x)",

	"sin(-x)",
	"-sin(x)",

	"sin(b-a)",
	"-sin(a-b)",

	// check against the floating point math library

	"f(a,x)=1+sin(float(a/360*2*pi))-float(x)+sin(a/360*2*pi)-x",
	"",

	"f(0,0)",			// 0
	"1",

	"f(90,1)",			// 90
	"1",

	"f(180,0)",			// 180
	"1",

	"f(270,-1)",			// 270
	"1",

	"f(360,0)",			// 360
	"1",

	"f(-90,-1)",			// -90
	"1",

	"f(-180,0)",			// -180
	"1",

	"f(-270,1)",			// -270
	"1",

	"f(-360,0)",			// -360
	"1",

	"f(45,sqrt(2)/2)",		// 45
	"1",

	"f(135,sqrt(2)/2)",		// 135
	"1",

	"f(225,-sqrt(2)/2)",		// 225
	"1",

	"f(315,-sqrt(2)/2)",		// 315
	"1",

	"f(-45,-sqrt(2)/2)",		// -45
	"1",

	"f(-135,-sqrt(2)/2)",		// -135
	"1",

	"f(-225,sqrt(2)/2)",		// -225
	"1",

	"f(-315,sqrt(2)/2)",		// -315
	"1",

	"f(30,1/2)",			// 30
	"1",

	"f(150,1/2)",			// 150
	"1",

	"f(210,-1/2)",			// 210
	"1",

	"f(330,-1/2)",			// 330
	"1",

	"f(-30,-1/2)",			// -30
	"1",

	"f(-150,-1/2)",			// -150
	"1",

	"f(-210,1/2)",			// -210
	"1",

	"f(-330,1/2)",			// -330
	"1",

	"f(60,sqrt(3)/2)",		// 60
	"1",

	"f(120,sqrt(3)/2)",		// 120
	"1",

	"f(240,-sqrt(3)/2)",		// 240
	"1",

	"f(300,-sqrt(3)/2)",		// 300
	"1",

	"f(-60,-sqrt(3)/2)",		// -60
	"1",

	"f(-120,-sqrt(3)/2)",		// -120
	"1",

	"f(-240,sqrt(3)/2)",		// -240
	"1",

	"f(-300,sqrt(3)/2)",		// -300
	"1",

	"f=quote(f)",
	"",

	"sin(arcsin(x))",
	"x",

	"circexp(sin(x))",
	"1/2*i*exp(-i*x)-1/2*i*exp(i*x)",
};

void
test_sin(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
