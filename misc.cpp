#include "stdafx.h"

#include "defs.h"

U *minus_one_half;

U *varlist;
int symbol_level;

int
is_blank(char *s)
{
	while (*s)
		if (!isspace(*s++))
			return 0;
	return 1;
}

static U *stab[256];

U *
symbol(int n)
{
	return stab[n];
}

void
push_symbol(int k)
{
	if (k < 0 || k > 255)
		stop("internal error: push_symbol");
	push(stab[k]);
}

void
define_symbol(char *s, int k)
{
	stab[k] = new_symbol(s);
	stab[k]->k = k;
}

void
define_variable(char *s, int n)
{
	stab[n] = new_symbol(s);
}

extern U *formal_arg[6];

void
init(void)
{
	int i;

	tos = 0;

	frame = stack + TOS;

	nil = new_symbol("nil");

	nil->k = NIL;
	nil->u.sym.binding = nil;
	nil->u.sym.binding2 = nil;

	for (i = 0; i < 256; i++)
		stab[i] = nil;

	table = nil;

	varlist = nil;

	p1 = nil;
	p2 = nil;
	p3 = nil;
	p4 = nil;
	p5 = nil;
	p6 = nil;
	p7 = nil;
	p8 = nil;

	last = nil;

	init_alloc();

	define_variable("?", ZZZ);

	define_variable("autoexpand", AUTOEXPAND);
	define_variable("~e", E); // tilde so sort puts it after scalar symbols
	define_variable("expomode", EXPOMODE);
	define_variable("tty", FORMAT);
	define_variable("*im", IM);
	define_variable("last", LAST);
	define_variable("overlay", OVERLAY);
	define_variable("pencolor", PENCOLOR);
	define_variable("pi", PI);

	define_variable("trange", TRANGE);
	define_variable("xrange", XRANGE);
	define_variable("yrange", YRANGE);

	define_variable("D", SYMBOL_BIG_D);
	define_variable("Q", SYMBOL_BIG_Q);

	define_variable("d", SYMBOL_D);
	define_variable("r", SYMBOL_R);
	define_variable("t", SYMBOL_T);
	define_variable("x", SYMBOL_X);
	define_variable("y", SYMBOL_Y);
	define_variable("z", SYMBOL_Z);

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
	define_symbol("imagunit", IMAGINARYUNIT);
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

	tmp		= new_symbol("*tmp");

	a		= new_symbol("a");
	b		= new_symbol("b");
	x		= new_symbol("x");
	yya		= new_symbol("yya");
	yyb		= new_symbol("yyb");
	yyx		= new_symbol("yyx");

	formal_arg[0] = new_symbol("@1");
	formal_arg[1] = new_symbol("@2");
	formal_arg[2] = new_symbol("@3");
	formal_arg[3] = new_symbol("@4");
	formal_arg[4] = new_symbol("@5");
	formal_arg[5] = new_symbol("@6");

	push_integer(0);
	_zero = pop();

	push_integer(1);
	_one = pop();

	push_integer(-1);
	_minus_one = pop();

	push_rational(1, 2);
	plus_one_half = pop();

	push_rational(-1, 2);
	minus_one_half = pop();

	push_symbol(POWER);
	push_integer(-1);
	push_rational(1, 2);
	list(3);
	unit_imaginary = pop();

	mark_alloc();
}

void
list(int n)
{
	int i;
	push(nil);
	for (i = 0; i < n; i++)
		cons();
}

U *
new_symbol(char *s)
{
	struct symbol *p;

	p = symtbl[*s & 63];

	while (p) {
		if (strcmp(p->name, s) == 0)
			return (U *) p;
		p = p->next;
	}

	p = (struct symbol *) malloc(sizeof (struct symbol) + strlen(s) + 1);

	if (p == NULL)
		stop("malloc failure");

	p->u.k = SYM;
	p->u.tag = 0;
	strcpy(p->name, s);

	p->u.u.sym.binding = (U *) p;
	p->u.u.sym.binding2 = nil;

	p->next = symtbl[*s & 63];
	symtbl[*s & 63] = p;

	return (U *) p;
}

void
new_string(char *s)
{
	save();
	p1 = alloc();
	p1->k = STR;
	p1->u.str = strdup(s);
	push(p1);
	restore();
}

#if 0

void
solve(void)
{
	save();

	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	coeff();

	p3 = pop();

	if (length(p3) == 4) {

		_c->u.sym.binding = car(cdr(p3));
		_b->u.sym.binding = car(cdr(cdr(p3)));
		_a->u.sym.binding = car(cdr(cdr(cdr(p3))));

//		push(_tensor);

		scan("(-_b+(_b^2-4*_a*_c)^(1/2))/(2*_a)");
		eval();

		scan("(-_b-(_b^2-4*_a*_c)^(1/2))/(2*_a)");
		eval();

		list(3);

		restore();
		return;
	}

	if (length(p3) == 5) {

		_d->u.sym.binding = car(cdr(p3));
		_c->u.sym.binding = car(cdr(cdr(p3)));
		_b->u.sym.binding = car(cdr(cdr(cdr(p3))));
		_a->u.sym.binding = car(cdr(cdr(cdr(cdr(p3)))));

		scan("_b/_a");
		eval();
		_r->u.sym.binding = pop();

		scan("_c/_a");
		eval();
		_s->u.sym.binding = pop();

		scan("_d/_a");
		eval();
		_t->u.sym.binding = pop();

		scan("_s-_r^2/3");
		eval();
		_p->u.sym.binding = pop();

		scan("2*_r^3/27-_r*_s/3+_t");
		eval();
		_q->u.sym.binding = pop();

		scan("(-_q/2+(_q^2/4+_p^3/27)^(1/2))^(1/3)");
		eval();
		_u->u.sym.binding = pop();

		scan("(-_q/2-(_q^2/4+_p^3/27)^(1/2))^(1/3)");
		eval();
		_v->u.sym.binding = pop();

//		push(_tensor);

		scan("(_u+_v)-_r/3");
		eval();

		scan("(-(_u+_v)/2+i*3^(1/2)*(_u-_v)/2)-_r/3");
		eval();

		scan("(-(_u+_v)/2-i*3^(1/2)*(_u-_v)/2)-_r/3");
		eval();

		list(4);

		restore();
		return;
	}

//	push(_tensor);
	restore();
}

#endif

int
isposint(U *p)
{
	if (isinteger(p) && MSIGN(p->u.q.a) == 1)
		return 1;
	else
		return 0;
}

int
ispoly(U *p, U *x)
{
	if (find(p, x))
		return ispoly_expr(p, x);
	else
		return 0;
}

int
ispoly_expr(U *p, U *x)
{
	if (isadd(p)) {
		p = cdr(p);
		while (iscons(p)) {
			if (!ispoly_term(car(p), x))
				return 0;
			p = cdr(p);
		}
		return 1;
	} else
		return ispoly_term(p, x);
}

int
ispoly_term(U *p, U *x)
{
	if (car(p) == symbol(MULTIPLY)) {
		p = cdr(p);
		while (iscons(p)) {
			if (!ispoly_factor(car(p), x))
				return 0;
			p = cdr(p);
		}
		return 1;
	} else
		return ispoly_factor(p, x);
}

int
ispoly_factor(U *p, U *x)
{
	if (equal(p, x))
		return 1;
	if (car(p) == symbol(POWER) && equal(cadr(p), x)) {
		if (isposint(caddr(p)))
			return 1;
		else
			return 0;
	}
	if (find(p, x))
		return 0;
	else
		return 1;
}

static int find2(U *, U *);

int
find(U *p, U *q)
{
	int i;
	if (p->k == TENSOR) {
		for (i = 0; i < p->u.tensor->nelem; i++)
			if (find2(p->u.tensor->elem[i], q))
				return 1;
		return 0;
	} else
		return find2(p, q);
}

static int
find2(U *p, U *q)
{
	if (equal(p, q))
		return 1;
	while (iscons(p)) {
		if (find2(car(p), q))
			return 1;
		p = cdr(p);
	}
	return 0;
}

#if 0

int
is_unit_imaginary(U *p)
{
	if (equal(p, unit_imaginary))
		return 1;
	else
		return 0;
}

#endif

void
out_of_memory(void)
{
	stop("out of memory");
}

void
maxdim_error(void)
{
	stop("maxdim error");
}

void
append(void)
{
	int h;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
	}

	while (iscons(p2)) {
		push(car(p2));
		p2 = cdr(p2);
	}

	list(tos - h);

	restore();
}

void
push_zero_matrix(int i, int j)
{
	push(alloc_tensor(i * j));
	stack[tos - 1]->u.tensor->ndim = 2;
	stack[tos - 1]->u.tensor->dim[0] = i;
	stack[tos - 1]->u.tensor->dim[1] = j;
}

void
push_identity_matrix(int n)
{
	int i;
	push_zero_matrix(n, n);
	for (i = 0; i < n; i++)
		stack[tos - 1]->u.tensor->elem[i * n + i] = _one;
}

void
push_cars(U *p)
{
	while (iscons(p)) {
		push(car(p));
		p = cdr(p);
	}
}

void
peek(void)
{
	save();
	p1 = pop();
	push(p1);
	printline(p1);
	restore();
}

void
peek2(void)
{
	print_lisp(stack[tos - 2]);
	print_lisp(stack[tos - 1]);
}

void
cons(void)
{
	save();
	p3 = alloc();
	p2 = pop();
	p1 = pop();
	p3->k = CONS;
	p3->u.cons.car = p1;
	p3->u.cons.cdr = p2;
	push(p3);
	restore();
}

extern void run(char *);

#if 0
void
read_file(char *s)
{
	int n;
	FILE *f;
	f = fopen(s, "r");
	if (f == NULL) {
		printf("cannot open file %s\n", s);
		return;
	}
	n = (int) fread(program_buf, 1, MAX_PROGRAM_SIZE, f); // WARNING size_t
	fclose(f);
	if (n == MAX_PROGRAM_SIZE) {
		printstr("program file is too long");
		return;
	}
	program_buf[n] = 0;
	run(program_buf);
}
#endif

int
equal(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) == 0)
		return 1;
	else
		return 0;
}

int
lessp(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) < 0)
		return 1;
	else
		return 0;
}

int
sign(int n)
{
	if (n < 0)
		return -1;
	else if (n > 0)
		return 1;
	else
		return 0;
}

int
cmp_expr(U *p1, U *p2)
{
	int n;

	if (p1 == p2)
		return 0;

	if (p1 == nil)
		return -1;

	if (p2 == nil)
		return 1;

	if (isnum(p1) && isnum(p2))
		return sign(compare_numbers(p1, p2));

	if (isnum(p1))
		return -1;

	if (isnum(p2))
		return 1;

	if (isstr(p1) && isstr(p2))
		return sign(strcmp(p1->u.str, p2->u.str));

	if (isstr(p1))
		return -1;

	if (isstr(p2))
		return 1;

	if (issym(p1) && issym(p2))
		return sign(strcmp(((struct symbol *) p1)->name, ((struct symbol *) p2)->name));

	if (issym(p1))
		return -1;

	if (issym(p2))
		return 1;

	if (istensor(p1) && istensor(p2))
		return compare_tensors(p1, p2);

	if (istensor(p1))
		return -1;

	if (istensor(p2))
		return 1;

	while (iscons(p1) && iscons(p2)) {
		n = cmp_expr(car(p1), car(p2));
		if (n != 0)
			return n;
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	if (iscons(p2))
		return -1;

	if (iscons(p1))
		return 1;

	return 0;
}

int
length(U *p)
{
	int n = 0;
	while (iscons(p)) {
		p = cdr(p);
		n++;
	}
	return n;
}

char *defn_str[] = {
	"imagunit(i)",
	"autoexpand=1",
	"expomode=0",
	"overlay=0",
	"pencolor=(0,0,0)",
	"trange=(-pi,pi)",
	"xrange=(-10,10)",
	"yrange=(-10,10)",
	"last=nil",
	"tty=0",
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

static void unique_f(U *);

U *
unique(U *p)
{
	save();
	p1 = nil;
	p2 = nil;
	unique_f(p);
	if (p2 != nil)
		p1 = nil;
	p = p1;
	restore();
	return p;
}

static void
unique_f(U *p)
{
	if (p->k == SYM) {
		if (p1 == nil)
			p1 = p;
		else if (p != p1)
			p2 = p;
		return;
	}
	while (iscons(p)) {
		unique_f(car(p));
		if (p2 != nil)
			return;
		p = cdr(p);
	}
}

#if 0

void
check_endianess(void)
{
	int tmp = 1;
	if (((char *) &tmp)[0] == 1 && Y_LITTLE_ENDIAN == 0) {
		printf("Please change Y_LITTLE_ENDIAN to 1 in defs.h and recompile.\n");
		Exit(1);
	}
	if (((char *) &tmp)[0] == 0 && Y_LITTLE_ENDIAN != 0) {
		printf("Please change Y_LITTLE_ENDIAN to 0 in defs.h and recompile.\n");
		Exit(1);
	}
}

#endif

void
ssqrt(void)
{
	push_rational(1, 2);
	power();
}

void
slog(void)
{
	double d;

	save();

	p1 = pop();

	if (p1->k == DOUBLE) {
		d = p1->u.d;
		if (d < 0.0) {
			d = log(-d);
			push_double(d);
			push(unit_imaginary);
			push_symbol(PI);
			multiply();
			add();
		} else {
			d = log(d);
			push_double(d);
		}
		restore();
		return;
	}

	if (equal(p1, _one)) {
		push_integer(0);
		restore();
		return;
	}

	if (p1 == symbol(E)) {
		push(_one);
		restore();
		return;
	}

	if (car(p1) == symbol(POWER)) {
		push(caddr(p1));
		push(cadr(p1));
		slog();
		multiply();
		restore();
		return;
	}

	push_symbol(LOG);
	push(p1);
	list(2);
	restore();
}

void
conjugate(void)
{
	int tmp;
	tmp = conjugating;
	conjugating = 1;
	eval();
	conjugating = tmp;
}

void
expand(void)
{
	int x;
	x = expanding;
	expanding = 1;
	eval();
	expanding = x;
}

void
exponential(void)
{
	push_symbol(E);
	swap();
	power();
}

void
square(void)
{
	push_integer(2);
	power();
}

// save and restore symbols for formal arg lists, prog vars, etc.

int
save_symbols(int n)
{
	int h, i;

	save();

	h = tos - n;

	for (i = 0; i < n; i++) {
		p1 = stack[h + i];
		if (p1->k != SYM)
			continue;
		push(p1);
		push(p1->u.sym.binding);
		push(p1->u.sym.binding2);
	}

	list(tos - h - n);

	push(varlist);

	cons();

	varlist = pop();

	tos = h;

	restore();

	symbol_level++;

	return symbol_level - 1;
}

void
restore_symbols_f(void)
{
	save();

	p1 = varlist;

	varlist = cdr(p1);

	p1 = car(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		p2->u.sym.binding = cadr(p1);
		p2->u.sym.binding2 = caddr(p1);
		p1 = cdddr(p1);
	}

	restore();

	symbol_level--;
}

void
restore_symbols(int mark)
{
	while (symbol_level > mark)
		restore_symbols_f();
}

static int
__cmp(const void *p1, const void *p2)
{
	return cmp_expr(*((U **) p1), *((U **) p2));
}

void
sort_stack(int n)
{
	qsort(stack + tos - n, n, sizeof (U *), __cmp);
}

int
isnegativeterm(U *p)
{
	if (isnum(p))
		return isnegativenumber(p);
	else if (car(p) == symbol(MULTIPLY) && isnum(cadr(p)))
		return isnegativenumber(cadr(p));
	else
		return 0;
}

int
isimaginarynumber(U *p)
{
	if ((car(p) == symbol(MULTIPLY)
	&& length(p) == 3
	&& isnum(cadr(p))
	&& equal(caddr(p), unit_imaginary))
	|| equal(p, unit_imaginary))
		return 1;
	else 

		return 0;
}

int
iscomplexnumber(U *p)
{
	if ((car(p) == symbol(ADD)
	&& length(p) == 3
	&& isnum(cadr(p))
	&& isimaginarynumber(caddr(p)))
	|| isimaginarynumber(p))
		return 1;
	else
		return 0;
}

void
clear_mem(void)
{
	int i;
	struct symbol *p;
	for (i = 0; i < 64; i++) {
		p = symtbl[i];
		while (p) {
			if (p->u.k == SYM) {
				p->u.u.sym.binding = &p->u;
				p->u.u.sym.binding2 = nil;
			}
			p = p->next;
		}
	}
	defn();
}