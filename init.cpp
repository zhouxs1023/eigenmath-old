#include "stdafx.h"
#include "defs.h"
extern void define_symbol(char *, int);
extern void define_variable(char *, int);
extern U *varlist;
extern U symtab[];

void
init(void)
{
	if (nil)
		return;

	nil = symtab; // this must be first, get_symbol() uses nil

	get_symbol("nil"); // must be first symbol defined

	p1 = nil;
	p2 = nil;
	p3 = nil;
	p4 = nil;
	p5 = nil;
	p6 = nil;
	p7 = nil;
	p8 = nil;

	last = nil;
	table_of_integrals = nil;
	varlist = nil;

	init_alloc(); // init_alloc() uses nil too

	// built-in function names

	define_symbol("abs", ABS);
	define_symbol("add", ADD);
	define_symbol("adj", ADJ);
	define_symbol("arccos", ARCCOS);
	define_symbol("arccosh", ARCCOSH);
	define_symbol("arcsin", ARCSIN);
	define_symbol("arcsinh", ARCSINH);
	define_symbol("arctan", ARCTAN);
	define_symbol("arctanh", ARCTANH);
	define_symbol("atomize", ATOMIZE);
	define_symbol("binding2", BINDING2);
	define_symbol("binomial", BINOMIAL);
	define_symbol("break", BREAK);
	define_symbol("ceiling", CEILING);
	define_symbol("charpoly", CHARPOLY);
	define_symbol("check", CHECK);
	define_symbol("clear", CLEAR);
	define_symbol("cls", CLS);
	define_symbol("coeff", COEFF);
	define_symbol("condense", CONDENSE);
	define_symbol("conj", CONJ);
	define_symbol("contract", CONTRACT);
	define_symbol("cos", COS);
	define_symbol("cosh", COSH);
	define_symbol("degree", DEGREE);
	define_symbol("denominator", DENOMINATOR);
	define_symbol("det", DET);
	define_symbol("derivative", DERIVATIVE);
	define_symbol("dim", DIM);
	define_symbol("display", DISPLAY);
	define_symbol("divisors", DIVISORS);
	define_symbol("do", DO);
	define_symbol("dot", DOT);
	define_symbol("draw", DRAW);
	define_symbol("dsolve", DSOLVE);
	define_symbol("eigen", EIGEN);
	define_symbol("eigenval", EIGENVAL);
	define_symbol("eigenvec", EIGENVEC);
	define_symbol("eval", EVAL);
	define_symbol("exp", EXP);
	define_symbol("expand", EXPAND);
	define_symbol("expcos", EXPCOS);
	define_symbol("expsin", EXPSIN);
	define_symbol("factor", FACTOR);
	define_symbol("factorial", FACTORIAL);
	define_symbol("factorpoly", FACTORPOLY);
	define_symbol("filter", FILTER);
	define_symbol("float", FLOATF);
	define_symbol("floor", FLOOR);
	define_symbol("for", FOR);
	define_symbol("gcd", GCD);
	define_symbol("hermite", HERMITE);
	define_symbol("hilbert", HILBERT);
	define_symbol("component", INDEX);
	define_symbol("inner", INNER);
	define_symbol("integral", INTEGRAL);
	define_symbol("inv", INV);
	define_symbol("invg", INVG);
	define_symbol("isinteger", ISINTEGER);
	define_symbol("isprime", ISPRIME);
	define_symbol("laguerre", LAGUERRE);
	define_symbol("lcm", LCM);
	define_symbol("legendre", LEGENDRE);
	define_symbol("log", LOG);
	define_symbol("mod", MOD);
	define_symbol("multiply", MULTIPLY);
	define_symbol("numerator", NUMERATOR);
	define_symbol("operator", OPERATOR);
	define_symbol("outer", OUTER);
	define_symbol("power", POWER);
	define_symbol("prime", PRIME);
	define_symbol("print", PRINT);
	define_symbol("product", PRODUCT);
	define_symbol("prog", PROG);
	define_symbol("quote", QUOTE);
	define_symbol("rank", RANK);
	define_symbol("rationalize", RATIONALIZE);
	define_symbol("return", RETURN);
	define_symbol("roots", ROOTS);
	define_symbol("equals", SETQ);
	define_symbol("sample", SAMPLE);
	define_symbol("simplify", SIMPLIFY);
	define_symbol("sin", SIN);
	define_symbol("sinh", SINH);
	define_symbol("sqrt", SQRT);
	define_symbol("stop", STOP);
	define_symbol("subst", SUBST);
	define_symbol("sum", SUM);
	define_symbol("tab", TAB);
	define_symbol("tan", TAN);
	define_symbol("tanh", TANH);
	define_symbol("taylor", TAYLOR);
	define_symbol("test", TEST);
	define_symbol("testeq", TESTEQ);
	define_symbol("testge", TESTGE);
	define_symbol("testgt", TESTGT);
	define_symbol("testle", TESTLE);
	define_symbol("testlt", TESTLT);
	define_symbol("trace", TRACE);
	define_symbol("transpose", TRANSPOSE);
	define_symbol("unit", UNIT);
	define_symbol("wedge", WEDGE);
	define_symbol("zero", ZERO);

	// built-in symbols

	define_variable("autoexpand", AUTOEXPAND);
	define_variable("~exp", E); // tilde so sort puts it after scalar symbols
	define_variable("expomode", EXPOMODE);
	define_variable("tty", TTY);
	define_variable("*im", IM);
	define_variable("last", LAST);
	define_variable("pi", PI);

	define_variable("a", SYMBOL_A);
	define_variable("b", SYMBOL_B);
	define_variable("d", SYMBOL_D);
	define_variable("r", SYMBOL_R);
	define_variable("t", SYMBOL_T);
	define_variable("x", SYMBOL_X);
	define_variable("y", SYMBOL_Y);
	define_variable("z", SYMBOL_Z);

	meta_a = get_symbol("$a");
	meta_b = get_symbol("$b");
	meta_x = get_symbol("$x");

	tmp = get_symbol("$tmp");

	formal_arg[0] = get_symbol("$1");
	formal_arg[1] = get_symbol("$2");
	formal_arg[2] = get_symbol("$3");
	formal_arg[3] = get_symbol("$4");
	formal_arg[4] = get_symbol("$5");
	formal_arg[5] = get_symbol("$6");

	// if anything is added here be sure it gets untagged in gc()

	tos = 0;
	frame = stack + TOS;

	push_integer(0);
	zero = pop();

	push_integer(1);
	one = pop();

	push_symbol(POWER);
	push_integer(-1);
	push_rational(1, 2);
	list(3);
	imaginaryunit = pop();

	defn();
}

static char *defn_str[] = {
	"e=exp(1)",
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
