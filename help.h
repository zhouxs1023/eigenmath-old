static char *help_type_caret[] = {
	"x^2",
};

static char *help_multiply[] = {
	"A (B+C)",
	"A*(B+C)",
};

static char *help_rational_arithmetic[] = {
	"1/2 + 1/3",
	"float",
};

static char *help_solve[] = {
	"A = ((1,1),(0.24,0.86))",
	"B = (274,120.32)",
	"X = dot(inv(A),B)",
	"X",
};

static char *help_factor_polynomial[] = {
	"factor(125 x^3 - 1)",
};

static char *help_factor_number[] = {
	"factor(10^20 - 1)",
};

static char *help_parabola[] = {
	"draw(x^2)",
};

static char *help_circle[] = {
	"r = (cos(t),sin(t))",
	"draw(5r)",
};

static char *help_lissajous[] = {
	"r = (sin(2t),cos(3t))",
	"draw(5r)",
};

// linear algebra

static char *help_adj[] = {
	"A = ((a,b),(c,d))",
	"adj(A)",
	"inv(A) - adj(A) / det(A)",
};

static char *help_contract[] = {
	"A = ((a,b),(c,d))",
	"A",
	"contract(A)",
};

static char *help_det[] = {
	"A = ((a,b),(c,d))",
	"A",
	"det(A)",
};

static char *help_dot[] = {
	"A = ((1,2),(3,4))",
	"X = (X1,X2)",
	"dot(A,X)",
};

static char *help_inv[] = {
	"A = ((1,2),(3,4))",
	"inv(A)",
	"dot(A,inv(A))",
};

static char *help_outer[] = {
	"A = (A1,A2)",
	"B = (B1,B2)",
	"outer(A,B)",
	"dot(A,B) - contract(outer(A,B))",
};

static char *help_transpose[] = {
	"A = ((1,2),(3,4))",
	"transpose(A)",
};

static char *help_unit[] = {
	"I = unit(2)",
	"I",
};

static char *help_zero[] = {
	"A = zero(2,2)",
	"A",
	"A[1,1] = A11",
	"A[1,2] = A12",
	"A",
};

// calculus

static char *help_derivative[] = {
	"d(x^2)",
};

static char *help_gradient[] = {
	"f = x^2 + y^2",
	"d(f,(x,y))",
};

static char *help_integral[] = {
	"integral(x^2)",
};

// complex number functions

static char *help_arg[] = {
	"z = exp(i theta)",
	"arg(z)",
};

static char *help_conj[] = {
	"conj(3 + 4 i)",
};

static char *help_imag[] = {
	"imag(3 + 4 i)",
};

static char *help_mag[] = {
	"mag(3 + 4 i)",
};

static char *help_polar[] = {
	"z = 1 + sqrt(3) i",
	"polar(z)",
};

static char *help_real[] = {
	"real(3 + 4 i)",
};

static char *help_rect[] = {
	"z = exp(i theta)",
	"rect(z)",
};

// circular functions

static char *help_arccos[] = {
	"xrange=(-1,1)",
	"yrange=(0,pi)",
	"draw(arccos(x))",
};

static char *help_arcsin[] = {
	"xrange=(-1,1)",
	"yrange=(-pi/2,pi/2)",
	"draw(arcsin(x))",
};

static char *help_arctan[] = {
	"draw(arctan(x))",
};

static char *help_cos[] = {
	"draw(cos(x))",
};

static char *help_sin[] = {
	"draw(sin(x))",
};

static char *help_tan[] = {
	"xrange=(-pi,pi)",
	"draw(tan(x))",
};

// hyperbolic functions

static char *help_arccosh[] = {
	"draw(arccosh(x))",
};

static char *help_arcsinh[] = {
	"draw(arcsinh(x))",
};

static char *help_arctanh[] = {
	"draw(arctanh(x))",
};

static char *help_cosh[] = {
	"draw(cosh(x))",
};

static char *help_sinh[] = {
	"draw(sinh(x))",
};

static char *help_tanh[] = {
	"draw(tanh(x))",
};

// special functions

static char *help_besselj[] = {
	"n = 5/2",
	"y = besselj(x,n)",
	"x^2 d(y,x,x) + x d(y,x) + (x^2 - n^2) y",
};

static char *help_hermite[] = {
	"n = 7",
	"y = hermite(x,n)",
	"d(y,x,x) - 2 x d(y,x) + 2 n y",
};

static char *help_laguerre[] = {
	"n = 7",
	"y = laguerre(x,n)",
	"x d(y,x,x) + (1 - x) d(y,x) + n y",
	"y = laguerre(x,n,a)",
	"x d(y,x,x) + (a + 1 - x) d(y,x) + n y",
};

static char *help_legendre[] = {
	"n = 7",
	"y = legendre(x,n)",
	"(1 - x^2) d(y,x,x) - 2 x d(y,x) + n (n + 1) y",
};

// abs

static char *help_abs[] = {
	"abs(-1)",
	"X = (X1,X2)",
	"abs(X)",
};

// circexp

static char *help_circexp[] = {
	"f = cos(x) + sin(x)",
	"circexp(f)",
};

// coeff

static char *help_coeff[] = {
	"p = a x^2 + b x + c",
	"coeff(p,2)",
};

// cofactor

static char *help_cofactor[] = {
	"A=((1,2),(3,4))",
	"B=((0,0),(0,0))",
	"for(i,1,2,for(j,1,2,B[i,j]=cofactor(A,i,j)))",
	"adj(A)-transpose(B)",
};

// deg

static char *help_deg[] = {
	"p = a x^2 + b x + c",
	"deg(p)",
};

// denominator

static char *help_denominator[] = {
	"denominator(a/b)",
};

// erf

static char *help_erf[] = {
	"draw(erf(x))",
};

// erfc

static char *help_erfc[] = {
	"draw(erfc(x))",
};

// eval

static char *help_eval[] = {
	"f=taylor(cos(t),t,6)",
	"f",
	"eval(f,t,pi/3)",
	"float",
	"r=sqrt(x^2+y^2)",
	"eval(r,x,2,y,3)",
};

// exp

static char *help_exp[] = {
	"exp(1.0)",
};

// expcos

static char *help_expcos[] = {
	"expcos(x)",
};

// expsin

static char *help_expsin[] = {
	"expsin(x)",
};

// factor

static char *help_factor[] = {
	"factor(125 x^3 - 1)",
	"factor(10^20 - 1)",
};

// factorial

static char *help_factorial[] = {
	"100!",
	"float",
};

// for

static char *help_for[] = {
	"x=0",
	"y=2",
	"for(k,1,9,x=sqrt(2+x),y=2y/x)",
	"float(y)",
};

// log

static char *help_log[] = {
	"log(e)",
};

// numerator

static char *help_numerator[] = {
	"numerator(a/b)",
};

// product

static char *help_product[] = {
	"product(k,1,4,prime(k)^2)",
	"factor",
};

// quotient

static char *help_quotient[] = {
	"p = x^2 + x + 1",
	"q = x + 2",
	"quotient(p,q)",
};

// sqrt

static char *help_sqrt[] = {
	"sqrt(2)",
};

// sum

static char *help_sum[] = {
	"99^2 - sum(k,1,99,2k-1)",
};

// taylor

static char *help_taylor[] = {
	"taylor(cos(x),x,8)",
};

#define HELP(x) do_help(x, sizeof x / sizeof (char *))

static void do_help(char **, int);
