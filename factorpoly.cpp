#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Factor polynomial
//
//	Input:		tos-2		p(x)
//
//			tos-1		x
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#include "defs.h"

#define POLY p1
#define X p2
#define Z p3
#define A p4
#define B p5
#define Q p6
#define RESULT p7
#define FACTOR p8

static void factorpoly_auto(void);
static void factorpoly2(void);
static void factorpoly3(void);
static void factorpoly4(void);
static void rationalize_coefficients(int);
static int get_factor(void);
static void evalpoly(void);
static void divpoly(void);
static void __lcm(void);
int isnegativeterm(U *);

static int expo;
static U **polycoeff;

void
factorpoly(void)
{
	int i, n;
	save();
	p2 = pop();
	p1 = pop();
	if (p2 == nil) {
		push(p1);
		factorpoly_auto();
		restore();
		return;
	}
	if (p2->k == TENSOR) {
		n = p2->u.tensor->nelem;
		push(p1);
		for (i = 0; i < n; i++) {
			push(p2->u.tensor->elem[i]);
			factorpoly2();
		}
	} else {
		push(p1);
		push(p2);
		factorpoly2();
	}
	restore();
}

// scan to find vars

static void
factorpoly_auto(void)
{
	int i, n;
	save();
	p1 = pop();
	push(p1);
	variables();
	p2 = pop();
	if (p2 == nil) {
		push(p1);
		restore();
		return;
	}
	n = p2->u.tensor->nelem;
	push(p1);
	for (i = 0; i < n; i++) {
		push(p2->u.tensor->elem[i]);
		factorpoly2();
	}
	restore();
}

// condense first then rationalize

// would it be better the other way around?

static void
factorpoly2(void)
{
	save();
	p2 = pop();
	condense();
	p1 = pop();
	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		push(_one);
		while (iscons(p1)) {
			push(car(p1));
			if (caar(p1) == symbol(ADD)) {
				push(p2);
				factorpoly3();
			}
			multiply_noexpand();
			p1 = cdr(p1);
		}
	} else if (car(p1) == symbol(ADD)) {
		push(p1);
		push(p2);
		factorpoly3();
	} else
		push(p1);
	restore();
}

//-----------------------------------------------------------------------------
//
//	Input:		tos-2		sum expression (may be a polynomial)
//
//			tos-1		free variable
//
//	Output:		expression on stack (may be factored)
//
//-----------------------------------------------------------------------------

void
factorpoly3(void)
{
	save();

	p2 = pop();
	p1 = pop();

	if (ispoly(p1, p2)) {
		push(p1);
		push(p2);
		factorpoly4();
		restore();
		return;
	}

	// try rationalizing

	push(p1);
	rationalize();
	p1 = pop();

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		push(_one);
		while (iscons(p1)) {
			push(car(p1));
			if (ispoly(car(p1), p2)) {
				push(p2);
				factorpoly4();
			}
			multiply_noexpand();
			p1 = cdr(p1);
		}
	} else if (ispoly(p1, p2)) {
		push(p1);
		push(p2);
		factorpoly4();
	} else
		push(p1);

	restore();
}

//-----------------------------------------------------------------------------
//
//	Input:		tos-2		true polynomial
//
//			tos-1		free variable
//
//	Output:		factored polynomial on stack
//
//-----------------------------------------------------------------------------

static void
factorpoly4(void)
{
	int h, i;

	save();

	X = pop();
	POLY = pop();

	h = tos;

	polycoeff = stack + tos;

	push(POLY);
	push(X);
	expo = coeff() - 1;

	rationalize_coefficients(h);

	// for univariate polynomials we could do expo > 1

	while (expo > 0) {

		if (get_factor() == 0) {
			if (verbosing)
				printf("no factor found\n");
			break;
		}

		push(A);
		push(X);
		multiply();
		push(B);
		add();
		FACTOR = pop();

		if (verbosing) {
			printf("success\nFACTOR=");
			print(FACTOR);
			printf("\n");
		}

		// factor out negative sign (not req'd because A > 1)
#if 0
		if (isnegativeterm(A)) {
			push(FACTOR);
			negate();
			FACTOR = pop();
			push(RESULT);
			negate_noexpand();
			RESULT = pop();
		}
#endif
		push(RESULT);
		push(FACTOR);
		multiply_noexpand();
		RESULT = pop();

		divpoly();

		while (expo && iszero(polycoeff[expo]))
			expo--;
	}

	// unfactored polynomial

	push(_zero);
	for (i = 0; i <= expo; i++) {
		push(polycoeff[i]);
		push(X);
		push_integer(i);
		power();
		multiply();
		add();
	}
	POLY = pop();

	if (verbosing) {
		printf("POLY=");
		print(POLY);
		printf("\n");
	}

	// factor out negative sign

	if (expo > 0 && isnegativeterm(polycoeff[expo])) {
		push(POLY);
		negate();
		POLY = pop();
		push(RESULT);
		negate_noexpand();
		RESULT = pop();
	}

	push(RESULT);
	push(POLY);
	multiply_noexpand();
	RESULT = pop();

	if (verbosing) {
		printf("RESULT=");
		print(RESULT);
		printf("\n");
	}

	stack[h] = RESULT;

	tos = h + 1;

	restore();
}

static void
rationalize_coefficients(int h)
{
	int i;
	if (verbosing)
		printf("rationalizing coefficients\n");
	RESULT = _one;
	for (i = h; i < tos; i++) {
		if (isnum(stack[i])) {
			push(stack[i]);
			denominator();
			push(RESULT);
			__lcm();
			RESULT = pop();
		} else if (car(stack[i]) == symbol(MULTIPLY) && isnum(cadr(stack[i]))) {
			push(cadr(stack[i]));
			denominator();
			push(RESULT);
			__lcm();
			RESULT = pop();
		}
	}
	for (i = h; i < tos; i++) {
		if (isnum(stack[i])) {
			push(stack[i]);
			push(RESULT);
			multiply();
			stack[i] = pop();
		} else if (car(stack[i]) == symbol(MULTIPLY) && isnum(cadr(stack[i]))) {
			push(stack[i]);
			push(RESULT);
			multiply();
			stack[i] = pop();
		}
	}
	push(RESULT);
	inverse();
	RESULT = pop();
	if (verbosing) {
		printf("RESULT=");
		print(RESULT);
	}
}

static int
get_factor(void)
{
	int i, j, h;
	int a0, an, na0, nan;

	if (verbosing) {
		push(_zero);
		for (i = 0; i <= expo; i++) {
			push(polycoeff[i]);
			push(X);
			push_integer(i);
			power();
			multiply();
			add();
		}
		POLY = pop();
		printf("POLY=");
		print(POLY);
		printf("\n");
	}

	h = tos;

	an = tos;
	push(polycoeff[expo]);

	divisors_onstack();

	nan = tos - an;

	a0 = tos;
	push(polycoeff[0]);
	divisors_onstack();
	na0 = tos - a0;

	if (verbosing) {
		printf("divisors of base term");
		for (i = 0; i < na0; i++) {
			printf(", ");
			print(stack[a0 + i]);
		}
		printf("\n");
		printf("divisors of leading term");
		for (i = 0; i < nan; i++) {
			printf(", ");
			print(stack[an + i]);
		}
		printf("\n");
	}

	// try roots

	for (i = 0; i < nan; i++) {
		for (j = 0; j < na0; j++) {

			A = stack[an + i];
			B = stack[a0 + j];

			push(B);
			push(A);
			divide();
			negate();
			Z = pop();

			evalpoly();

			if (verbosing) {
				printf("try A=");
				print(A);
				printf(", B=");
				print(B);
				printf(", root ");
				print(X);
				printf("=-B/A=");
				print(Z);
				printf(", POLY(");
				print(Z);
				printf(")=");
				print(Q);
				printf("\n");
			}

			if (iszero(Q)) {
				tos = h;
				return 1;
			}

			push(B);
			negate();
			B = pop();

			push(Z);
			negate();
			Z = pop();

			evalpoly();

			if (verbosing) {
				printf("try A=");
				print(A);
				printf(", B=");
				print(B);
				printf(", root ");
				print(X);
				printf("=-B/A=");
				print(Z);
				printf(", POLY(");
				print(Z);
				printf(")=");
				print(Q);
				printf("\n");
			}

			if (iszero(Q)) {
				tos = h;
				return 1;
			}
		}
	}

	tos = h;

	return 0;
}

//-----------------------------------------------------------------------------
//
//	Divide a polynomial by Ax+B
//
//	Input:		polycoeff	Dividend coefficients
//
//			expo		Degree of dividend
//
//			A		As above
//
//			B		As above
//
//	Output:		polycoeff	Contains quotient coefficients
//
//-----------------------------------------------------------------------------

static void
divpoly(void)
{
	int i;
	Q = _zero;
	for (i = expo; i > 0; i--) {
		push(polycoeff[i]);
		polycoeff[i] = Q;
		push(A);
		divide();
		Q = pop();
		push(polycoeff[i - 1]);
		push(Q);
		push(B);
		multiply();
		subtract();
		polycoeff[i - 1] = pop();
	}
	polycoeff[0] = Q;
}

static void
evalpoly(void)
{
	int i;
	push(_zero);
	for (i = expo; i >= 0; i--) {
		push(Z);
		multiply();
		push(polycoeff[i]);
		add();
	}
	Q = pop();
}

static char *s[] = {

	"factor((x+1)*(x+2)*(x+3),x)",
	"(1+x)*(2+x)*(3+x)",

	"factor((x+a)*(x^2+x+1),x)",
	"(1+x+x^2)*(a+x)",

	"factor(x*(x+1)*(x+2),x)",
	"x*(1+x)*(2+x)",

	"factor((x+1)*(x+2)*(y+3)*(y+4),x,y)",
	"(1+x)*(2+x)*(3+y)*(4+y)",

	"factor((x+1)*(x+2)*(y+3)*(y+4),(x,y))",
	"(1+x)*(2+x)*(3+y)*(4+y)",

	"factor((x+1)*(x+2)*(y+3)*(y+4))",
	"(1+x)*(2+x)*(3+y)*(4+y)",

	"factor((-2*x+3)*(x+4),x)",
	"-(-3+2*x)*(4+x)",

	"(-2*x+3)*(x+4)+(-3+2*x)*(4+x)",
	"0",

	// make sure sign of remaining poly is factored

	"factor((x+1)*(-x^2+x+1),x)",
	"-(-1-x+x^2)*(1+x)",

// sign handling

//++++++

	"factor((x+1/2)*(+x+1/3)*(+x+1/4),x)",
	"1/24*(1+2*x)*(1+3*x)*(1+4*x)",

	"(x+1/2)*(+x+1/3)*(+x+1/4)-1/24*(1+2*x)*(1+3*x)*(1+4*x)",
	"0",

//+++++-

	"factor((x+1/2)*(+x+1/3)*(+x-1/4),x)",
	"1/24*(-1+4*x)*(1+2*x)*(1+3*x)",

	"(x+1/2)*(+x+1/3)*(+x-1/4)-1/24*(-1+4*x)*(1+2*x)*(1+3*x)",
	"0",

//++++-+

	"factor((x+1/2)*(+x+1/3)*(-x+1/4),x)",
	"-1/24*(-1+4*x)*(1+2*x)*(1+3*x)",

	"(x+1/2)*(+x+1/3)*(-x+1/4)+1/24*(-1+4*x)*(1+2*x)*(1+3*x)",
	"0",

//++++--

	"factor((x+1/2)*(+x+1/3)*(-x-1/4),x)",
	"-1/24*(1+2*x)*(1+3*x)*(1+4*x)",

	"(x+1/2)*(+x+1/3)*(-x-1/4)+1/24*(1+2*x)*(1+3*x)*(1+4*x)",
	"0",

//+++-++

	"factor((x+1/2)*(+x-1/3)*(+x+1/4),x)",
	"1/24*(-1+3*x)*(1+2*x)*(1+4*x)",

	"(x+1/2)*(+x-1/3)*(+x+1/4)-1/24*(-1+3*x)*(1+2*x)*(1+4*x)",
	"0",

//+++-+-

	"factor((x+1/2)*(+x-1/3)*(+x-1/4),x)",
	"1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",

	"(x+1/2)*(+x-1/3)*(+x-1/4)-1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",
	"0",

//+++--+

	"factor((x+1/2)*(+x-1/3)*(-x+1/4),x)",
	"-1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",

	"(x+1/2)*(+x-1/3)*(-x+1/4)+1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",
	"0",

//+++---

	"factor((x+1/2)*(+x-1/3)*(-x-1/4),x)",
	"-1/24*(-1+3*x)*(1+2*x)*(1+4*x)",

	"(x+1/2)*(+x-1/3)*(-x-1/4)+1/24*(-1+3*x)*(1+2*x)*(1+4*x)",
	"0",

//++-+++

	"factor((x+1/2)*(-x+1/3)*(+x+1/4),x)",
	"-1/24*(-1+3*x)*(1+2*x)*(1+4*x)",

	"(x+1/2)*(-x+1/3)*(+x+1/4)+1/24*(-1+3*x)*(1+2*x)*(1+4*x)",
	"0",

//++-++-

	"factor((x+1/2)*(-x+1/3)*(+x-1/4),x)",
	"-1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",

	"(x+1/2)*(-x+1/3)*(+x-1/4)+1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",
	"0",

//++-+-+

	"factor((x+1/2)*(-x+1/3)*(-x+1/4),x)",
	"1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",

	"(x+1/2)*(-x+1/3)*(-x+1/4)-1/24*(-1+3*x)*(-1+4*x)*(1+2*x)",
	"0",

//++-+--

	"factor((x+1/2)*(-x+1/3)*(-x-1/4),x)",
	"1/24*(-1+3*x)*(1+2*x)*(1+4*x)",

	"(x+1/2)*(-x+1/3)*(-x-1/4)-1/24*(-1+3*x)*(1+2*x)*(1+4*x)",
	"0",

//++--++

	"factor((x+1/2)*(-x-1/3)*(+x+1/4),x)",
	"-1/24*(1+2*x)*(1+3*x)*(1+4*x)",

	"(x+1/2)*(-x-1/3)*(+x+1/4)+1/24*(1+2*x)*(1+3*x)*(1+4*x)",
	"0",

//++--+-

	"factor((x+1/2)*(-x-1/3)*(+x-1/4),x)",
	"-1/24*(-1+4*x)*(1+2*x)*(1+3*x)",

	"(x+1/2)*(-x-1/3)*(+x-1/4)+1/24*(-1+4*x)*(1+2*x)*(1+3*x)",
	"0",

//++---+

	"factor((x+1/2)*(-x-1/3)*(-x+1/4),x)",
	"1/24*(-1+4*x)*(1+2*x)*(1+3*x)",

	"(x+1/2)*(-x-1/3)*(-x+1/4)-1/24*(-1+4*x)*(1+2*x)*(1+3*x)",
	"0",

//++----

	"factor((x+1/2)*(-x-1/3)*(-x-1/4),x)",
	"1/24*(1+2*x)*(1+3*x)*(1+4*x)",

	"(x+1/2)*(-x-1/3)*(-x-1/4)-1/24*(1+2*x)*(1+3*x)*(1+4*x)",
	"0",

//++++++

	"factor((+x+a)*(+x+b)*(+x+c),x)",
	"(a+x)*(b+x)*(c+x)",

	"(a+x)*(b+x)*(c+x)-(a+x)*(b+x)*(c+x)",
	"0",

//+++++-

	"factor((+x+a)*(+x+b)*(+x-c),x)",
	"(a+x)*(b+x)*(-c+x)",

	"(+x+a)*(+x+b)*(+x-c)-(a+x)*(b+x)*(-c+x)",
	"0",

//++++-+

	"factor((+x+a)*(+x+b)*(-x+c),x)",
	"-(a+x)*(b+x)*(-c+x)",

	"(+x+a)*(+x+b)*(-x+c)+(a+x)*(b+x)*(-c+x)",
	"0",

//++++--

	"factor((+x+a)*(+x+b)*(-x-c),x)",
	"-(a+x)*(b+x)*(c+x)",

	"(+x+a)*(+x+b)*(-x-c)+(a+x)*(b+x)*(c+x)",
	"0",

//++++

	"factor((+a*x+b)*(+c*x+d),x)",
	"(b+a*x)*(d+c*x)",

	"(+a*x+b)*(+c*x+d)-(b+a*x)*(d+c*x)",
	"0",

//+++-

	"factor((+a*x+b)*(+c*x-d),x)",
	"(b+a*x)*(-d+c*x)",

	"(+a*x+b)*(+c*x-d)-(b+a*x)*(-d+c*x)",
	"0",

//++-+

	"factor((+a*x+b)*(-c*x+d),x)",
	"-(b+a*x)*(-d+c*x)",

	"(+a*x+b)*(-c*x+d)+(b+a*x)*(-d+c*x)",
	"0",

//++--

	"factor((+a*x+b)*(-c*x-d),x)",
	"-(b+a*x)*(d+c*x)",

	"(+a*x+b)*(-c*x-d)+(b+a*x)*(d+c*x)",
	"0",

//+-++

	"factor((+a*x-b)*(+c*x+d),x)",
	"(d+c*x)*(-b+a*x)",

	"(+a*x-b)*(+c*x+d)-(d+c*x)*(-b+a*x)",
	"0",

//+-+-

	"factor((+a*x-b)*(+c*x-d),x)",
	"(-b+a*x)*(-d+c*x)",

	"(+a*x-b)*(+c*x-d)-(-b+a*x)*(-d+c*x)",
	"0",

//+--+

	"factor((+a*x-b)*(-c*x+d),x)",
	"-(-b+a*x)*(-d+c*x)",

	"(+a*x-b)*(-c*x+d)+(-b+a*x)*(-d+c*x)",
	"0",

//+---

	"factor((+a*x-b)*(-c*x-d),x)",
	"-(d+c*x)*(-b+a*x)",

	"(+a*x-b)*(-c*x-d)+(d+c*x)*(-b+a*x)",
	"0",

//-+++

	"factor((-a*x+b)*(+c*x+d),x)",
	"-(d+c*x)*(-b+a*x)",

	"(-a*x+b)*(+c*x+d)+(d+c*x)*(-b+a*x)",
	"0",

//-++-

	"factor((-a*x+b)*(+c*x-d),x)",
	"-(-b+a*x)*(-d+c*x)",

	"(-a*x+b)*(+c*x-d)+(-b+a*x)*(-d+c*x)",
	"0",

//-+-+

	"factor((-a*x+b)*(-c*x+d),x)",
	"(-b+a*x)*(-d+c*x)",

	"(-a*x+b)*(-c*x+d)-(-b+a*x)*(-d+c*x)",
	"0",

//-+--

	"factor((-a*x+b)*(-c*x-d),x)",
	"(d+c*x)*(-b+a*x)",

	"(-a*x+b)*(-c*x-d)-(d+c*x)*(-b+a*x)",
	"0",

//--++

	"factor((-a*x-b)*(+c*x+d),x)",
	"-(b+a*x)*(d+c*x)",

	"(-a*x-b)*(+c*x+d)+(b+a*x)*(d+c*x)",
	"0",

//--+-

	"factor((-a*x-b)*(+c*x-d),x)",
	"-(b+a*x)*(-d+c*x)",

	"(-a*x-b)*(+c*x-d)+(b+a*x)*(-d+c*x)",
	"0",

//---+

	"factor((-a*x-b)*(-c*x+d),x)",
	"(b+a*x)*(-d+c*x)",

	"(-a*x-b)*(-c*x+d)-(b+a*x)*(-d+c*x)",
	"0",

//----

	"factor((-a*x-b)*(-c*x-d),x)",
	"(b+a*x)*(d+c*x)",

	"(-a*x-b)*(-c*x-d)-(b+a*x)*(d+c*x)",
	"0",

	// this used to cause divide by zero

	// fixed by calling ispoly before calling coeff

	"factor(1/x+1)",
	"(1+x)/x",

	// see if poly gets rationalized

	"(x+1)(x+2)(x+3)/x^3",
	"1+6/(x^3)+11/(x^2)+6/x",

	"factor(last)",
	"(1+x)*(2+x)*(3+x)/(x^3)",

	// this used to fail

	"factor(x,x)",
	"x",

	"factor(x^2,x)",
	"x^2",

	"factor(x^3,x)",
	"x^3",
};

void
test_factorpoly(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

static void
__lcm(void)
{
	save();
	p2 = pop();
	p1 = pop();
	push(p1);
	push(p2);
	multiply();
	push(p1);
	push(p2);
	gcd();
	divide();
	absval();
	restore();
}
