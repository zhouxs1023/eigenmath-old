#include "stdafx.h"
#include "defs.h"
extern U *varlist;
extern U symtab[];
extern int nsym;

void
init(void)
{
	if (nil)
		return;		// already initted

	// endianess

	little_endian = 1;
	little_endian = *((unsigned char *) &little_endian);

	nsym = USR_SYMBOLS;

	nil = symtab + NIL;

	p1 = nil;
	p2 = nil;
	p3 = nil;
	p4 = nil;
	p5 = nil;
	p6 = nil;
	p7 = nil;
	p8 = nil;

	varlist = nil;

	table_of_integrals = nil;
	table_of_fourier = nil;

	init_alloc();

	std_symbol("abs", ABS);
	std_symbol("add", ADD);
	std_symbol("adj", ADJ);
	std_symbol("and", AND);
	std_symbol("arccos", ARCCOS);
	std_symbol("arccosh", ARCCOSH);
	std_symbol("arcsin", ARCSIN);
	std_symbol("arcsinh", ARCSINH);
	std_symbol("arctan", ARCTAN);
	std_symbol("arctanh", ARCTANH);
	std_symbol("arg", ARG);
	std_symbol("atomize", ATOMIZE);
	std_symbol("besselj", BESSELJ);
	std_symbol("bessely", BESSELY);
	std_symbol("binding2", BINDING2);
	std_symbol("binomial", BINOMIAL);
	std_symbol("break", BREAK);
	std_symbol("carac", CARAC);
	std_symbol("ceiling", CEILING);
	std_symbol("charpoly", CHARPOLY);
	std_symbol("check", CHECK);
	std_symbol("clear", CLEAR);
	std_symbol("cls", CLS);
	std_symbol("coeff", COEFF);
	std_symbol("cofactor", COFACTOR);
	std_symbol("condense", CONDENSE);
	std_symbol("conj", CONJ);
	std_symbol("contract", CONTRACT);
	std_symbol("convolution", CONVOLUTION);
	std_symbol("cos", COS);
	std_symbol("cosh", COSH);
	std_symbol("degree", DEGREE);
	std_symbol("denominator", DENOMINATOR);
	std_symbol("det", DET);
	std_symbol("derivative", DERIVATIVE);
	std_symbol("dim", DIM);
	std_symbol("dirac", DIRAC);
	std_symbol("display", DISPLAY);
	std_symbol("divisors", DIVISORS);
	std_symbol("do", DO);
	std_symbol("dot", DOT);
	std_symbol("draw", DRAW);
	std_symbol("dsolve", DSOLVE);
	std_symbol("erf", ERF);
	std_symbol("erfc", ERFC);
	std_symbol("eigen", EIGEN);
	std_symbol("eigenval", EIGENVAL);
	std_symbol("eigenvec", EIGENVEC);
	std_symbol("eval", EVAL);
	std_symbol("exp", EXP);
	std_symbol("expand", EXPAND);
	std_symbol("expcos", EXPCOS);
	std_symbol("expsin", EXPSIN);
	std_symbol("factor", FACTOR);
	std_symbol("factorial", FACTORIAL);
	std_symbol("factorpoly", FACTORPOLY);
	std_symbol("filter", FILTER);
	std_symbol("float", FLOATF);
	std_symbol("floor", FLOOR);
	std_symbol("for", FOR);
	std_symbol("fourier", FOURIER);
	std_symbol("Gamma", GAMMA);
	std_symbol("gcd", GCD);
	std_symbol("heaviside", HEAVISIDE);
	std_symbol("hermite", HERMITE);
	std_symbol("hilbert", HILBERT);
	std_symbol("imag", IMAG);
	std_symbol("component", INDEX);
	std_symbol("inner", INNER);
	std_symbol("integral", INTEGRAL);
	std_symbol("inv", INV);
	std_symbol("invfourier", INVFOURIER);
	std_symbol("invg", INVG);
	std_symbol("isinteger", ISINTEGER);
	std_symbol("isprime", ISPRIME);
	std_symbol("laguerre", LAGUERRE);
	std_symbol("lcm", LCM);
	std_symbol("legendre", LEGENDRE);
	std_symbol("log", LOG);
	std_symbol("mag", MAG);
	std_symbol("mod", MOD);
	std_symbol("multiply", MULTIPLY);
	std_symbol("not", NOT);
	std_symbol("numerator", NUMERATOR);
	std_symbol("operator", OPERATOR);
	std_symbol("or", OR);
	std_symbol("outer", OUTER);
	std_symbol("power", POWER);
	std_symbol("prime", PRIME);
	std_symbol("print", PRINT);
	std_symbol("product", PRODUCT);
	std_symbol("prog", PROG);
	std_symbol("quote", QUOTE);
	std_symbol("rank", RANK);
	std_symbol("rationalize", RATIONALIZE);
	std_symbol("real", REAL);
	std_symbol("rect", YYRECT);
	std_symbol("return", RETURN);
	std_symbol("roots", ROOTS);
	std_symbol("equals", SETQ);
	std_symbol("sgn", SGN);
	std_symbol("simfac", SIMFAC);
	std_symbol("simplify", SIMPLIFY);
	std_symbol("sin", SIN);
	std_symbol("sinh", SINH);
	std_symbol("sqrt", SQRT);
	std_symbol("stop", STOP);
	std_symbol("subst", SUBST);
	std_symbol("sum", SUM);
	std_symbol("summarize", SUMMARIZE);
	std_symbol("tab", TAB);
	std_symbol("tan", TAN);
	std_symbol("tanh", TANH);
	std_symbol("taylor", TAYLOR);
	std_symbol("tchebychevT", TCHEBYCHEVT);
	std_symbol("tchebychevU", TCHEBYCHEVU);
	std_symbol("test", TEST);
	std_symbol("testeq", TESTEQ);
	std_symbol("testge", TESTGE);
	std_symbol("testgt", TESTGT);
	std_symbol("testle", TESTLE);
	std_symbol("testlt", TESTLT);
	std_symbol("trace", TRACE);
	std_symbol("transpose", TRANSPOSE);
	std_symbol("unit", UNIT);
	std_symbol("wedge", WEDGE);
	std_symbol("zero", ZERO);

	std_symbol("nil", NIL);

	std_symbol("autoexpand", AUTOEXPAND);
	std_symbol("expomode", EXPOMODE);
	std_symbol("last", LAST);
	std_symbol("tty", TTY);
	std_symbol("void", YYVOID);
	std_symbol("~", YYE); // tilde so sort puts it after other symbols
	std_symbol("", YYLAST);
	std_symbol("true", YYTRUE);
	std_symbol("false", YYFALSE);

	std_symbol("pi", PI);
	std_symbol("a", SYMBOL_A);
	std_symbol("b", SYMBOL_B);
	std_symbol("c", SYMBOL_C);
	std_symbol("d", SYMBOL_D);
	std_symbol("n", SYMBOL_N);
	std_symbol("r", SYMBOL_R);
	std_symbol("t", SYMBOL_T);
	std_symbol("x", SYMBOL_X);
	std_symbol("y", SYMBOL_Y);
	std_symbol("z", SYMBOL_Z);

	std_symbol("$C1", C1);
	std_symbol("$C2", C2);
	std_symbol("$C3", C3);
	std_symbol("$C4", C4);
	std_symbol("$C5", C5);
	std_symbol("$C6", C6);

	meta_a = usr_symbol("$a");
	meta_b = usr_symbol("$b");
	meta_c = usr_symbol("$c");
	meta_n = usr_symbol("$n");
	meta_x = usr_symbol("$x");

	tmp = usr_symbol("$tmp");

	formal_arg[0] = usr_symbol("$1");
	formal_arg[1] = usr_symbol("$2");
	formal_arg[2] = usr_symbol("$3");
	formal_arg[3] = usr_symbol("$4");
	formal_arg[4] = usr_symbol("$5");
	formal_arg[5] = usr_symbol("$6");

	// set up for evaluation

	tos = 0;
	frame = stack + TOS;

	push_integer(0);
	zero = pop();		// must be untagged in gc

	push_integer(1);
	one = pop();		// must be untagged in gc

	push_symbol(POWER);
	push_integer(-1);
	push_rational(1, 2);
	list(3);
	imaginaryunit = pop();	// must be untagged in gc

	defn();
}

static char *defn_str[] = {
	"i=sqrt(-1)",
	"autoexpand=1",
	"expomode=0",
	"trange=(-pi,pi)",
	"xrange=(-10,10)",
	"yrange=(-10,10)",
	"last=nil",
	"tty=0",
	"curl(v)=(d(v[3],y)-d(v[2],z),d(v[1],z)-d(v[3],x),d(v[2],x)-d(v[1],y))",
	"div(v)=d(v[1],x)+d(v[2],y)+d(v[3],z)",
	"real(z)=(z+conj(z))/2",
	"imag(z)=(z-conj(z))/(2i)",
};

void
defn(void)
{
	int i, n;
	n = sizeof defn_str / sizeof (char *);
	for (i = 0; i < n; i++) {
		scan(defn_str[i]);
		eval();
		pop();
	}
}
