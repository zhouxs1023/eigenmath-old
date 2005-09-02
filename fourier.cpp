//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
// 
//	Fourier transform :
//	The formalism used is : integral(f(t)*exp(-i*t*x)) dt
//	The following properties are implemented :
//	- linearity :fourier(a*f(t)+b*g(t),t)=a*fourier(f(t),t)+b*fourier(g(t),t)
//	- derivation : fourier(d(f(t),t),t)=i*t*fourier(f(t),t)
//	- product by t : fourier(t*f(t),t)=-i*d(fourier(f(t),t),t)
//	- integral : fourier(integral(f(t),t),t)=fourier(f(t),t)/(i*t)+pi*fourier(f(t),t)*dirac(t)
//	- division by t : fourier(f(t)/t,t)=integral(fourier(f(t),t),t)/i
//	- product of convolution : fourier(convolution(f(t),g(t)),t)=fourier(f(t),t)*fourier(g(t),t)
//	- fourier of fourier : fourier(fourier(f(t),t),t)=2*pi*f(-t)
//
//	The formalism used is : integral(f(t)*exp(-i*t*x)) dt
//	N.B.: the is no change of variable : fourier(x,x)=2*i*pi*d(dirac(x),x)
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

static void yfourier(void);
static void fourier_of_sum(void);
static void fourier_of_product(void);
static void fourier_of_convolution(void);
static void fourier_of_derivative1(void);
static void fourier_of_derivative2(void);
static void fourier_of_integral1(void);
static void fourier_of_integral2(void);
static void fourier_of_fourier(void);
static void fourier_of_invfourier(void);
static void fourier_of_form(void);
static int match(U *, U *, U *, int, int);
static void scan_fouriers(void);

void
eval_fourier(void)
{
	push(cadr(p1));
	eval();
	if (caddr(p1) == nil)
		guess();
	else {
		push(caddr(p1));
	}
	fourier();
}

void
fourier(void)
{
	save();
	yfourier();
	restore();
}

static void
yfourier(void)
{
	if (table_of_fourier == nil)
		scan_fouriers();

	p2 = pop();
	p1 = pop();
	
	push(caddr(p1));
	negate();
	p3 = pop();
	
	if (car(p1) == symbol(ADD))
		fourier_of_sum();
	else if (car(p1) == symbol(MULTIPLY))
		fourier_of_product();
	else if (car(p1) == symbol(CONVOLUTION))
		fourier_of_convolution();
	else if (car(p1) == symbol(DERIVATIVE)){
		if (caddr(p1) == p2)
			fourier_of_derivative1();
		else
			fourier_of_derivative2();}
	else if (car(p1) == symbol(INTEGRAL)){
		if (caddr(p1) == p2)
			fourier_of_integral1();
		else
			fourier_of_integral2();}
	else if (car(p1) == symbol(FOURIER) && (caddr(p1) == p2 || p3 == p2))
		fourier_of_fourier();
	else if (car(p1) == symbol(INVFOURIER) && caddr(p1) == p2)
		fourier_of_invfourier();
	else
		fourier_of_form();
}

static void
fourier_of_form(void)
{
	int h;

	// save meta symbols

	push(meta_a->u.sym.binding);
	push(meta_b->u.sym.binding);
	push(meta_c->u.sym.binding);
	push(meta_n->u.sym.binding);
	push(meta_x->u.sym.binding);

	meta_x->u.sym.binding = p2;

	h = tos;

	push(one);

	push(p1);
	push(p2);

	distill();

	p3 = table_of_fourier;

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
		push_symbol(FOURIER);
		push(p1);
		push(p2);
		list(3);
	}

	p3 = pop();

	tos = h;

	// restore meta symbols

	meta_x->u.sym.binding = pop();
	meta_n->u.sym.binding = pop();
	meta_c->u.sym.binding = pop();
	meta_b->u.sym.binding = pop();
	meta_a->u.sym.binding = pop();

	push(p3);
}

static void
fourier_of_sum(void)
{
	p1 = cdr(p1);
	push(car(p1));
	push(p2);
	fourier();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		fourier();
		add();
		p1 = cdr(p1);
	}
}
	
static void
fourier_of_derivative1(void)
{
	p1 = cdr(p1);
	push(imaginaryunit);
	push(p2);
	multiply();
	push(car(p1));
	push(p2);
	fourier();
	multiply();
}

static void
fourier_of_derivative2(void)
{
	push(cadr(p1));
	push(p2);
	fourier();
	push(caddr(p1));
	derivative();
}

static void
fourier_of_integral1(void)
{
	p1 = cdr(p1);
	push(car(p1));
	push(p2);
	fourier();
	push(imaginaryunit);
	push(p2);
	multiply();
	divide();
// code below originally commented out, why?
	push_symbol(PI);
	push(car(p1));
	push(p2);
	fourier();
	multiply();
	push(p2);
	dirac();
	multiply();
	add();
// end of commented out
}

static void
fourier_of_integral2(void)
{
	push(cadr(p1));
	push(p2);
	fourier();
	push(caddr(p1));
	integral();
}

static void
fourier_of_fourier(void)
{
	p1 = cdr(p1);
	push_symbol(PI);
	push_integer(2);
	multiply();
	push(car(p1));
	push(p2);
	push(p2);
	negate();
	subst();
	multiply();
}

static void
fourier_of_invfourier(void)
{
	p1 = cdr(p1);
	push(car(p1));
}


static void
fourier_of_convolution(void)
{	p1 = cdr(p1);
	push(car(p1));
	push(p2);
	fourier();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		fourier();
		multiply();
		p1 = cdr(p1);
	}

}

static void
fourier_of_product(void)
{
	int h1, h2, n;

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

	if (tos - h2 == 0){
		push_integer(2);
		push_symbol(PI);
		multiply();
		push(p2);
		dirac();
		multiply();
		}
	else {
		multiply_all(tos - h2);
		p1 = pop();
		if (car(p1) == symbol(MULTIPLY) && cadr(p1)==p2){
			push(caddr(p1));
			push(p2);
			fourier();
			push(p2);
			derivative();
			push(imaginaryunit);
			multiply();}
		else
			if (car(p1) == symbol(MULTIPLY) && caaddr(p1) == symbol(POWER) && 
			cadaddr(p1) == p2 && isinteger(caddaddr(p1))&& !isnegativenumber(caddaddr(p1))){
				push(caddaddr(p1));
				n = pop_integer();
				push(imaginaryunit);
				push_integer(n);
				power();
				push(cadr(p1));
				push(p2);
				fourier();
				while(n > 0){
				push(p2);
				derivative();
				n=n-1;}
				push(imaginaryunit);
				push_integer(n);
				power();
				multiply();
				}
		else
			if (car(p1) == symbol(MULTIPLY)&& caadr(p1) == symbol(POWER)
				&& cadadr(p1) == p2 && isinteger(caddadr(p1)) && !isnegativenumber(caddadr(p1))){
				push(caddadr(p1));
				n = pop_integer();
				push(imaginaryunit);
				push_integer(n);
				power();
				push(caddr(p1));
				push(p2);
				fourier();
				while(n > 0){
				push(p2);
				derivative();
				n=n-1;}
				multiply();
				}
		else
			if (car(p1) == symbol(MULTIPLY) && caaddr(p1) == symbol(POWER) && 
				cadaddr(p1) == p2 && isinteger(caddaddr(p1))&& isnegativenumber(caddaddr(p1))){
				push(caddaddr(p1));
				n = pop_integer();
				push(imaginaryunit);
				push_integer(-1);
				multiply();
				push_integer(n);
				power();
				push(cadr(p1));
				push(p2);
				fourier();
				while(n > 0){
				push(p2);
				integral();
				n=n-1;}
				push(imaginaryunit);
				push_integer(n);
				power();
				multiply();
				}
		else
			if (car(p1) == symbol(MULTIPLY)&& caadr(p1) == symbol(POWER)
				&& cadadr(p1) == p2 && isinteger(caddadr(p1)) && isnegativenumber(caddadr(p1))){
				push(caddadr(p1));
				negate();
				n = pop_integer();
				push(imaginaryunit);
				push_integer(-1);
				multiply();
				push_integer(n);
				power();
				push(caddr(p1));
				push(p2);
				fourier();
				while(n > 0){
				push(p2);
				integral();
				n=n-1;}
				multiply();
				}
		else
			fourier_of_form();
	}

	multiply_all(tos - h1);
}


static int
match(U *actual, U *formal, U *caveats, int h1, int h2)
{
	int i, j, k, l;

	save();

	for (i = h1; i < h2; i++) {
		for (j = h1; j < h2; j++) {
			for (k = h1; k < h2; k++) {
				for (l = h1; l < h2; l++) {
						meta_a->u.sym.binding = stack[i];
						meta_b->u.sym.binding = stack[j];
						meta_c->u.sym.binding = stack[k];
						meta_n->u.sym.binding = stack[l];

						// check caveats	

						p1 = caveats;

						while (iscons(p1)) {
						push(car(p1));
						eval();
						p2 = pop();
						if (p2 != symbol(YYTRUE))
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
		}
	}

	restore();

	return 0;
}



char *fouriers[] = {

	"a",
	"2*pi*a*dirac(x)",
	NULL,
	
	"besselj(a*x,0)",
	"2/(abs(a)*sqrt(1-(x/a)^2))",
	NULL,
	
	"besselj(a*x+b,0)",
	"2*exp(i*b*x/a)/(abs(a)*sqrt(1-(x/a)^2))",
	NULL,

	"1/sqrt(1-(a*x)^2)",
	"pi*besselj(x/a,0)/abs(a)",
	NULL,
	
	"1/sqrt(1-(a*x+b)^2)",
	"pi*besselj(x/a,0)*exp(i*b*x/a)/abs(a)",
	NULL,


	"besselj(a*x,1)/(2*a*x)",
	"sqrt(1-(x/a)^2)",
	NULL,
	
	"sqrt(1-(a*x)^2)",
	"pi*besselj(a*x,1)/(2*x*abs(a))",
	NULL,
	
	"sqrt(1-(a*x+b)^2)",
	"pi*besselj(a*x,1)*exp(i*b*x/a)/(2*x*abs(a))",
	NULL,
	
	"dirac(x)",
	"1",
	NULL,
	
	"x",
	"2*i*pi*d(dirac(x),x)",
	NULL,
	
	"d(dirac(x),x)",
	"i*x",
	NULL,

	"x^2",
	"-d(d(dirac(x),x),x)*2*pi",
	NULL,
	
	"d(d(dirac(x),x),x)",
	"-x^2",
	NULL,

	"x^3",
	"-d(d(d(dirac(x),x),x),x)*2*i*pi",
	NULL,

	"d(d(d(dirac(x),x),x),x)",	
	"-i*x^3",
	NULL,
	
	"dirac(a*x+b)",
	"exp(i*b*x/a)/abs(a)",
	NULL,
	
	"exp(a*x)",
	"2*pi*dirac(x+i*a)",
	NULL,

	"exp(a*x+b*x)",
	"2*pi*dirac(x+i*a+i*b)",
	NULL,

	"exp(a*x+b)",
	"2*pi*dirac(x+i*a)*exp(b)",
	NULL,

		
/*	"exp(i*a*(-x))",
	"2*pi*dirac(-x+a)",
	NULL,*/
	

	"exp(a*abs(x))",
	"-2*a/(a^2+x^2)",
	NULL,

	"exp(a*abs(b*x+c))",
	"-2*a*exp(i*c*x/b)/((a^2+x^2)*abs(b))",
	NULL,

	"exp(a*x)/(b^2+x^2)",
	"pi*exp(-b*(abs(i*a-x)))/b",
	NULL,

	"1/(a+b*x^2)",
	"exp(-sqrt(a)*abs(x/sqrt(b)))*pi/(sqrt(a)*abs(sqrt(b)))",
	NULL,

	"1/(a+b*(x+c)^2)",
	"exp(-sqrt(a)*abs(x/sqrt(b)))*exp(i*c*x/sqrt(b))*pi/(sqrt(a)*abs(sqrt(b)))",
	NULL,
	
	"sgn(x)*exp(a*abs(x))",
	"-2*i*x/(a^2+x^2)",
	NULL,

	"sgn(x+b)*exp(a*abs(bx+c))",
	"-2*i*(x/b)*exp(i*c*x/b)/(a^2+(x/b)^2)",
	NULL,

	"heaviside(x)*exp(a*x)",
	"(-a-i*x)/(a^2+x^2)",
	NULL,

	"heaviside(x+b)*exp(a*(x+b))",
	"(-a-x)*exp(i*b*x)/(a^2+x^2)",
	NULL,
	
	"heaviside(x)*exp(a*x+b*x)",
	"1/(-b+(-i*a+x)*i)",
	NULL,
	
	"heaviside(x)*sin(a*x)*exp(b*x)",
	"a/((a^2+b^2-x^2)+2*i*b*x)",
	NULL,
	
	"heaviside(x)*cos(a*x)*exp(b*x)",
	"(a*(a^2+b^2+x^2)-i*x*(a^2+b^2+x^2))/((a^2+b^2-x^2)^2+(2*b*x)^2)",
	NULL,
	
	"cos(b*x)/(a+x^2)",
	"(exp(-sqrt(a)*abs(x-b))+exp(-sqrt(a)*abs(x+b)))*pi/(2*sqrt(a))",
	NULL,

	"sin(b*x)/(a+x^2)",
	"-i*(exp(-sqrt(a)*abs(x-b))-exp(-sqrt(a)*abs(x+b)))*pi/(2*sqrt(a))",
	NULL,
	
/*	"exp(i*a*x^2)",
	"sqrt(pi/a)*exp(-i*((x/a)^2-pi)/4)",
	NULL,
	
	"exp(i*a*(x+b)^2)",
	"sqrt(pi/a)*exp(-i*((x/a)^2-pi)/4)*exp(i*b*x)",
	NULL,
*/	
	"exp(a*x^2)",
	"sqrt(pi/(-a))*exp(x^2/(4*a))",
	NULL,
	
	"exp(a*(x+b)^2)",
	"sqrt(pi/(-a))*exp(x^2/(4*a))*exp(i*b*x)",
	NULL,
	
	"exp(a*x+b*abs(x))",
	"-2*b/(b^2+(x+i*a)^2)",
	NULL,

	"2*b/(b^2+(x+a)^2)",
	"-exp(-i*a*x-b*abs(x))",
	NULL,
	
	"cos(a*x)*exp(b*abs(x))",
	"-b*(1/(b^2+(x-a)^2)+1/(b^2+(x+a)^2))",
	NULL,
	
	"sin(a*x)*exp(b*abs(x))",
	"i*(-b*(1/(b^2+(x+a)^2)-1/(b^2+(x-a)^2)))",
	NULL,
	
	"1-carac(x,-a,a)",
	"pi*dirac(x)-sin(a*x)/x",
	NULL,	
	
	"cos(a*x)",
	"pi*(dirac(x-a)+dirac(x+a))",
	NULL,
	
	"cos(a*x+b)",
	"pi*(dirac(x-a)+dirac(x+a))*exp(i*b*x/a)",
	NULL,
	
	"sin(a*x)",
	"i*pi*(-dirac(x-a)+dirac(x+a))",
	NULL,
	
	"sin(a*x+b)",
	"i*pi*(-dirac(x-a)+dirac(x+a))*exp(i*b*x/a)",
	NULL,
	
	"cos(a*x)^2",
	"pi*(1/2*(dirac(x-2*a)+dirac(x+2*a))+dirac(x))",
	NULL,

	"cos(a*x+b)^2",
	"(pi*(1/2*(dirac(x-2*a)+dirac(x+2*a))+dirac(x)))*exp(i*b*x/a)",
	NULL,

	"sin(a*x)^2",
	"pi*(-1/2*(dirac(x-2*a)+dirac(x+2*a))+dirac(x))",
	NULL,
	
	"sin(a*x+b)^2",
	"pi*(-1/2*(dirac(x-2*a)+dirac(x+2*a))+dirac(x))*exp(i*b*x/a)",
	NULL,
	
	"1/x",
	"-i*sgn(x)*pi",
	NULL,
	
	"1/(a*x+b)",
	"-i*exp(i*b*x/a)*sgn(x)*pi/a",
	NULL,

	"exp(a*x)*sgn(x)",
	"-2*i/(x+i*a)",
	NULL,
		
/*	"1/(-x+a)",
	"exp(i*a*x)*sgn(-x)*(pi/i)",
	NULL,

	"1/(x-a)",
	"exp(i*a*x)*sgn(x)*(pi/i)",
	NULL,
	
	"1/(-x-a)",
	"exp(-i*a*x)*sgn(-x)*(pi/i)",
	NULL,
*/
	
	"sgn(a*x)",
	"-2*i*sgn(a)/x",
	NULL,
	
	"sgn(a*x+b)",
	"-2*i*exp(i*b*x/a)*sgn(a)/x",
	NULL,
	
	"heaviside(a*x)",
	"pi*dirac(x)-i*sgn(a)/x",
	NULL,
	
	"heaviside(a*x+b)",
	"pi*dirac(x)-i*sgn(a)*exp(i*b*x/a)/x",
	NULL,

	
	"heaviside(x)*(1-exp(a*x))",
	"pi*dirac(x)-(-a/(a^2-x^2)+i*a^2/(x*(a^2-x^2)))",
	NULL,

	"heaviside(x+b)*(1-exp(a*(x+b)))",
	"(pi*dirac(x)-(-a/(a^2-x^2)+i*a^2/(x*(a^2-x^2))))*exp(i*b*x)",
	NULL,

	"heaviside(x)*sin(a*x)",
	"a/(a^2-x^2)-i*pi*(dirac(x-a)-dirac(x+a))/2",
	NULL,

	"heaviside(x+b)*sin(a*(x+b))",
	"(a/(a^2-x^2)-i*pi*(dirac(x-a)-dirac(x+a))/2)*exp(i*b*x)",
	NULL,
		
	"heaviside(x)*cos(a*x)",
	"i*x/(a^2-x^2)+pi*(dirac(x-a)-dirac(x+a))/2",
	NULL,

	"heaviside(x+b)*cos(a*(x+b))",
	"(i*x/(a^2-x^2)+pi*(dirac(x-a)-dirac(x+a))/2)*exp(i*b*x)",
	NULL,

	"heaviside(x)*x",
	"-1/x^2+i*pi*d(dirac(x),x)",
	NULL,
	
	"heaviside(x)*x*exp(a*x)",
	"(a^2-x^2)/(a^2+x^2)^2+i*2a*x/(a^2+x^2)^2",
	NULL,

	
	"abs(a*x)",
	"-2/(abs(a)*(x/a)^2)",
	NULL,

	"abs(a*x+b)",
	"-2*exp(i*b*x/a)/(abs(a)*(x/a)^2)",
	NULL,
	
	
	"1/(a*x)^2",
	"-abs(x/a)*pi/abs(a)",
	NULL,

	"1/(a*x+b)^2",
	"-abs(x/a)*exp(i*b*x/a)*pi/abs(a)",
	NULL,
	
	"1/abs(a*x)",
	"(-2*euler-2*log(abs(x/a)))/abs(a)",
	NULL,
	
	"1/abs(a*x+b)",
	"(-2*euler-2*log(abs(x/a)))*exp(i*b*x/a)/abs(a)",
	NULL,
	
	"log(abs(a*x))",
	"(-2*pi*euler*dirac(x/a)-pi/abs(x/a))/abs(a)",
	NULL,
	
	"log(abs(a*x+b))",
	"(-2*pi*euler*dirac(x/a)-pi/abs(x/a))*exp(i*b*x/a)/abs(a)",
	NULL,
	
	"x^a",
	"-abs(x)^(-a-1)*gamma(1+a)*(-i*(-1+(-1)^a)*cos(a*pi/2)*sgn(x)+(-1+(-1)^a)*cos(a*pi/2))",
	NULL,

	"(x+b)^a",
	"-abs(x)^(-a-1)*gamma(1+a)*exp(i*b*x)*(-i*(-1+(-1)^a)*cos(a*pi/2)*sgn(x)+(-1+(-1)^a)*cos(a*pi/2))",
	NULL,
	
	"x^a*heaviside(x)",
	"-i*abs(x)^(-a-1)*gamma(1+a)*(cos(a*pi/2)*sgn(x)-i*sin(a*pi/2))",
	NULL,
	
	"(x+b)^a*heaviside(x+b)",
	"-i*exp(i*b*x)*abs(x)^(-a-1)*gamma(1+a)*(cos(a*pi/2)*sgn(x)-i*sin(a*pi/2))",
	NULL,
	
	"abs(x)^a*heaviside(x)",
	"-i*abs(x)^(-a-1)*gamma(1+a)*(cos(a*pi/2)*sgn(x)-i*sin(a*pi/2))",
	NULL,
	
	"abs(x+b)^a*heaviside(x+b)",
	"-i*exp(i*b*x)*abs(x)^(-a-1)*gamma(1+a)*(cos(a*pi/2)*sgn(x)-i*sin(a*pi/2))",
	NULL,
	
	"abs(x)^a",
	"-2*gamma(a+1)*sin(pi*a/2)*abs(x)^(-a-1)",
	NULL,
	
	"abs(x+b)^a",
	"-2*exp(i*b*x)*gamma(a+1)*sin(pi*a/2)*abs(x)^(-a-1)",
	NULL,
	
	"abs(x)^a*sgn(x)",
	"-2*i*gamma(a+1)*cos(pi*a/2)*sgn(x)*abs(x)^(-a-1)",
	NULL,
	
	"abs(x+b)^a*sgn(x+b)",
	"-2*i*exp(i*b*x)*gamma(a+1)*cos(pi*a/2)*sgn(x)*abs(x)^(-a-1)",
	NULL,
	
	"carac(x,a,b)",
	"i*(exp(-i*b*x)-exp(-i*a*x))/x",
	NULL,

	"carac(x,-a,a)*exp(b*x)",
	"2*sin(a*(x+i*b))/(x+i*b)",
	NULL,

	"carac(x,-a,a)*sin(b*x)",
	"i*(sin(a*(x+b))/(x+b)-sin(a*(x-b))/(x-b))",
	NULL,

	"carac(x,-a,a)*cos(b*x)",
	"sin(a*(x+b))/(x+b)+sin(a*(x-b))/(x-b)",
	NULL,
	
	"carac(x,-a,a)*x",
	"2*i*(cos(a*x)-sin(a*x)/(a*x))/x",
	NULL,
	
	"carac(x,-a,a)*abs(x)",
	"2*a*(sin(a*x)/(a*x)-2*(sin(a*x/2)/(a*x))^2)",
	NULL,
	
	"(exp(-i*b*x)-exp(-i*a*x))/x",
	"2*pi*carac(x,a,b)/i",
	NULL,

	"sin(a*x)/x",
	"pi*carac(x,-a,a)",
	NULL,

	"sgn(x)*carac(x,-a,a)",
	"-4*i*sin(a*x/2)*sin(a*x/2)/x",
	NULL,

	NULL,
};

// build the table, subst. meta symbols for a, b and x.

static void
scan_fouriers(void)
{
	int h, i, k;

	k = tos;

	i = 0;

	while (fouriers[i]) {

		h = tos;

		while (fouriers[i]) {
			scan(fouriers[i++]);
			push_symbol(SYMBOL_A);
			push(meta_a);
			subst();
			push_symbol(SYMBOL_B);
			push(meta_b);
			subst();
			push_symbol(SYMBOL_C);
			push(meta_c);
			subst();
			push_symbol(SYMBOL_N);
			push(meta_n);
			subst();
			push_symbol(SYMBOL_X);
			push(meta_x);
			subst();
		}

		list(tos - h);

		i++;
	}

	list(tos - k);

	table_of_fourier = pop();
}

static char *s[] = {

	"fourier(d(f(t,x),x),x)",
	"i*x*fourier(f(t,x),x)",

	"fourier(d(f(t,x),t),x)",
	"d(fourier(f(t,x),x),t)",

	"fourier(x*f(t,x),x)",
	"i*d(fourier(f(t,x),x),x)",

	"fourier(f(t,x)/x,x)",
	"-i*integral(fourier(f(t,x),x),x)",

	"fourier(integral(f(t,x),x),x)",
	"pi*dirac(x)*fourier(f(t,x),x)-i*fourier(f(t,x),x)/x",

	"fourier(carac(x,-1,1),x)",
	"i*exp(-i*x)/x-i*exp(i*x)/x",

	"fourier(dirac(x),x)",
	"1",

	"fourier(1,x)",
	"2*pi*dirac(x)",

	"fourier(exp(i*b*x),x)",
	"2*pi*dirac(b-x)",

	"fourier(d(dirac(A + x),x),x)",
	"i*x*exp(i*A*x)",

	"fourier(1/x^2,x)",
	"-pi*abs(x)",

	"fourier(d(f(x),x),x)",
	"i*x*fourier(f(x),x)",

	"fourier(d(d(f(x),x),x))",
	"-x^2*fourier(f(x),x)",

	"fourier(x*f(x),x)",
	"i*d(fourier(f(x),x),x)",

	"fourier(x^2*f(x),x)",
	"-d(d(fourier(f(x),x),x),x)",

	"eval(fourier(dirac(t)-d(d(dirac(t),t),t),t))",
	"1+t^2",

	"eval(invfourier(1/(a+t^2),t))",
	"exp(-a^(1/2)*abs(t))/(2*a^(1/2))",

	"eval(fourier(fourier(d(dirac(t)*dirac(x),t)-d(d(dirac(t)*dirac(x),x),x),t),x))",
	"i*t+x^2",

	"fourier(exp(-a*x^2),x)",
	"pi^(1/2)*exp(-x^2/(4*a))/(a^(1/2))",

	"fourier(exp(-a*abs(x)),x)",
	"2*a/(a^2+x^2)",

	"fourier(sgn(x)*exp(-a*abs(x)),x)",
	"-2*i*x/(a^2+x^2)",

	"fourier(heaviside(x)exp(-a*x),x)",
	"a/(a^2+x^2)-i*x/(a^2+x^2)",

	"fourier(1 / abs(a*x),x)",
	"-2*euler/abs(a)-2*log(abs(x/a))/abs(a)",

	"fourier(besselj(x,0),x)",
	"2/((1-x^2)^(1/2))",

	"fourier(x exp(-pi x^2),x)",
	"-i*x*exp(-x^2/(4*pi))/(2*pi)",

	"fourier(exp(-abs(x)) sin(x) / x,x)",
	"-2*i*arctan(x)",

	"fourier(exp(-b abs(x) + i a x),x)",
	"2*b/(-2*a*x+a^2+b^2+x^2)",

	"fourier(exp(-a x + i b x) heaviside(x),x)",
	"1/(-a-i*b+i*x)",

	"fourier(cos(b x) / (a^2 + x^2),x)",
	"pi*exp(-a*abs(b+x))/(2*a)+pi*exp(-a*abs(b-x))/(2*a)",

	"fourier(sin(b x) / (a^2 + x^2),x)",
	"i*pi*exp(-a*abs(b+x))/(2*a)-i*pi*exp(-a*abs(b-x))/(2*a)",
};

void
test_fourier(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
