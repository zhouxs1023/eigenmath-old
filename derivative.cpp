#include "stdafx.h"

#include "defs.h"

static void d_scalar_scalar_1(void);
void dsum(void);
void dproduct(void);
void dpower(void);
void dd(void);
void dlog(void);
void dfunction(void);
static void dsin(void);
static void dcos(void);
static void dtan(void);
static void darcsin(void);
static void darccos(void);
static void darctan(void);
static void dsinh(void);
static void dcosh(void);
static void dtanh(void);
static void darcsinh(void);
static void darccosh(void);
static void darctanh(void);

void
derivative(void)
{
	save();
	p2 = pop();
	p1 = pop();
	if (isnum(p2))
		stop("undefined function");
	if (istensor(p1))
		if (istensor(p2))
			d_tensor_tensor();
		else
			d_tensor_scalar();
	else
		if (istensor(p2))
			d_scalar_tensor();
		else
			d_scalar_scalar();
	restore();
}

void
d_scalar_scalar(void)
{
	if (issymbol(p2))
		d_scalar_scalar_1();
	else {
		// Example: d(sin(cos(x)),cos(x))
		// Replace cos(x) <- X, find derivative, then do X <- cos(x)
		push(p1);		// sin(cos(x))
		push(p2);		// cos(x)
		push(tmp);		// X
		subst();		// sin(cos(x)) -> sin(X)
		push(tmp);		// X
		derivative();
		push(tmp);		// X
		push(p2);		// cos(x)
		subst();		// cos(X) -> cos(cos(x))
	}
}

static void
d_scalar_scalar_1(void)
{
	// d(x,x)?

	if (equal(p1, p2)) {
		push(_one);
		return;
	}

	// d(a,x)?

	if (!iscons(p1)) {
		push(_zero);
		return;
	}

	if (isadd(p1)) {
		dsum();
		return;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		dproduct();
		return;
	}

	if (car(p1) == symbol(POWER)) {
		dpower();
		return;
	}

	if (car(p1) == symbol(DERIVATIVE)) {
		dd();
		return;
	}

	if (car(p1) == symbol(LOG)) {
		dlog();
		return;
	}

	if (car(p1) == symbol(SIN)) {
		dsin();
		return;
	}

	if (car(p1) == symbol(COS)) {
		dcos();
		return;
	}

	if (car(p1) == symbol(TAN)) {
		dtan();
		return;
	}

	if (car(p1) == symbol(ARCSIN)) {
		darcsin();
		return;
	}

	if (car(p1) == symbol(ARCCOS)) {
		darccos();
		return;
	}

	if (car(p1) == symbol(ARCTAN)) {
		darctan();
		return;
	}

	if (car(p1) == symbol(SINH)) {
		dsinh();
		return;
	}

	if (car(p1) == symbol(COSH)) {
		dcosh();
		return;
	}

	if (car(p1) == symbol(TANH)) {
		dtanh();
		return;
	}

	if (car(p1) == symbol(ARCSINH)) {
		darcsinh();
		return;
	}

	if (car(p1) == symbol(ARCCOSH)) {
		darccosh();
		return;
	}

	if (car(p1) == symbol(ARCTANH)) {
		darctanh();
		return;
	}

	dfunction();
}

void
dsum(void)
{
	int h = tos;
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		derivative();
		p1 = cdr(p1);
	}
	addk(tos - h);
}

void
dproduct(void)
{
	int i, j, n;
	n = length(p1) - 1;
	for (i = 0; i < n; i++) {
		p3 = cdr(p1);
		for (j = 0; j < n; j++) {
			push(car(p3));
			if (i == j) {
				push(p2);
				derivative();
			}
			p3 = cdr(p3);
		}
		multiply_all(n);
	}
	addk(n);
}

//-----------------------------------------------------------------------------
//
//	     v
//	y = u
//
//	log y = v log u
//
//	1 dy   v du           dv
//	- -- = - -- + (log u) --
//	y dx   u dx           dx
//
//	dy    v  v du           dv
//	-- = u  (- -- + (log u) --)
//	dx       u dx           dx
//
//-----------------------------------------------------------------------------

void
dpower(void)
{
	push(caddr(p1));	// v/u
	push(cadr(p1));
	divide();

	push(cadr(p1));		// du/dx
	push(p2);
	derivative();

	multiply();

	push(cadr(p1));		// log u
	slog();

	push(caddr(p1));	// dv/dx
	push(p2);
	derivative();

	multiply();

	add();

	push(p1);		// u^v

	multiply();
}

void
dlog(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	divide();
}

//	derivative of derivative
//
//	example: d(d(f(x,y),y),x)
//
//	p1 = d(f(x,y),y)
//
//	p2 = x
//
//	cadr(p1) = f(x,y)
//
//	caddr(p1) = y

void
dd(void)
{
	// d(f(x,y),x)

	push(cadr(p1));
	push(p2);
	derivative();

	p3 = pop();

	if (car(p3) == symbol(DERIVATIVE)) {

		// sort dx terms

		push_symbol(DERIVATIVE);
		push_symbol(DERIVATIVE);
		push(cadr(p3));

		if (lessp(caddr(p3), caddr(p1))) {
			push(caddr(p3));
			list(3);
			push(caddr(p1));
		} else {
			push(caddr(p1));
			list(3);
			push(caddr(p3));
		}

		list(3);

	} else {
		push(p3);
		push(caddr(p1));
		derivative();
	}
}

// derivative of a generic function

void
dfunction(void)
{
	p3 = cdr(p1);	// p3 is the argument list for the function

	if (p3 == nil || find(p3, p2)) {
		push_symbol(DERIVATIVE);
		push(p1);
		push(p2);
		list(3);
	} else
		push(_zero);
}

void
dsin(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	cosine();
	multiply();
}

void
dcos(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	sine();
	multiply();
	negate();
}

static void
dtan(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	cosine();
	push_integer(-2);
	power();
	multiply();
}

static void
darcsin(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push_integer(1);
	push(cadr(p1));
	push_integer(2);
	power();
	subtract();
	push_rational(-1, 2);
	power();
	multiply();
}

static void
darccos(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push_integer(1);
	push(cadr(p1));
	push_integer(2);
	power();
	subtract();
	push_rational(-1, 2);
	power();
	multiply();
	negate();
}

static void
darctan(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push_integer(1);
	push(cadr(p1));
	push_integer(2);
	power();
	add();
	inverse();
	multiply();
}

static void
dsinh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	ycosh();
	multiply();
}

static void
dcosh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	ysinh();
	multiply();
}

static void
dtanh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	ycosh();
	push_integer(-2);
	power();
	multiply();
}

static void
darcsinh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	push_integer(2);
	power();
	push_integer(1);
	add();
	push_rational(-1, 2);
	power();
	multiply();
}

static void
darccosh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push(cadr(p1));
	push_integer(2);
	power();
	push_integer(-1);
	add();
	push_rational(-1, 2);
	power();
	multiply();
}

static void
darctanh(void)
{
	push(cadr(p1));
	push(p2);
	derivative();
	push_integer(1);
	push(cadr(p1));
	push_integer(2);
	power();
	subtract();
	inverse();
	multiply();
}

static char *s[] = {

	"x=quote(x)",
	"",

	"f=quote(f)",
	"",

	"g=quote(g)",
	"",

	"d(a,x)",
	"0",

	"d(x,x)",
	"1",

	"d(x^2,x)",
	"2*x",

	"d(log(x),x)",
	"1/x",

	"d(exp(x),x)",
	"exp(x)",

	"d(a^x,x)",
	"a^x*log(a)",

	"d(x^x,x)-(x^x+x^x*log(x))",
	"0",

	"d(log(x^2+5),x)",
	"2*x/(5+x^2)",

	"d(d(f(x),x),y)",
	"0",

	"d(d(f(x),y),x)",
	"0",

	"d(d(f(y),x),y)",
	"0",

	"d(d(f(y),y),x)",
	"0",

	"d((x*y*z,y,x+z),(x,y,z))",
	"((y*z,x*z,x*y),(0,1,0),(1,0,1))",

	"d(x+z,(x,y,z))",
	"(1,0,1)",

	"d(cos(theta)^2,cos(theta))",
	"2*cos(theta)",

	"d(f())",
	"d(f(),x)",

	"d(x^2)",
	"2*x",

	"d(t^2)",
	"2*t",

	"d(t^2 x^2)",
	"2*t^2*x",

// trig functions

	"d(sin(x),x)-cos(x)",
	"0",

	"d(cos(x),x)+sin(x)",
	"0",

	"d(tan(x),x)-cos(x)^(-2)",
	"0",

	"d(arcsin(x),x)-1/sqrt(1-x^2)",
	"0",

	"d(arccos(x),x)+1/sqrt(1-x^2)",
	"0",

	"d(arctan(x),x)-1/(1+x^2)",
	"0",

// hyp functions

	"d(sinh(x),x)-cosh(x)",
	"0",

	"d(cosh(x),x)-sinh(x)",
	"0",

	"d(tanh(x),x)-cosh(x)^(-2)",
	"0",

	"d(arcsinh(x),x)-1/sqrt(x^2+1)",
	"0",

	"d(arccosh(x),x)-1/sqrt(x^2-1)",
	"0",

	"d(arctanh(x),x)-1/(1-x^2)",
	"0",

	"d(sin(cos(x)),x)+cos(cos(x))*sin(x)",
	"0",

	"d(sin(x)^2,x)-2*sin(x)*cos(x)",
	"0",

	"d(sin(cos(x)),cos(x))-cos(cos(x))",
	"0",

// generic functions

	"d(f(),x)",
	"d(f(),x)",

	"d(f(x),x)",
	"d(f(x),x)",

	"d(f(y),x)",
	"0",

	"d(g(f(x)),f(x))",
	"d(g(f(x)),f(x))",

	"d(g(f(x)),x)",
	"d(g(f(x)),x)",
};

void
test_derivative(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
