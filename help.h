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

#define HELP(x) do_help(x, sizeof x / sizeof (char *))

static void do_help(char **, int);
