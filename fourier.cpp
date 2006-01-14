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
//	- integral : fourier(integral(f(t),t),t)=fourier(f(t),t)/(i*t)
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
static void fourier_of_inverse_trig(void);
static int match(U *, U *, U *, int, int);
static void scan_fouriers(void);


void
eval_fourier(void)
{
	int expotemp;

	expotemp=expomode;

	expomode=0;
	push(cadr(p1));
	eval();
	if (caddr(p1) == Nil)
		guess();
	else
		push(caddr(p1));
	fourier();
	p5=pop();

//	print(p5);
//	printf("\n");
//	printf("%d\n",length(p5));

	expomode=1;
	push(cadr(p1));
	eval();
	if (caddr(p1) == Nil)
		guess();
	else
		push(caddr(p1));
	fourier();
	p6=pop();

//	print(p6);
//	printf("\n");
//	printf("%d\n",length(p6));
//
	if (find(p5,symbol(FOURIER))==1)
		push(p6);
	else
		if ((length(p6) <=  length(p5)) || (find(p6,symbol(FOURIER))==1))
			push(p5);
		else
			push(p6);

	expomode=expotemp;
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

	if (table_of_fourier == Nil)
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
	else if (	find(p1,symbol(ARCCOS))==1 ||
			find(p1,symbol(ARCCOSH))==1 ||
			find(p1,symbol(ARCSIN))==1 ||
			find(p1,symbol(ARCSINH))==1 ||
			find(p1,symbol(ARCTAN))==1 ||
			find(p1,symbol(ARCTANH))==1/* ||
			find(p1,symbol(HEAVISIDE))==1 ||
			find(p1,symbol(SGN))==1*/)
		fourier_of_inverse_trig();
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
	p3=pop();
/*	print(p3);
	printstr("\n");*/
	push(p3);
	push(imaginaryunit);
	push(p2);
	multiply();
	divide();
//	if ((find(p3,symbol(FOURIER))==0) && 
//		(find(p3,symbol(INTEGRAL))==0) &&
//		(find(p3,symbol(DERIVATIVE))==0)) {
//		push_symbol(PI);
//		push(p3);
//		push(p2);
//		push_integer(0);
//		evalat();
//		multiply();
//		add();
//	}
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
fourier_of_inverse_trig(void)
{
	push(p1);
	push(p2);
	derivative();
	push(p2);
	fourier();
	push(imaginaryunit);
	push(p2);
	multiply();
	divide();
}

static void
fourier_of_product(void)
{
	int h1, h2,n=0;

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
		if (find(car(p3), p2)){
			push(car(p3));}
		if (caar(p3) == symbol(POWER) && cadar(p3) == p2 && isinteger(caddar(p3))){
			push(caddar(p3));
			n = pop_integer();
		}
		if (car(p3) == p2){
			push_integer(1);
			n = pop_integer();
		}
		p3 = cdr(p3);
	}

	if (tos - h2 == 0){
		push_integer(2);
		push_symbol(PI);
		multiply();
		push(p2);
		dirac();
		multiply();}
	else {
		multiply_all(tos - h2);
		p1 = pop();
		if (!(n==0)){
			push(p1);
			push(p1);
			p3=pop();
			push(p2);
			push_integer(n);
			power();
			divide();
			p1=pop();
		}
		if (!isplusone(p1)) {
			fourier_of_form();
			if (!(n==0)) {
				if (n > 0) {
					while(n > 0){
					push(p2);
					derivative();
					push(imaginaryunit);
					multiply();
					n=n-1;}
				}
				else {
					n=-n;
					while(n > 0){
					push(p2);
					integral();
					push(imaginaryunit);
					push_integer(-1);
					multiply();
					multiply();
					n=n-1;}
				}
			}
		}
		else {
			push(p3);
			push(p2);
			fourier();
			}
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
						if (iszero(p2))
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


char * fouriers[] = {

	"a",
	"2*pi*a*dirac(x)",
	NULL,
	
	"dirac(a*x)",
	"a",
	NULL,
	
	"dirac(a*x+b)",
	"exp(i*b*x/a)/abs(a)",
	NULL,

	"heaviside(x)",
	"pi*dirac(x)-i/x",
	NULL,

	"heaviside(x+a)",
	"pi*dirac(x)-i*exp(i*a*x)/x",
	NULL,
	
	"heaviside(x)*exp(a*x)",
	"(-a-i*x)/(a^2+x^2)",
	"isnegativeterm(a)==1",
	NULL,

	"sgn(a*x)",
	"-2*i*sgn(a)/x",
	NULL,
	
	"sgn(a*x+b)",
	"-2*i*exp(i*b*x/a)*sgn(a)/x",
	NULL,

	"exp(a*x)*sgn(x)",
	"-2*i/(x+i*a)",
	NULL,
	
	"carac(x,a,b)",
	"i*(exp(-i*b*x)-exp(-i*a*x))/x",
	NULL,

	"carac(x,-a,a)*exp(b*x)",
	"2*sin(a*(x+i*b))/(x+i*b)",
	NULL,

	"carac(x,-a,a)*abs(x)",
	"2*a*(sin(a*x)/(a*x)-2*(sin(a*x/2)/(a*x))^2)",
	NULL,

	"sgn(x)*carac(x,-a,a)",
	"-4*i*sin(a*x/2)*sin(a*x/2)/x",
	NULL,
	
	"shah(a*x)",
	"2*pi*shah(2*pi*x/a)/a",
	NULL,

	"shah(a*x+b)",
	"2*pi*exp(i*b*x/a)*shah(2*pi*x/a)/a",
	NULL,

	"besselj(a*x,0)",
	"2/(abs(a)*sqrt(1-(x/a)^2))",
	NULL,
	
	"besselj(a*x+b,0)",
	"2*exp(i*b*x/a)/(abs(a)*sqrt(1-(x/a)^2))",
	NULL,

	"exp(a*x)*besselj(b*x,0)",
	"2/(abs(b)*sqrt(1-((x+i*a)/b)^2))",
	NULL,
	
	"1/sqrt(a+b*x^2)",
	"pi*besselj(-sgn(b/a)*x*sqrt(-a/b),0)/sqrt(a*abs(b))",
	NULL,
	
	"1/sqrt(1+a*(x+b)^2)",
	"pi*besselj(-sgn(a)x*sqrt(-1/a),0)*exp(i*b*x)/sqrt(abs(a))",
	NULL,

	"exp(a*x)/sqrt(1+b*x^2)",
	"pi*besselj(-sgn(b)(-i*x+i*a)*sqrt(-1/b),0)/sqrt(abs(b))",
	NULL,

	"besselj(a*x,1)/(2*a*x)",
	"sqrt(1-(x/a)^2)",
	NULL,
	
	"sqrt(1+a*x^2)",
	"pi*besselj(i*sqrt(a)*x,1)/(x*abs(i*sqrt(a)))",
	NULL,
	
	"sqrt(1+a*(x+b)^2)",
	"pi*besselj(i*sqrt(a)*x,1)*exp(i*b*x)/(x*abs(i*sqrt(a)))",
	NULL,
	
	"exp(a*x)*sqrt(1+b*x^2)",
	"pi*besselj(i*sqrt(b)*(x+i*a),1)/((x+i*a)*abs(i*sqrt(b)))",
	NULL,

	"erf(a*x)",
	"-i*2*exp(-(x/(2*a))^2)/x",
	NULL,
	
	"erf(a*x+b)",
	"-i*2*exp(-(x/(2*a))^2)*exp(i*b*x/a)/x",
	NULL,

	"x^a",
	"2*(i^a)*pi*d(dirac(x),x,a)",
	"and(a>0,isinteger(a)==1)",
	NULL,
	
	"(x+a)^b",	
	"2*(i^b)*pi*d(dirac(x),x,b))*exp(i*a*x)",
	"and(b>0,isinteger(b)==1)",
	NULL,

	"x^a",
	"(-1)^(-a)*i^(-a)*x^(-a-1)*pi*sgn(x)/(-a-1)!",
	"and(a<0,isinteger(a)==1)",
	NULL,

	"(x+a)^b",	
	"(-1)^(-b)*i^(-b)*x^(-b-1)*exp(i*a*x)*pi*sgn(x)/(-b-1)!",
	"and(b<0,isinteger(b)==1)",
	NULL,

	"1/(a*x+b)",
	"-i*exp(i*b*x/a)*sgn(x)*pi/a",
	NULL,

	"exp(a*x)/(c*x+b)",
	"-i*exp(i*b*(x+i*a)/c)*sgn(x+i*a)*pi/c",
	NULL,

	"1/(a*x^2+b)",
	"exp(-sqrt(b/a)*abs(x))*pi/(sqrt(b*a))",
	NULL,

	"1/(a*x+b)^2",
	"-abs(x/a)*exp(i*b*x/a)*pi/abs(a)",
	NULL,

	"exp(a*x)/(b*x^2+c)",
	"exp(-sqrt(c/b)*abs(x+i*a))*pi/(sqrt(b*c))",
	NULL,

	"1/(a*(x+b)^2+c)",
	"exp(i*b*x)*exp(sqrt(c/a)*abs(x))*pi/(sqrt(c*a))",
	NULL,

	"1/(a*x^2+b*x)",
	"(-i*pi*sgn(x)+i*pi*exp(i*b*x/a)*sgn(x))/b",
	NULL,

	"1/abs(a*x)",
	"(-2*euler-2*log(abs(x/a)))/abs(a)",
	NULL,
	
	"1/abs(a*x+b)",
	"(-2*euler-2*log(abs(x/a)))*exp(i*b*x/a)/abs(a)",
	NULL,

	"exp(a*x)/abs(x)",
	"-2*euler-2*log(abs(x+i*a))",
	NULL,
	
	"exp(a*x)/abs(b*x+c)",
	"(-2*euler-2*log(abs((x+i*a)/b)))*exp(i*c*(x+i*a)/b)/abs(b)",
	NULL,
		
	"log(a*abs(x)))",
	"(-2*pi*euler*dirac(x/abs(a))-pi/abs(x/a))/abs(a)",
	NULL,
	
	"log(abs(a*x+b))",
	"(-2*pi*euler*dirac(x/abs(a))-pi/abs(x/a))*exp(i*b*x/a)/abs(a)",
	NULL,
	
	"exp(a*x)*log(abs(b*x))",
	"(-2*pi*euler*dirac((x+i*a)/b)-pi/abs((x+i*a)/b))/abs(b)",
	NULL,
	
	"exp(a*x)*log(abs(b*x+c))",
	"(-2*pi*euler*dirac((x+i*a)/b)-pi/abs(x/b))*exp(i*c*(x+i*a)/b)/abs(b)",
	NULL,

	"abs(a*x)",
	"-2/(abs(a)*(x/a)^2)",
	NULL,

	"abs(a*x+b)",
	"-2*exp(i*b*x/a)/(abs(a)*(x/a)^2)",
	NULL,
	
	"exp(a*x)*abs(b*x)",
	"-2/(abs(b)*((x+i*a)/b)^2)",
	NULL,
	
	"abs(x)^a",
	"-2*Gamma(a+1)*sin(pi*a/2)*abs(x)^(-a-1)",
	"and(not(abs(a)==1),isinteger(a)==0)",
	NULL,
	
	"exp(a*x)*abs(x)^b",
	"-2*Gamma(b+1)*sin(pi*b/2)*abs((x+i*a))^(-b-1)",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"abs(x+a)^b",
	"-2*exp(i*a*x)*Gamma(b+1)*sin(pi*b/2)*abs(x)^(-b-1)",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"exp(a*x)*abs(x+c)^b",
	"-2*exp(i*c*(x+i*a))*Gamma(b+1)*sin(pi*b/2)*abs((x+i*a))^(-b-1)",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"abs(x)^a*sgn(x)",
	"-2*i*Gamma(a+1)*cos(pi*a/2)*sgn(x)*abs(x)^(-a-1)",
	"and(not(abs(a)==1),isinteger(a)==0)",
	NULL,
	
	"exp(a*x)*abs(x)^b*sgn(x)",
	"-2*i*Gamma(b+1)*cos(pi*b/2)*sgn((x+i*a))*abs(x)^(-b-1)",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"abs(x+b)^a*sgn(x+b)",
	"-2*i*exp(i*b*x)*Gamma(a+1)*cos(pi*a/2)*sgn(x)*abs(x)^(-a-1)",
	"and(not(abs(a)==1),isinteger(a)==0)",
	NULL,
	
	"abs(x)^a*heaviside(x)",
	"-i*abs(x)^(-a-1)*Gamma(1+a)*(cos(a*pi/2)*sgn(x)-i*sin(a*pi/2))",
	"and(not(abs(a)==1),isinteger(a)==0)",
	NULL,
	
	"exp(a*x)*abs(x)^b*heaviside(x)",
	"-i*abs((x+i*a))^(-b-1)*Gamma(1+b)*(cos(b*pi/2)*sgn((x+i*a))-i*sin(b*pi/2))",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"abs(x+a)^b*heaviside(x+a)",
	"-i*exp(i*a*x)*abs(x)^(-b-1)*Gamma(1+b)*(cos(b*pi/2)*sgn(x)-i*sin(b*pi/2))",
	"and(not(abs(b)==1),isinteger(b)==0)",
	NULL,
	
	"exp(a*x)*abs(x+c)^b*heaviside(x+c)",
	"-i*exp(i*c*(x+i*a))*abs((x+i*a))^(-b-1)*Gamma(1+b)*(cos(b*pi/2)*sgn((x+i*a))-i*sin(b*pi/2))",
	"and(not(abs(b)==1),isinteger(b)==0)",
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
	
	"heaviside(x)*exp(a*x)",
	"(-a-i*x)/(a^2+x^2)",
	"isnegativeterm(a)==1",
	NULL,

	"exp(a*x)*heaviside(x)",
	"(i*a+i*x)/(a^2+x^2)+pi*(dirac(x+i*a)-dirac(x-i*a))",
	"and(real(a)==0,not(imag(a)==0))",
	NULL,

	"heaviside(x+b)*exp(a*(x+b))",
	"(a+x)*exp(i*b*x)/(a^2+x^2)",
	NULL,
	
	"heaviside(x)*exp(a*x+b*x)",
	"1/(-b+(i*a+x)*i)",
	NULL,
	
	"exp(a*abs(x))",
	"-2*a/(a^2+x^2)",
	"isnegativeterm(a)==1",
	NULL,

	"exp(a*abs(b*x+c))",
	"-2*a*exp(i*c*x/b)/((a^2+(x/b)^2)*abs(b))",
	"isnegativeterm(a)==1",
	NULL,

	"exp(a*x+b*abs(x))",
	"-2*b/(b^2+(x+i*a)^2)",
	NULL,

	"2*a/(a^2+(x+b)^2)",
	"-exp(-i*b*x-a*abs(x))",
	NULL,
	
	"sgn(x)*exp(a*abs(x))",
	"-2*i*x/(a^2+x^2)",
	NULL,

	"sgn(x+b)*exp(a*abs(b*x+c))",
	"-2*i*(x/b)*exp(i*c*x/b)/(a^2+(x/b)^2)",
	NULL,

	"exp(a*x^2)",
	"sqrt(pi/(2*abs(imag(a))))*exp(-i*x^2/(4*imag(a)))*(1+i*sgn(imag(a)))",
	"and(real(a)==0,not(imag(a)==0))",
	NULL,

	"exp(a*(x+b)^2)",
	"sqrt(pi/(2*abs(imag(a))))*exp(-i*x^2/(4*imag(a)imag(a)))*(1+i*sgn(imag(a)))*exp(i*b*x)",
	"and(real(a)==0,not(imag(a)==0))",
	NULL,
	
	"exp(a*x^2)",
	"sqrt(pi/abs(a))*exp(x^2/(4*a))",
	"and(imag(a)==0,isnegativeterm(a)==1)",
	NULL,
	
	"exp(a*(x+b)^2)",
	"sqrt(pi/abs(a))*exp(x^2/(4*a))*exp(i*b*x)",
	"and(imag(a)==0,isnegativeterm(a)==1)",
	NULL,
};

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
	"#1",
	"fourier(d(f(t,x),x),x)",
	"i*x*fourier(f(t,x),x)",

	"#2",
	"eval(invfourier(i*x*fourier(f(t,x),x),x))",
	"d(f(t,x),x)",

	"#3",
	"fourier(d(f(t,x),t),x)",
	"d(fourier(f(t,x),x),t)",

	"#4",
	"eval(invfourier(d(fourier(f(t,x),x),t),x))",
	"d(f(t,x),t)",

	"#5",
	"fourier(x*f(t,x),x)",
	"i*d(fourier(f(t,x),x),x)",

	"#6",
	"eval(invfourier(i*d(fourier(f(t,x),x),x),x))",
	"x*f(t,x)",

	"#7",
	"fourier(d(d(f(x),x),x))",
	"-x^2*fourier(f(x),x)",

	"#8",
	"eval(invfourier(-x^2*fourier(f(x),x),x))",
	"d(d(f(x),x),x)",

	"#9",
	"fourier(x^2*f(x),x)",
	"-d(d(fourier(f(x),x),x),x)",

	"#10",
	"eval(invfourier(-d(d(fourier(f(x),x),x),x),x))",
	"x^2*f(x)",

	"#11",
	"fourier(f(t,x)/x,x)",
	"-i*integral(fourier(f(t,x),x),x)",

	"#12",
	"eval(invfourier(-i*integral(fourier(f(t,x),x),x),x))",
	"f(t,x)/x",

	"#13",
	"fourier(integral(f(t,x),x),x)",
	"-i*fourier(f(t,x),x)/x",

	"#14",
	"eval(invfourier(-i*fourier(f(t,x),x)/x,x))",
	"integral(f(t,x),x)",

	"#15",
	"fourier(exp(-a^2*x^2),x)",
	"pi^(1/2)*exp(-x^2/(4*a^2))/abs(a)",

	"#16",
	"eval(invfourier(pi^(1/2)*exp(-x^2/(4*a^2))/abs(a),x))",
	"exp(-a^2*x^2)",

	"#17",
	"fourier(exp(-a*abs(x)),x)",
	"2*a/(a^2+x^2)",

	"#18",
	"eval(invfourier(2*a/(a^2+x^2),x))",
	"exp(-a*abs(x))",

	"#19",
	"fourier(heaviside(x)*exp(-a*x),x)",
	"a/(a^2+x^2)-i*x/(a^2+x^2)",

	"#20",
	"eval(invfourier(a/(a^2+x^2)-i*x/(a^2+x^2),x))",
	"1/2*exp(-a*abs(x))*sgn(x)+1/2*exp(-a*abs(x))",

	"#21",
	"fourier(sgn(x)*exp(-a*abs(x)),x)",
	"-2*i*x/(a^2+x^2)",

	"#22",
	"eval(invfourier(-2*i*x/(a^2+x^2),x))",
	"exp(-a*abs(x))*sgn(x)",

	"#23",
	"fourier(exp(i*y0*x-a*abs(x)),x)",
	"2*a/(-2*x*y0+a^2+x^2+y0^2)",

	"#24",
	"fourier(heaviside(x)*exp(i*y0*x-a*x),x)",
	"1/(a+i*x-i*y0)",

	"#25",
	"fourier(exp(i*x*y0)*carac(x,-L,L),x)",
	"2*sin(L*x-L*y0)/(x-y0)",

	"#26",
	"fourier(sin(a*x),x)",
	"i*pi*dirac(a+x)-i*pi*dirac(a-x)",

	"#27",
	"eval(invfourier(i*pi*dirac(a+x)-i*pi*dirac(a-x),x))",
	"1/2*i*exp(-i*a*x)-1/2*i*exp(i*a*x)",

	"#28",
	"fourier(cos(a*x),x)",
	"pi*dirac(a+x)+pi*dirac(a-x)",

	"#29",
	"eval(invfourier(pi*dirac(a+x)+pi*dirac(a-x),x))",
	"1/2*exp(-i*a*x)+1/2*exp(i*a*x)",

	"#30",
	"fourier(sinh(a*x),x)",
	"-pi*dirac(x-i*a)+pi*dirac(x+i*a)",

	"#31",
	"eval(invfourier(-pi*dirac(x-i*a)+pi*dirac(x+i*a),x))",
    "-1/2*exp(-a*x)+1/2*exp(a*x)",
    
	"#32",
	"fourier(cosh(a*x),x)",
	"pi*dirac(x-i*a)+pi*dirac(x+i*a)",

	"#33",
	"eval(invfourier(pi*dirac(x-i*a)+pi*dirac(x+i*a),x))",
    "1/2*exp(-a*x)+1/2*exp(a*x)",
    
	"#34",
	"fourier(exp(i*b*x),x)",
	"2*pi*dirac(b-x)",

	"#35",
	"fourier(dirac(x),x)",
	"1",

	"#36",
	"fourier(1,x)",
	"2*pi*dirac(x)",

	"#37",
	"fourier(d(dirac(A + x),x),x)",
	"i*x*exp(i*A*x)",

	"#38",
	"fourier(carac(x,-1,1),x)",
	"i*exp(-i*x)/x-i*exp(i*x)/x",

	"#39",
	"fourier(shah(a*x),x)",
	"2*pi*shah(2*pi*x/a)/a",

	"#40",
	"fourier(1/x,x)",
	"-i*pi*sgn(x)",

	"#41",
 	"fourier(1/(a*x+b),x)",
	"-i*pi*exp(i*b*x/a)*sgn(x)/a",

	"#42",
	"fourier(1/x^2,x)",
	"-pi*x*sgn(x)",

	"#43",
	"fourier(1/(x^2+a^2),x)",
	"pi*exp(-a*abs(x))/a",

	"#44",
	"fourier(1 / abs(x),x)",
	"-2*euler-2*log(abs(x))",

	"#45",
	"fourier(sqrt(abs(x)))",
	"-pi^(1/2)/(2^(1/2)*abs(x)^(3/2))",

	"#46",
	"fourier(exp(-a*abs(x)),x)",
	"2*a/(a^2+x^2)",

	"#47",
	"eval(invfourier(2*a/(a^2+t^2),t))",
	"exp(-a*abs(t))",


	"#48",
	"fourier(sgn(x)*exp(-a*abs(x)),x)",
	"-2*i*x/(a^2+x^2)",

	"#49",
	"fourier(exp(-abs(x))/x,x)",
	"-2*i*arctan(x)",

	"#50",
	"fourier(exp(-b abs(x) + i a x),x)",
	"2*b/(-2*a*x+a^2+b^2+x^2)",

	"#51",
	"fourier(exp(-a x + i b x) heaviside(x),x)",
	"1/(a-i*b+i*x)",

	"#52",
	"fourier(exp(-a*x^2),x)",
	"pi^(1/2)*exp(-x^2/(4*a))/(abs(a)^(1/2))",

	"#53",
	"fourier(x*exp(-x^2))",
	"-1/2*i*pi^(1/2)*x*exp(-1/4*x^2)",

	"#54",
	"rationalize(fourier(exp(i*a*x^2),x))",
	"pi^(1/2)*exp(-i*x^2/(4*a))*(1+i*sgn(a))/(2^(1/2)*abs(a)^(1/2))",

	"#55",
	"fourier(besselj(x,0),x)",
	"2/((1-x^2)^(1/2))",

	"#56",
	"eval(invfourier(2/((1-x^2)^(1/2)),x))",
	"besselj(x,0)",

	"#57",
	"fourier(exp(i*b x) / (a^2 + x^2),x)",
	"pi*exp(-a*abs(b-x))/a",

	"#58",
	"eval(fourier(dirac(t)-d(d(dirac(t),t),t),t))",
	"1+t^2",

	"#59",
	"eval(fourier(fourier(d(dirac(t)*dirac(x),t)-d(d(dirac(t)*dirac(x),x),x),t),x))",
	"i*t+x^2",

};

void
test_fourier(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
