// Do the integral function.

#include "stdafx.h"
#include "defs.h"

static void integral_f(void);
static void integral_of_sum(void);
static void integral_of_product(void);
static void integral_of_form(void);
static int match(U *, U *, U *, int, int);
static void scan_integrals(void);

void
eval_integral(void)
{
	push(cadr(p1));
	eval();
	if (caddr(p1) == nil)
		guess();
	else {
		push(caddr(p1));
		eval(); // need this for integrals in the result, see #64
	}
	integral();
}

void
integral(void)
{
	save();
	integral_f();
	restore();
}

static void
integral_f(void)
{
	if (table_of_integrals == nil)
		scan_integrals();

	p2 = pop();
	p1 = pop();

	if (car(p1) == symbol(ADD))
		integral_of_sum();
	else if (car(p1) == symbol(MULTIPLY))
		integral_of_product();
	else
		integral_of_form();
}

static void
integral_of_form(void)
{
	int h;

	// save meta symbols

	push(meta_a->u.sym.binding);
	push(meta_b->u.sym.binding);
	push(meta_x->u.sym.binding);

	meta_x->u.sym.binding = p2;

	h = tos;

	push(one);

	push(p1);
	push(p2);

	distill();

	p3 = table_of_integrals;

	while (iscons(p3)) {

		p4 = car(p3);

		if (match(p1, car(p4), cddr(p4), h, tos))
			break;

		p3 = cdr(p3);
	}

	if (iscons(p3)) {
		push(cadr(p4));
		eval();
	} else {
		push_symbol(INTEGRAL);
		push(p1);
		push(p2);
		list(3);
	}

	p3 = pop();

	tos = h;

	// restore meta symbols

	meta_x->u.sym.binding = pop();
	meta_b->u.sym.binding = pop();
	meta_a->u.sym.binding = pop();

	push(p3);
}

static void
integral_of_sum(void)
{
	p1 = cdr(p1);
	push(car(p1));
	push(p2);
	integral();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		integral();
		add();
		p1 = cdr(p1);
	}
}

static void
integral_of_product(void)
{
	int h1, h2;

	h1 = tos;

	p3 = cdr(p1);
	while (iscons(p3)) {
		if (!find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	h2 = tos;

	p3 = cdr(p1);
	while (iscons(p3)) {
		if (find(car(p3), p2))
			push(car(p3));
		p3 = cdr(p3);
	}

	if (tos - h2 == 0)
		push(p2);
	else {
		multiply_all(tos - h2);
		p1 = pop();
		integral_of_form();
	}

	multiply_all(tos - h1);
}

static int
match(U *actual, U *formal, U *caveats, int h1, int h2)
{
	int i, j;

	save();

	for (i = h1; i < h2; i++) {
		for (j = h1; j < h2; j++) {

			meta_a->u.sym.binding = stack[i];
			meta_b->u.sym.binding = stack[j];

			// check caveats

			p1 = caveats;

			while (iscons(p1)) {
				push(car(p1));
				eval();
				p2 = pop();
				if (!isnum(p2) || iszero(p2))
					break;
				p1 = cdr(p1);
			}

			if (iscons(p1))
				continue;

			// actual == formal?

			push(formal);

			eval();

			if (equal(actual, pop())) {
				restore();
				return 1;
			}
		}
	}

	restore();

	return 0;
}

char *integrals[] = {

// 1

	"a",
	"a*x",
	NULL,

// 9 (need a caveat for 7 so we can put 9 after 7)

	"1/x",
	"log(x)",
	NULL,

// 7

	"x^a",
	"x^(a+1)/(a+1)",
	NULL,

// 12

	"exp(a*x)",
	"1/a*exp(a*x)",
	NULL,

	"exp(a*x+b)",
	"1/a*exp(a*x+b)",
	NULL,

	"x*exp(a*x^2)",
	"exp(a*x^2)/(2*a)",
	NULL,

	"x*exp(a*x^2+b)",
	"exp(a*x^2+b)/(2*a)",
	NULL,

// 14
	"log(a*x)",
	"x*log(a*x)-x",
	NULL,
// 15
	"a^x",
	"a^x/log(a)",
	"a>0",
	NULL,
// 16
	"1/(a+x^2)",
	"1/sqrt(a)*arctan(x/sqrt(a))",
	"a>0",
	NULL,

// 17

	"1/(a-x^2)",
	"1/sqrt(a)*arctanh(x/sqrt(a))",
	NULL,

// 19

	"1/sqrt(a-x^2)",
	"arcsin(x/(sqrt(a)))",
	NULL,

// 20

	"1/sqrt(a+x^2)",
	"log(x+sqrt(a+x^2))",
	NULL,

// 27

	"1/(a+b*x)",
	"1/b*log(a+b*x)",
	NULL,

// 28

	"1/(a+b*x)^2",
	"-1/(b*(a+b*x))",
	NULL,

//	"1/(2*a*b*x+a^2+b^2*x^2)", // can't math expanded poly
//	"-1/b*1/(a+b*x)",
//	NULL,

// 29
	"1/(a+b*x)^3",
	"-1/(2*b)*1/(a+b*x)^2",
	NULL,
// 30
	"x/(a+b*x)",
	"x/b-a*log(a+b*x)/b/b",
	NULL,
// 31
	"x/(a+b*x)^2",
	"1/b^2*(log(a+b*x)+a/(a+b*x))",
	NULL,
// 33
	"x^2/(a+b*x)",
	"1/b^2*(1/2*(a+b*x)^2-2*a*(a+b*x)+a^2*log(a+b*x))",
	NULL,
// 34
	"x^2/(a+b*x)^2",
	"1/b^3*(a+b*x-2*a*log(a+b*x)-a^2/(a+b*x))",
	NULL,
// 35
	"x^2/(a+b*x)^3",
	"1/b^3*(log(a+b*x)+2*a/(a+b*x)-1/2*a^2/(a+b*x)^2)",
	NULL,
// 37
	"1/x*1/(a+b*x)",
	"-1/a*log((a+b*x)/x)",
	NULL,
// 38
	"1/x*1/(a+b*x)^2",
	"1/a*1/(a+b*x)-1/a^2*log((a+b*x)/x)",
	NULL,
// 39
	"1/x*1/(a+b*x)^3",
	"1/a^3*(1/2*((2*a+b*x)/(a+b*x))^2+log(x/(a+b*x)))",
	NULL,
// 40
	"1/x^2*1/(a+b*x)",
	"-1/(a*x)+b/a^2*log((a+b*x)/x)",
	NULL,
// 41
	"1/x^3*1/(a+b*x)",
	"(2*b*x-a)/(2*a^2*x^2)+b^2/a^3*log(x/(a+b*x))",
	NULL,
// 42
	"1/x^2*1/(a+b*x)^2",
	"-(a+2*b*x)/(a^2*x*(a+b*x))+2*b/a^3*log((a+b*x)/x)",
	NULL,
// 60
	"1/(a+b*x^2)",
	"1/sqrt(a*b)*arctan(x*sqrt(a*b)/a)",
	"a*b>0",
	NULL,
// 61
	"1/(a+b*x^2)",
	"1/(2*sqrt(-a*b))*log((a+x*sqrt(-a*b))/(a-x*sqrt(-a*b)))",
	"a*b<0",
	NULL,
// 62 is the same as 60
// 63
	"x/(a+b*x^2)",
	"1/2*1/b*log(a+b*x^2)",
	NULL,
//64
	"x^2/(a+b*x^2)",
	"x/b-a/b*integral(1/(a+b*x^2),x)",
	NULL,
//65
	"1/(a+b*x^2)^2",
	"x/(2*a*(a+b*x^2))+1/2*1/a*integral(1/(a+b*x^2),x)",
	NULL,
//66 is covered by 61
//70
	"1/x*1/(a+b*x^2)",
	"1/2*1/a*log(x^2/(a+b*x^2))",
	NULL,
//71
	"1/x^2*1/(a+b*x^2)",
	"-1/(a*x)-b/a*integral(1/(a+b*x^2),x)",
	NULL,
//74
	"1/(a+b*x^3)",
	"1/3*1/a*(a/b)^(1/3)*(1/2*log(((a/b)^(1/3)+x)^3/(a+b*x^3))+sqrt(3)*arctan((2*x-(a/b)^(1/3))*(a/b)^(-1/3)/sqrt(3)))",
	NULL,
//76
	"x^2/(a+b*x^3)",
	"1/3*1/b*log(a+b*x^3)",
	NULL,
//77
	"1/(a+b*x^4)",
	"1/2*1/a*(a/b/4)^(1/4)*(1/2*log((x^2+2*(a/b/4)^(1/4)*x+2*(a/b/4)^(1/2))/(x^2-2*(a/b/4)^(1/4)*x+2*(a/b/4)^(1/2)))+arctan(2*(a/b/4)^(1/4)*x/(2*(a/b/4)^(1/2)-x^2)))",
	"a*b>0",
	NULL,
//78
	"1/(a+b*x^4)",
	"1/2*(-a/b)^(1/4)/a*(1/2*log((x+(-a/b)^(1/4))/(x-(-a/b)^(1/4)))+arctan(x*(-a/b)^(-1/4)))",
	"a*b<0",
	NULL,
//79
	"x/(a+b*x^4)",
	"1/2*sqrt(b/a)/b*arctan(x^2*sqrt(b/a))",
	"a*b>0",
	NULL,
//80
	"x/(a+b*x^4)",
	"1/4*sqrt(-b/a)/b*log((x^2-sqrt(-a/b))/(x^2+sqrt(-a/b)))",
	"a*b<0",
	NULL,
//81
	"x^2/(a+b*x^4)",
	"1/4*1/b*(a/b/4)^(-1/4)*(1/2*log((x^2-2*(a/b/4)^(1/4)*x+2*sqrt(a/b/4))/(x^2+2*(a/b/4)^(1/4)*x+2*sqrt(a/b/4)))+arctan(2*(a/b/4)^(1/4)*x/(2*sqrt(a/b/4)-x^2)))",
	"a*b>0",
	NULL,
//82
	"x^2/(a+b*x^4)",
	"1/4*1/b*(-a/b)^(-1/4)*(log((x-(-a/b)^(1/4))/(x+(-a/b)^(1/4)))+2*arctan(x*(-a/b)^(-1/4)))",
	"a*b<0",
	NULL,
//83
	"x^3/(a+b*x^4)",
	"1/4*1/b*log(a+b*x^4)",
	NULL,
//124
	"sqrt(a+b*x)",
	"2/3*1/b*sqrt((a+b*x)^3)",
	NULL,
//125
	"x*sqrt(a+b*x)",
	"-2*(2*a-3*b*x)*sqrt((a+b*x)^3)/15/b^2",
	NULL,
//126
	"x^2*sqrt(a+b*x)",
	"2*(8*a^2-12*a*b*x+15*b^2*x^2)*sqrt((a+b*x)^3)/105/b^3",
	NULL,
//128
	"sqrt(a+b*x)/x",
	"2*sqrt(a+b*x)+a*integral(1/x*1/sqrt(a+b*x),x)",
	NULL,
//129
	"sqrt(a+b*x)/x^2",
	"-sqrt(a+b*x)/x+b/2*integral(1/x*1/sqrt(a+b*x),x)",
	NULL,
//131
	"1/sqrt(a+b*x)",
	"2*sqrt(a+b*x)/b",
	NULL,
//132
	"x/sqrt(a+b*x)",
	"-2/3*(2*a-b*x)*sqrt(a+b*x)/b^2",
	NULL,
//133
	"x^2/sqrt(a+b*x)",
	"2/15*(8*a^2-4*a*b*x+3*b^2*x^2)*sqrt(a+b*x)/b^3",
	NULL,
//135
	"1/x*1/sqrt(a+b*x)",
	"1/sqrt(a)*log((sqrt(a+b*x)-sqrt(a))/(sqrt(a+b*x)+sqrt(a)))",
	"a>0",
	NULL,
//136
	"1/x*1/sqrt(a+b*x)",
	"2/sqrt(-a)*arctan(sqrt(-(a+b*x)/a))",
	"a<0",
	NULL,
//137
	"1/x^2*1/sqrt(a+b*x)",
	"-sqrt(a+b*x)/a/x-1/2*b/a*integral(1/x*1/sqrt(a+b*x),x)",
	NULL,
//156
	"sqrt(x^2+a)",
	"1/2*(x*sqrt(x^2+a)+a*log(x+sqrt(x^2+a)))",
	NULL,
//157
	"1/sqrt(x^2+a)",
	"log(x+sqrt(x^2+a))",
	NULL,
//158
	"1/x*1/sqrt(x^2+a)",
	"arcsec(x/sqrt(-a))/sqrt(-a)",
	"a<0",
	NULL,
//159
	"1/x*1/sqrt(x^2+a)",
	"-1/sqrt(a)*log((sqrt(a)+sqrt(x^2+a))/x)",
	"a>0",
	NULL,
//160
	"sqrt(x^2+a)/x",
	"sqrt(x^2+a)-sqrt(a)*log((sqrt(a)+sqrt(x^2+a))/x)",
	"a>0",
	NULL,
//161
	"sqrt(x^2+a)/x",
	"sqrt(x^2+a)-sqrt(-a)*arcsec(x/sqrt(-a))",
	"a<0",
	NULL,
//162
	"x/sqrt(x^2+a)",
	"sqrt(x^2+a)",
	NULL,
//163
	"x*sqrt(x^2+a)",
	"1/3*sqrt((x^2+a)^3)",
	NULL,
//164 need an unexpanded version?
	"sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"1/4*(x*sqrt((x^2+a^(1/3))^3)+3/2*a^(1/3)*x*sqrt(x^2+a^(1/3))+3/2*a^(2/3)*log(x+sqrt(x^2+a^(1/3))))",
	NULL,
	// match doesn't work for the following
	"sqrt(-a+x^6-3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"1/4*(x*sqrt((x^2-a^(1/3))^3)-3/2*a^(1/3)*x*sqrt(x^2-a^(1/3))+3/2*a^(2/3)*log(x+sqrt(x^2-a^(1/3))))",
	NULL,
//165
	"1/sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"x/a^(1/3)/sqrt(x^2+a^(1/3))",
	NULL,
//166
	"x/sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"-1/sqrt(x^2+a^(1/3))",
	NULL,
//167
	"x*sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"1/5*sqrt((x^2+a^(1/3))^5)",
	NULL,
//168
	"x^2*sqrt(x^2+a)",
	"1/4*x*sqrt((x^2+a)^3)-1/8*a*x*sqrt(x^2+a)-1/8*a^2*log(x+sqrt(x^2+a))",
	NULL,
//169
	"x^3*sqrt(x^2+a)",
	"(1/5*x^2-2/15*a)*sqrt((x^2+a)^3)",
	NULL,
//170 like 169?
//171
	"x^2/sqrt(x^2+a)",
	"1/2*x*sqrt(x^2+a)-1/2*a*log(x+sqrt(x^2+a))",
	NULL,
//172
	"x^3/sqrt(x^2+a)",
	"1/3*sqrt((x^2+a)^3)-a*sqrt(x^2+a)",
	NULL,
//173
	"1/x^2*1/sqrt(x^2+a)",
	"-sqrt(x^2+a)/a/x",
	NULL,
//174
	"1/x^3*1/sqrt(x^2+a)",
	"-1/2*sqrt(x^2+a)/a/x^2+1/2*log((sqrt(a)+sqrt(x^2+a))/x)/a^(3/2)",
	"a>0",
	NULL,
//175
	"1/x^3*1/sqrt(x^2-a)",
	"1/2*sqrt(x^2-a)/a/x^2+1/2*1/(a^(3/2))*arcsec(x/(a^(1/2)))",
	"a>0",
	NULL,
//176+
	"x^2*sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"1/6*x*sqrt((x^2+a^(1/3))^5)"
	"-1/24*a^(1/3)*x*sqrt((x^2+a^(1/3))^3)"
	"-1/16*a^(2/3)*x*sqrt(x^2+a^(1/3))"
	"-1/16*a*log(x+sqrt(x^2+a^(1/3)))",
	"a>0",
	NULL,
//176-
	"x^2*sqrt(-a-3*a^(1/3)*x^4+3*a^(2/3)*x^2+x^6)",
	"1/6*x*sqrt((x^2-a^(1/3))^5)"
	"+1/24*a^(1/3)*x*sqrt((x^2-a^(1/3))^3)"
	"-1/16*a^(2/3)*x*sqrt(x^2-a^(1/3))"
	"+1/16*a*log(x+sqrt(x^2-a^(1/3)))",
	"a>0",
	NULL,
//177+
	"x^3*sqrt(a+x^6+3*a^(1/3)*x^4+3*a^(2/3)*x^2)",
	"1/7*sqrt((x^2+a^(1/3))^7)-1/5*a^(1/3)*sqrt((x^2+a^(1/3))^5)",
	"a>0",
	NULL,
//177-
	"x^3*sqrt(-a-3*a^(1/3)*x^4+3*a^(2/3)*x^2+x^6)",
	"1/7*sqrt((x^2-a^(1/3))^7)+1/5*a^(1/3)*sqrt((x^2-a^(1/3))^5)",
	"a>0",
	NULL,
//200+
	"sqrt(a-x^2)",
	"1/2*(x*sqrt(a-x^2)+a*arcsin(x/sqrt(abs(a))))",
	NULL,
//201		(seems to be handled somewhere else)
//202
	"1/x*1/sqrt(a-x^2)",
	"-1/sqrt(a)*log((sqrt(a)+sqrt(a-x^2))/x)",
	"a>0",
	NULL,
//203
	"sqrt(a-x^2)/x",
	"sqrt(a-x^2)-sqrt(a)*log((sqrt(a)+sqrt(a-x^2))/x)",
	"a>0",
	NULL,
//204
	"x/sqrt(a-x^2)",
	"-sqrt(a-x^2)",
	NULL,
//205
	"x*sqrt(a-x^2)",
	"-1/3*sqrt((a-x^2)^3)",
	NULL,
//210
	"x^2*sqrt(a-x^2)",
	"-x/4*sqrt((a-x^2)^3)+1/8*a*(x*sqrt(a-x^2)+a*arcsin(x/sqrt(a)))",
	"a>0",
	NULL,
//211
	"x^3*sqrt(a-x^2)",
	"(-1/5*x^2-2/15*a)*sqrt((a-x^2)^3)",
	"a>0",
	NULL,
//214
	"x^2/sqrt(a-x^2)",
	"-x/2*sqrt(a-x^2)+a/2*arcsin(x/sqrt(a))",
	"a>0",
	NULL,
//215
	"1/x^2*1/sqrt(a-x^2)",
	"-sqrt(a-x^2)/a/x",
	"a>0",
	NULL,
//216
	"sqrt(a-x^2)/x^2",
	"-sqrt(a-x^2)/x-arcsin(x/sqrt(a))",
	"a>0",
	NULL,
//217
	"sqrt(a-x^2)/x^3",
	"-1/2*sqrt(a-x^2)/x^2+1/2*log((sqrt(a)+sqrt(a-x^2))/x)/sqrt(a)",
	"a>0",
	NULL,
//218
	"sqrt(a-x^2)/x^4",
	"-1/3*sqrt((a-x^2)^3)/a/x^3",
	"a>0",
	NULL,
// 290

	"sin(a*x)",
	"-cos(a*x)/a",
	NULL,
// 291

	"cos(a*x)",
	"sin(a*x)/a",
	NULL,

// 292

	"tan(a*x)",
	"-log(cos(a*x))/a",
	NULL,

// 293

	"1/tan(a*x)",
	"log(sin(a*x))/a",
	NULL,

// 294

	"1/cos(a*x)",
	"log(tan(pi/4+a*x/2))/a",
	NULL,

// 295

	"1/sin(a*x)",
	"log(tan(a*x/2))/a",
	NULL,

// 296

	"sin(a*x)^2",
	"x/2-sin(2*a*x)/(4*a)",
	NULL,

// 297

	"sin(a*x)^3",
	"-cos(a*x)*(sin(a*x)^2+2)/(3*a)",
	NULL,

// 298

	"sin(a*x)^4",
	"3/8*x-sin(2*a*x)/(4*a)+sin(4*a*x)/(32*a)",
	NULL,

// 302

	"cos(a*x)^2",
	"x/2+sin(2*a*x)/(4*a)",
	NULL,

// 303

	"cos(a*x)^3",
	"sin(a*x)*(cos(a*x)^2+2)/(3*a)",
	NULL,

// 304

	"cos(a*x)^4",
	"3/8*x+sin(2*a*x)/(4*a)+sin(4*a*x)/(32*a)",
	NULL,

// 308

	"1/sin(a*x)^2",
	"-1/(a*tan(a*x))",
	NULL,

// 312

	"1/cos(a*x)^2",
	"tan(a*x)/a",
	NULL,

// 318
	"sin(a*x)*cos(a*x)",
	"sin(a*x)^2/(2*a)",
	NULL,

// 320

	"sin(a*x)^2*cos(a*x)^2",
	"-sin(4*a*x)/(32*a)+x/8",
	NULL,

// 326

	"sin(a*x)/cos(a*x)^2",
	"1/(a*cos(a*x))",
	NULL,

// 327

	"sin(a*x)^2/cos(a*x)",
	"(log(tan(pi/4+a*x/2))-sin(a*x))/a",
	NULL,

// 328

	"cos(a*x)/sin(a*x)^2",
	"-1/(a*sin(a*x))",
	NULL,

// 329

	"1/(sin(a*x)*cos(a*x))",
	"log(tan(a*x))/a",
	NULL,

// 330

	"1/(sin(a*x)*cos(a*x)^2)",
	"(1/cos(a*x)+log(tan(a*x/2)))/a",
	NULL,

// 331

	"1/(sin(a*x)^2*cos(a*x))",
	"(log(tan(pi/4+a*x/2))-1/sin(a*x))/a",
	NULL,

// 333

	"1/(sin(a*x)^2*cos(a*x)^2)",
	"-2/(a*tan(2*a*x))",
	NULL,

// 335

	"sin(a+b*x)",
	"-cos(a+b*x)/b",
	NULL,

// 336

	"cos(a+b*x)",
	"sin(a+b*x)/b",
	NULL,

// 337+ (with the addition of b)

	"1/(b+b*sin(a*x))",
	"-tan(pi/4-a*x/2)/a/b",
	NULL,

// 337- (with the addition of b)

	"1/(b-b*sin(a*x))",
	"tan(pi/4+a*x/2)/a/b",
	NULL,

// 338 (with the addition of b)

	"1/(b+b*cos(a*x))",
	"tan(a*x/2)/a/b",
	NULL,

// 339 (with the addition of b)

	"1/(b-b*cos(a*x))",
	"-1/tan(a*x/2)/a/b",
	NULL,

// 340

	"1/(a+b*sin(x))",
	"1/sqrt(b^2-a^2)*log((a*tan(x/2)+b-sqrt(b^2-a^2))/(a*tan(x/2)+b+sqrt(b^2-a^2)))",
	"test(a==b,0,1,1)",
	NULL,

// 341

	"1/(a+b*cos(x))",
	"1/sqrt(b^2-a^2)*log((sqrt(b^2-a^2)*tan(x/2)+a+b)/(sqrt(b^2-a^2)*tan(x/2)-a-b))",
	"test(a==b,0,1,1)",
	NULL,

// 441

	"arcsin(a*x)",
	"x*arcsin(a*x)+sqrt(1-a^2*x^2)/a",
	NULL,

// 442

	"arccos(a*x)",
	"x*arccos(a*x)+sqrt(1-a^2*x^2)/a",
	NULL,

// 443

	"arctan(a*x)",
	"x*arctan(a*x)-1/2*log(1+a^2*x^2)/a",
	NULL,

// 485 (with addition of a)

	"log(a*x)",
	"x*log(a*x)-x",
	NULL,

// 486 (with addition of a)

	"x*log(a*x)",
	"x^2*log(a*x)/2-x^2/4",
	NULL,

// 487 (with addition of a)

	"x^2*log(a*x)",
	"x^3*log(a*x)/3-1/9*x^3",
	NULL,

// 489

	"log(x)^2",
	"x*log(x)^2-2*x*log(x)+2*x",
	NULL,

// 493 (with addition of a)

	"1/(x*log(a*x))",
	"log(log(a*x))",
	NULL,

// 499

	"log(a*x+b)",
	"(a*x+b)*log(a*x+b)/a-x",
	NULL,

// 500

	"log(a*x+b)/x^2",
	"a/b*log(x)-(a*x+b)*log(a*x+b)/b/x",
	NULL,

// 554

	"sinh(x)",
	"cosh(x)",
	NULL,

// 555

	"cosh(x)",
	"sinh(x)",
	NULL,

// 556

	"tanh(x)",
	"log(cosh(x))",
	NULL,

// 560

	"x*sinh(x)",
	"x*cosh(x)-sinh(x)",
	NULL,

// 562

	"x*cosh(x)",
	"x*sinh(x)-cosh(x)",
	NULL,

// 566

	"sinh(x)^2",
	"sinh(2*x)/4-x/2",
	NULL,

// 569

	"tanh(x)^2",
	"x-tanh(x)",
	NULL,

// 572

	"cosh(x)^2",
	"sinh(2*x)/4+x/2",
	NULL,

// ?
	"x^3*exp(a*x^2)",
	"exp(a*x^2)*(x^2/a-1/(a^2))/2",
	NULL,

// ?

	"x^3*exp(a*x^2+b)",
	"exp(a*x^2)*exp(b)*(x^2/a-1/(a^2))/2",
	NULL,

	NULL,
};

// build the table, subst. meta symbols for a, b and x.

static void
scan_integrals(void)
{
	int h, i, k;

	k = tos;

	i = 0;

	while (integrals[i]) {

		h = tos;

		while (integrals[i]) {
			scan(integrals[i++]);
			push_symbol(SYMBOL_A);
			push(meta_a);
			subst();
			push_symbol(SYMBOL_B);
			push(meta_b);
			subst();
			push_symbol(SYMBOL_X);
			push(meta_x);
			subst();
		}

		list(tos - h);

		i++;
	}

	list(tos - k);

	table_of_integrals = pop();
}

static char *s[] = {

	"clear()",
	"",

	"tty=1",
	"",

	"integral(x^2+x)",
	"1/2*x^2+1/3*x^3",

	"#1",
	"integral(A,X)",
	"A*X",

	"#4",
	"integral(A+B,X)",
	"A*X+B*X",

	"#9",
	"integral(1/X,X)",
	"log(X)",

	"#11",
	"integral(exp(X),X)",
	"exp(X)",

	"#12",
	"integral(exp(A*X),X)-exp(A*X)/A",
	"0",

	"#14",
	"integral(log(X),X)-X*log(X)+X",
	"0",

	"#15",
	"integral(3^X*log(3),X)",
	"3^X",

	"#16",
	"integral(1/(3+x^2),x)-3^(-1/2)*arctan(3^(-1/2)*x)",
	"0",

	"#17",
	"integral(1/(a-x^2),x)-a^(-1/2)*arctanh(a^(-1/2)*x)",
	"0",

	"#19",
	"integral(1/sqrt(a-x^2),x)-arcsin(a^(-1/2)*x)",
	"0",

	"#20",
	"integral(1/sqrt(a+x^2),x)",
	"log(x+(a+x^2)^(1/2))",

	"#27",
	"integral(1/(a+b*x),x)",
	"log(a+b*x)/b",

	"#28",
	"integral(1/(A+B*X)^2,X)+1/B*1/(A+B*X)",
	"0",

	"#29",
	"integral(1/(a+b*x)^3,x)+1/2*1/b*(a+b*x)^(-2)",
	"0",

	"#30",
	"integral(X/(A+B*X),X)+A*B^(-2)*log(A+B*X)-X/B",
	"0",

	"#31",
	"integral(X/(A+B*X)^2,X)-1/B^2*(log(A+B*X)+A/(A+B*X))",
	"0",

	"#33",
	"integral(X^2/(A+B*X),X)-1/B^2*(1/2*(A+B*X)^2-2*A*(A+B*X)+A^2*log(A+B*X))",
	"0",

	"#34",
	"integral(X^2/(A+B*X)^2,X)-1/B^3*(A+B*X-2*A*log(A+B*X)-A^2/(A+B*X))",
	"0",

	"#35",
	"integral(X^2/(A+B*X)^3,X)-1/B^3*(log(A+B*X)+2*A/(A+B*X)-1/2*A^2/(A+B*X)^2)",
	"0",

	"#37",
	"integral(1/X*1/(A+B*X),X)+1/A*log((A+B*X)/X)",
	"0",

	"#38",
	"integral(1/X*1/(A+B*X)^2,X)-1/A*1/(A+B*X)+1/A^2*log((A+B*X)/X)",
	"0",

	"#39",
	"integral(1/X*1/(A+B*X)^3,X)-1/A^3*(1/2*((2*A+B*X)/(A+B*X))^2+log(X/(A+B*X)))",
	"0",

	"#40",
	"integral(1/X^2*1/(A+B*X),X)+1/(A*X)-B/A^2*log((A+B*X)/X)",
	"0",

	"#41",
	"integral(1/X^3*1/(A+B*X),X)-(2*B*X-A)/(2*A^2*X^2)-B^2/A^3*log(X/(A+B*X))",
	"0",

	"#42",
	"integral(1/X^2*1/(A+B*X)^2,X)+(A+2*B*X)/(A^2*X*(A+B*X))-2*B/A^3*log((A+B*X)/X)",
	"0",

	"#60",
	"integral(1/(2+3*X^2),X)-1/sqrt(6)*arctan(1/2*X*sqrt(6))",
	"0",
	"integral(1/(-2-3*X^2),X)-1/sqrt(6)*arctan(-1/2*X*sqrt(6))",
	"0",

	"#61",
	"integral(1/(2-3*X^2),X)-1/2*1/sqrt(6)*log((2+X*sqrt(6))/(2-X*sqrt(6)))",
	"0",
	"integral(1/(-2+3*X^2),X)-1/2*1/sqrt(6)*log((-2+X*sqrt(6))/(-2-X*sqrt(6)))",
	"0",

	"#63",
	"integral(X/(A+B*X^2),X)-1/2*1/B*log(A+B*X^2)",
	"0",

	"#64",
	"integral(X^2/(A+B*X^2),X)-X/B+A/B*integral(1/(A+B*X^2),X)",
	"0",

	"#65",
	"integral(1/(A+B*X^2)^2,X)-X/(2*A*(A+B*X^2))-1/2*1/A*integral(1/(A+B*X^2),X)",
	"0",

	"#70",
	"integral(1/X*1/(A+B*X^2),X)-1/2*1/A*log(X^2/(A+B*X^2))",
	"0",

	"#71",
	"integral(1/X^2*1/(A+B*X^2),X)+1/(A*X)+B/A*integral(1/(A+B*X^2),X)",
	"0",

	"#74",
	"integral(1/(A+B*X^3),X)-1/3*1/A*(A/B)^(1/3)*(1/2*log(((A/B)^(1/3)+X)^3/(A+B*X^3))+sqrt(3)*arctan((2*X-(A/B)^(1/3))*(A/B)^(-1/3)/sqrt(3)))",
	"0",

	"#76",
	"integral(X^2/(A+B*X^3),X)-1/3*1/B*log(A+B*X^3)",
	"0",

	"#77",
	"integral(1/(2+3*X^4),X)-1/2*1/2*(2/3/4)^(1/4)*(1/2*log((X^2+2*(2/3/4)^(1/4)*X+2*(2/3/4)^(1/2))/(X^2-2*(2/3/4)^(1/4)*X+2*(2/3/4)^(1/2)))+arctan(2*(2/3/4)^(1/4)*X/(2*(2/3/4)^(1/2)-X^2)))",
	"0",

	"#78",
	"integral(1/(2-3*X^4),X)-1/2*(2/3)^(1/4)/2*(1/2*log((X+(2/3)^(1/4))/(X-(2/3)^(1/4)))+arctan(X*(2/3)^(-1/4)))",
	"0",

	"#79",
	"integral(X/(2+3*X^4),X)-1/2*1/3*1/sqrt(2/3)*arctan(X^2/sqrt(2/3))",
	"0",

	"#80",
	"integral(X/(2-3*X^4),X)+1/4*1/3*sqrt(3/2)*log((X^2-sqrt(2/3))/(X^2+sqrt(2/3)))",
	"0",

	"#81",
	"integral(X^2/(2+3*X^4),X)-1/4*1/3*(2/3/4)^(-1/4)*(1/2*log((X^2-2*(2/3/4)^(1/4)*X+2*sqrt(2/3/4))/(X^2+2*(2/3/4)^(1/4)*X+2*sqrt(2/3/4)))+arctan(2*(2/3/4)^(1/4)*X/(2*sqrt(2/3/4)-X^2)))",
	"0",

	"#82",
	"integral(X^2/(2-3*X^4),X)+1/4*1/3*(2/3)^(-1/4)*(log((X-(2/3)^(1/4))/(X+(2/3)^(1/4)))+2*arctan(X*(2/3)^(-1/4)))",
	"0",

	"#83",
	"integral(X^3/(A+B*X^4),X)-1/4*1/B*log(A+B*X^4)",
	"0",

	"#124",
	"integral(sqrt(A+B*X),X)-2/3/B*sqrt((A+B*X)^3)",
	"0",

	"#125",
	"integral(X*sqrt(A+B*X),X)+2*(2*A-3*B*X)*sqrt((A+B*X)^3)/15*B^(-2)",
	"0",

	"#126",
	"integral(X^2*sqrt(A+B*X),X)-2*(8*A^2-12*A*B*X+15*B^2*X^2)*sqrt((A+B*X)^3)/105*B^(-3)",
	"0",

	"#128",
	"integral(sqrt(A+B*X)/X,X)-2*sqrt(A+B*X)-A*integral(1/X*1/sqrt(A+B*X),X)",
	"0",

	"#129",
	"integral(sqrt(A+B*X)/X^2,X)+sqrt(A+B*X)/X-B/2*integral(1/X*1/sqrt(A+B*X),X)",
	"0",

	"#131",
	"integral(1/sqrt(A+B*X),X)-2*sqrt(A+B*X)/B",
	"0",

	"#132",
	"integral(X/sqrt(A+B*X),X)+2/3*(2*A-B*X)*sqrt(A+B*X)/B^2",
	"0",

	"#133",
	"integral(X^2/sqrt(A+B*X),X)-2/15*(8*A^2-4*A*B*X+3*B^2*X^2)*sqrt(A+B*X)/B^3",
	"0",

	"#134",
	"integral(1/X*1/sqrt(2+B*X),X)-1/sqrt(2)*log((sqrt(2+B*X)-sqrt(2))/(sqrt(2+B*X)+sqrt(2)))",
	"0",

	"#136",
	"integral(1/X*1/sqrt(-2+B*X),X)-2/sqrt(2)*arctan(sqrt((-2+B*X)/2))",
	"0",

	"#137",
	"integral(1/X^2*1/sqrt(A+B*X),X)+sqrt(A+B*X)/A/X+1/2*B/A*integral(1/X*1/sqrt(A+B*X),X)",
	"0",

	"#156",
	"integral(sqrt(X^2+A),X)-1/2*(X*sqrt(X^2+A)+A*log(X+sqrt(X^2+A)))",
	"0",

	"#157",
	"integral(1/sqrt(X^2+A),X)-log(X+sqrt(X^2+A))",
	"0",

	"#158",
	"integral(1/X*1/sqrt(X^2-2),X)-arcsec(X/sqrt(2))/sqrt(2)",
	"0",

	"#159",
	"integral(1/X*1/sqrt(X^2+2),X)+1/sqrt(2)*log((sqrt(2)+sqrt(X^2+2))/X)",
	"0",

	"#160",
	"integral(sqrt(X^2+2)/X,X)-sqrt(X^2+2)+sqrt(2)*log((sqrt(2)+sqrt(X^2+2))/X)",
	"0",

	"#161",
	"integral(sqrt(X^2-2)/X,X)-sqrt(X^2-2)+sqrt(2)*arcsec(X/sqrt(2))",
	"0",

	"#162",
	"integral(X/sqrt(X^2+A),X)-sqrt(X^2+A)",
	"0",

	"#163",
	"integral(X*sqrt(X^2+A),X)-1/3*sqrt((X^2+A)^3)",
	"0",

	"#164",
	"integral(sqrt((X^2+A)^3),X)-1/4*(X*sqrt((X^2+A)^3)+3/2*A*X*sqrt(X^2+A)+3/2*A^2*log(X+sqrt(X^2+A)))",
	"0",
//	"integral(sqrt((X^2-A)^3),X)-1/4*(X*sqrt((X^2-A)^3)-3/2*A*X*sqrt(X^2-A)+3/2*A^2*log(X+sqrt(X^2-A)))",
//	"0",

	"#165",
	"integral(1/sqrt((X^2+A)^3),X)-X/A/sqrt(X^2+A)",
	"0",

	"#166",
	"integral(X/sqrt((X^2+A)^3),X)+1/sqrt(X^2+A)",
	"0",

	"#167",
	"integral(X*sqrt((X^2+A)^3),X)-1/5*sqrt((X^2+A)^5)",
	"0",

	"#168",
	"integral(X^2*sqrt(X^2+A),X)-1/4*X*sqrt((X^2+A)^3)+1/8*A*X*sqrt(X^2+A)+1/8*A^2*log(X+sqrt(X^2+A))",
	"0",

	"#169",
	"integral(X^3*sqrt(X^2+A),X)-(1/5*X^2-2/15*A)*sqrt((X^2+A)^3)",
	"0",

	"#171",
	"integral(X^2/sqrt(X^2+A),X)-1/2*X*sqrt(X^2+A)+1/2*A*log(X+sqrt(X^2+A))",
	"0",

	"#172",
	"integral(X^3/sqrt(X^2+A),X)-1/3*sqrt((X^2+A)^3)+A*sqrt(X^2+A)",
	"0",

	"#173",
	"integral(1/X^2*1/sqrt(X^2+A),X)+sqrt(X^2+A)/A/X",
	"0",

	"#174",
	"integral(1/X^3*1/sqrt(X^2+2),X)+1/2*sqrt(X^2+2)/2/X^2-1/2*log((sqrt(2)+sqrt(X^2+2))/X)/(sqrt(2)^3)",
	"0",

	"#175",
	"integral(1/X^3*1/sqrt(X^2-2),X)-1/2*sqrt(X^2-2)/2/X^2-1/2*1/(2^(3/2))*arcsec(X/(2^(1/2)))",
	"0",

	"#176+",
	"integral(X^2*sqrt((X^2+2^2)^3),X)"
	"-1/6*X*sqrt((X^2+2^2)^5)"
	"+1/24*(2^2)*X*sqrt((X^2+2^2)^3)"
	"+1/16*(2^4)X*sqrt(X^2+2^2)"
	"+1/16*(2^6)*log(X+sqrt(X^2+2^2))",
	"0",

	"#176-",
	"integral(X^2*sqrt((X^2-2^2)^3),X)"
	"-1/6*X*sqrt((X^2-2^2)^5)"
	"-1/24*(2^2)*X*sqrt((X^2-2^2)^3)"
	"+1/16*(2^4)X*sqrt(X^2-2^2)"
	"-1/16*(2^6)*log(X+sqrt(X^2-2^2))",
	"0",

	"#177+",
	"integral(X^3*sqrt((X^2+7^2)^3),X)"
	"-1/7*sqrt((X^2+7^2)^7)"
	"+1/5*(7^2)*sqrt((X^2+7^2)^5)",
	"0",

	"#177-",
	"integral(X^3*sqrt((X^2-7^2)^3),X)"
	"-1/7*sqrt((X^2-7^2)^7)"
	"-1/5*(7^2)*sqrt((X^2-7^2)^5)",
	"0",

	"#200",
	"integral(sqrt(7-X^2),X)-1/2*(X*sqrt(7-X^2)+7*arcsin(X/sqrt(7)))",
	"0",

	"#201",
	"integral(1/sqrt(7-X^2),X)-arcsin(X/sqrt(7))",
	"0",

	"#202",
	"integral(1/X*1/sqrt(7-X^2),X)+1/sqrt(7)*log((sqrt(7)+sqrt(7-X^2))/X)",
	"0",

	"#203",
	"integral(sqrt(7-X^2)/X,X)"
	"-sqrt(7-X^2)+sqrt(7)*log((sqrt(7)+sqrt(7-X^2))/X)",
	"0",

	"#204",
	"integral(X/sqrt(A-X^2),X)"
	"+sqrt(A-X^2)",
	"0",

	"#205",
	"integral(X*sqrt(A-X^2),X)"
	"+1/3*sqrt((A-X^2)^3)",
	"0",

	"#210",
	"integral(X^2*sqrt(7-X^2),X)"
	"+1/4*X*sqrt((7-X^2)^3)"
	"-7/8*(X*sqrt(7-X^2)+7*arcsin(X/sqrt(7)))",
	"0",

	"#211",
	"integral(X^3*sqrt(7-X^2),X)"
	"-(-1/5*X^2-2/15*7)*sqrt((7-X^2)^3)",
	"0",

	"#214",
	"integral(X^2/sqrt(7-X^2),X)"
	"+X/2*sqrt(7-X^2)"
	"-7/2*arcsin(X/sqrt(7))",
	"0",

	"#215",
	"integral(1/X^2*1/sqrt(7-X^2),X)"
	"+sqrt(7-X^2)/7/X",
	"0",

	"#216",
	"integral(sqrt(7-X^2)/X^2,X)"
	"+sqrt(7-X^2)/X"
	"+arcsin(X/sqrt(7))",
	"0",

	"#217",
	"integral(sqrt(7-X^2)/X^3,X)"
	"+1/2*sqrt(7-X^2)/X^2"
	"-1/2*log((sqrt(7)+sqrt(7-X^2))/X)/sqrt(7)",
	"0",

	"#218",
	"integral(sqrt(7-X^2)/X^4,X)"
	"+1/3*sqrt((7-X^2)^3)/7/X^3",
	"0",

	"#290",
	"integral(sin(A*X),X)+cos(A*X)/A",
	"0",

	"#291",
	"integral(cos(A*X),X)-sin(A*X)/A",
	"0",

	"#292",
	"integral(tan(A*X),X)+log(cos(A*X))/A",
	"0",

	"#293",
	"integral(1/tan(A*X),X)-log(sin(A*X))/A",
	"0",

	"#294",
	"integral(1/cos(A*X),X)-log(tan(pi/4+A*X/2))/A",
	"0",

	"#295",
	"integral(1/sin(A*X),X)-log(tan(A*X/2))/A",
	"0",

	"#296",
	"integral(sin(A*X)^2,X)-X/2+sin(2*A*X)/(4*A)",
	"0",

	"#297",
	"integral(sin(A*X)^3,X)+cos(A*X)*(sin(A*X)^2+2)/(3*A)",
	"0",

	"#298",
	"integral(sin(A*X)^4,X)-3/8*X+sin(2*A*X)/(4*A)-sin(4*A*X)/(32*A)",
	"0",

	"#302",
	"integral(cos(A*X)^2,X)-X/2-sin(2*A*X)/(4*A)",
	"0",

	"#303",
	"integral(cos(A*X)^3,X)-sin(A*X)*(cos(A*X)^2+2)/(3*A)",
	"0",

	"#304",
	"integral(cos(A*X)^4,X)-3/8*X-sin(2*A*X)/(4*A)-sin(4*A*X)/(32*A)",
	"0",

	"#308",
	"integral((1/sin(A*X))^2,X)+1/A*1/tan(A*X)",
	"0",

	"#312",
	"integral((1/cos(A*X))^2,X)-tan(A*X)/A",
	"0",

	"#318",
	"integral(sin(A*X)*cos(A*X),X)-sin(A*X)^2/(2*A)",
	"0",

	"#320",
	"integral(sin(A*X)^2*cos(A*X)^2,X)+sin(4*A*X)/(32*A)-X/8",
	"0",

	"#326",
	"integral(sin(A*X)/cos(A*X)/cos(A*X),X)-1/(A*cos(A*X))",
	"0",

	"#327",
	"integral(sin(A*X)^2/cos(A*X),X)+sin(A*X)/A-log(tan(pi/4+A*X/2))/A",
	"0",

	"#328",
	"integral(cos(A*X)/sin(A*X)^2,X)+1/(A*sin(A*X))",
	"0",

	"#329",
	"integral(1/sin(A*X)/cos(A*X),X)-log(tan(A*X))/A",
	"0",

	"#330",
	"integral(1/sin(A*X)/cos(A*X)^2,X)-(1/cos(A*X)+log(tan(A*X/2)))/A",
	"0",

	"#332",
	"integral(1/sin(A*X)^2/cos(A*X),X)-(log(tan(pi/4+A*X/2))-1/sin(A*X))/A",
	"0",

	"#333",
	"integral(1/sin(A*X)^2/cos(A*X)^2,X)+2/(A*tan(2*A*X))",
	"0",

	"#335",
	"integral(sin(A+B*X),X)+cos(A+B*X)/B",
	"0",

	"#336",
	"integral(cos(A+B*X),X)-sin(A+B*X)/B",
	"0",

	"#337+",
	"integral(1/(1+sin(A*X)),X)+tan(pi/4-A*X/2)/A",
	"0",

	"#337b+",
	"integral(1/(B+B*sin(A*X)),X)+tan(pi/4-A*X/2)/A/B",
	"0",

	"#337-",
	"integral(1/(1-sin(A*X)),X)-tan(pi/4+A*X/2)/A",
	"0",

	"#337b-",
	"integral(1/(B-B*sin(A*X)),X)-tan(pi/4+A*X/2)/A/B",
	"0",

	"#338",
	"integral(1/(1+cos(A*X)),X)-tan(A*X/2)/A",
	"0",

	"#339",
	"integral(1/(1-cos(A*X)),X)+1/(A*tan(A*X/2))",
	"0",

	"#340",
	"integral(1/(A+B*sin(X)),X)-1/sqrt(B^2-A^2)*log((A*tan(X/2)+B-sqrt(B^2-A^2))/(A*tan(X/2)+B+sqrt(B^2-A^2)))",
	"0",

	"#341",
	"integral(1/(A+B*cos(X)),X)-1/sqrt(B^2-A^2)*log((sqrt(B^2-A^2)*tan(X/2)+A+B)/(sqrt(B^2-A^2)*tan(X/2)-A-B))",
	"0",

	"#441",
	"integral(arcsin(A*X),X)-X*arcsin(A*X)-sqrt(1-A^2*X^2)/A",
	"0",

	"#442",
	"integral(arccos(A*X),X)-X*arccos(A*X)-sqrt(1-A^2*X^2)/A",
	"0",

	"#443",
	"integral(arctan(A*X),X)-X*arctan(A*X)+log(1+A^2*X^2)/(2*A)",
	"0",

	"#485",
	"integral(log(X),X)-X*log(X)+X",
	"0",

	"#485a",
	"integral(log(A*X),X)-X*log(A*X)+X",
	"0",

	"#486",
	"integral(X*log(X),X)-1/2*X^2*log(X)+1/4*X^2",
	"0",

	"#486a",
	"integral(X*log(A*X),X)-1/2*X^2*log(A*X)+1/4*X^2",
	"0",

	"#487",
	"integral(X^2*log(A*X),X)-1/3*X^3*log(A*X)+X^3/9",
	"0",

	"#489",
	"integral(log(X)^2,X)-X*log(X)^2+2*X*log(X)-2*X",
	"0",

	"#493",
	"integral(1/X*1/log(A*X),X)-log(log(A*X))",
	"0",

	"#499",
	"integral(log(A*X+B),X)-(A*X+B)*log(A*X+B)/A+X",
	"0",

	"#500",
	"integral(log(A*X+B)/X^2,X)-A*log(X)/B+(A*X+B)*log(A*X+B)/B/X",
	"0",

	"#554",
	"integral(sinh(X),X)-cosh(X)",
	"0",

	"#555",
	"integral(cosh(X),X)-sinh(X)",
	"0",

	"#556",
	"integral(tanh(X),X)-log(cosh(X))",
	"0",

	"#560",
	"integral(X*sinh(X),X)-X*cosh(X)+sinh(X)",
	"0",

	"#562",
	"integral(X*cosh(X),X)-X*sinh(X)+cosh(X)",
	"0",

	"#566",
	"integral(sinh(X)^2,X)-sinh(2*X)/4+X/2",
	"0",

	"#569",
	"integral(tanh(X)^2,X)-X+tanh(X)",
	"0",

	"#572",
	"integral(cosh(X)^2,X)-sinh(2*X)/4-X/2",
	"0",
};

void
test_integral(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
