#include "stdafx.h"
#include "defs.h"
extern U *unique(U *);
extern void eval_abs(void);
extern void eval_arccos(void);
extern void eval_arccosh(void);
extern void eval_arcsin(void);
extern void eval_arcsinh(void);
extern void eval_arctan(void);
extern void eval_arctanh(void);
extern void eval_atomize(void);
extern void eval_binding2(void);
extern void eval_binomial(void);
extern void eval_ceiling(void);
extern void eval_clear(void);
extern void eval_condense(void);
extern void eval_contract(void);
extern void eval_cos(void);
extern void eval_cosh(void);
extern void eval_denominator(void);
extern void eval_display(void);
extern void eval_draw(void);
extern void eval_eigen(void);
extern void eval_eigenval(void);
extern void eval_eigenvec(void);
extern void eval_expcos(void);
extern void eval_expsin(void);
extern void eval_factor(void);
extern void eval_filter(void);
extern void eval_floor(void);
extern void eval_inner(void);
extern void eval_integral(void);
extern void eval_isprime(void);
extern void eval_log(void);
extern void eval_mod(void);
extern void eval_numerator(void);
extern void eval_outer(void);
extern void eval_product(void);
extern void eval_rationalize(void);
extern void eval_roots(void);
extern void eval_sample(void);
extern void eval_simplify(void);
extern void eval_sin(void);
extern void eval_sinh(void);
extern void eval_sum(void);
extern void eval_tan(void);
extern void eval_tanh(void);
extern void eval_trace(void);
extern void eval_transpose(void);
extern void eval_user_function(void);
extern void eval_writefile(void);
extern void eval_zero(void);

extern void define_user_function(void);

int expomode;
int trigmode;

void
setup(void)
{
	U *p;

	trigmode = 0;

	p = symbol(AUTOEXPAND);
	if (iszero(p->u.sym.binding))
		expanding = 0;
	else
		expanding = 1;

	p = symbol(EXPOMODE);
	if (iszero(p->u.sym.binding))
		expomode = 0;
	else
		expomode = 1;
}

static void
eval_add(void)
{
	push(cadr(p1));
	eval();
	p1 = cddr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		add();
		p1 = cdr(p1);
	}
}

static void
eval_adj(void)
{
	push(cadr(p1));
	eval();
	adj();
}

extern void break_function(void);

static void
eval_break(void)
{
	push(cadr(p1));
	eval();
	break_function();
}

static void
eval_charpoly(void)
{
	push(cadr(p1));
	eval();
	if (caddr(p1) == nil)
		push_symbol(SYMBOL_X);
	else {
		push(caddr(p1));
		eval();
	}
	charpoly();
}

// accepts an equality, i.e. check(A = B)

static void
eval_check(void)
{
	p1 = cadr(p1);
	if (car(p1) == symbol(TESTLE)
	|| car(p1) == symbol(TESTLT)
	|| car(p1) == symbol(TESTGE)
	|| car(p1) == symbol(TESTGT)) {
		push(p1);
		eval();
		push(one);
		subtract();
	} else if (car(p1) == symbol(SETQ) || car(p1) == symbol(TESTEQ)) {
		push(cadr(p1));
		eval();
		push(caddr(p1));
		eval();
		subtract();
	} else {
		push(p1);
		eval();
	}
	p1 = pop();
	if (!iszero(p1))
		stop("check stop");
	push(nil);
}

static void
eval_cls(void)
{
	clear_term();
	push(nil);
}

static void
eval_conj(void)
{
	push(cadr(p1));
	eval();
	conjugate();
}

static void
eval_coeff(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	coeff_cooked();
}

static void
eval_degree(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	degree();
}

static void
eval_derivative(void)
{
	push(cadr(p1));
	eval();
	if (caddr(p1) == nil) {
		if (find(stack[tos - 1], symbol(SYMBOL_X)))
			push_symbol(SYMBOL_X);
		else if (find(stack[tos - 1], symbol(SYMBOL_T)))
			push_symbol(SYMBOL_T);
		else if (find(stack[tos - 1], symbol(SYMBOL_R)))
			push_symbol(SYMBOL_R);
		else
			push_symbol(SYMBOL_X); // d(f()) -> d(f(),x)
		derivative();
		return;
	}
	p1 = cddr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		derivative();
		p1 = cdr(p1);
	}
}

static void
eval_det(void)
{
	push(cadr(p1));
	eval();
	det();
}

static void
eval_dim(void)
{
	int n;
	push(cadr(p1));
	eval();
	p2 = pop();
	if (iscons(cddr(p1))) {
		push(caddr(p1));
		eval();
		n = pop_integer();
	} else
		n = 1;
	if (p2->k != TENSOR || n < 1 || n > p2->u.tensor->ndim)
		push(p1);
	else
		push_integer(p2->u.tensor->dim[n - 1]);
}

static void
eval_divisors(void)
{
	push(cadr(p1));
	eval();
	divisors();
}

static void
eval_do(void)
{
	push(car(p1));
	p1 = cdr(p1);
	while (iscons(p1)) {
		pop();
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
}

static void
eval_dsolve(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	dsolve();
}

static void
eval_eval(void)
{
	push(cadr(p1));
	eval();
	eval();
}

static void
eval_exp(void)
{
	push(cadr(p1));
	eval();
	exponential();
}

static void
eval_expand(void)
{
	push(cadr(p1));
	eval();
	expand();
}

static void
eval_factorial(void)
{
	push(cadr(p1));
	eval();
	factorial();
}

static void
eval_factorpoly(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	push(car(p1));
	eval();
	factorpoly();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		factorpoly();
		p1 = cdr(p1);
	}
}

// Must do eval then second eval to handle functions that want integer args

static void
eval_float(void)
{
	int f;
	push(cadr(p1));
	eval();
	f = floating;
	floating = 1;
	eval();
	floating = f;
}

extern void for_function(void);

static void
eval_for(void)
{
	push(cadr(p1));
	push(caddr(p1));
	push(cadddr(p1));
	push(caddddr(p1));
	for_function();
}

static void
eval_gcd(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		gcd();
		p1 = cdr(p1);
	}
}

static void
eval_hermite(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	hermite();
}

static void
eval_hilbert(void)
{
	push(cadr(p1));
	eval();
	hilbert();
}

#if 0

static void
eval_identity(void)
{
	int i, n;
	push(cadr(p1));
	eval();
	n = pop_integer();
	if (n < 2) {
		push(p1);
		return;
	}
	p1 = alloc_tensor(n * n);
	p1->u.tensor->ndim = 2;
	p1->u.tensor->dim[0] = n;
	p1->u.tensor->dim[1] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[n * i + i] = one;
	push(p1);
}

#endif

extern void index_function(int);

static void
eval_index(void)
{
	int h;
	h = tos;
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
	index_function(tos - h);
}

static void
eval_inv(void)
{
	push(cadr(p1));
	eval();
	inv();
}

static void
eval_invg(void)
{
	push(cadr(p1));
	eval();
	invg();
}

static void
eval_isinteger(void)
{
	int n;
	push(cadr(p1));
	eval();
	p1 = pop();
	if (p1->k == NUM) {
		if (isinteger(p1))
			push(one);
		else
			push(zero);
		return;
	}
	if (p1->k == DOUBLE) {
		n = (int) p1->u.d;
		if (n == p1->u.d)
			push(one);
		else
			push(zero);
		return;
	}
	push_symbol(ISINTEGER);
	push(p1);
	list(2);
}

static void
eval_laguerre(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	if (iscons(cdddr(p1))) {
		push(cadddr(p1));
		eval();
	} else
		push(zero);
	laguerre();
}

static void
eval_lcm(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	lcm();
}

static void
eval_legendre(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	if (iscons(cdddr(p1))) {
		push(cadddr(p1));
		eval();
	} else
		push(zero);
	legendre();
}

static void
eval_multiply(void)
{
	push(cadr(p1));
	eval();
	p1 = cddr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		multiply();
		p1 = cdr(p1);
	}
}

static void
eval_operator(void)
{
	int h = tos;
	push_symbol(OPERATOR);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		p1 = cdr(p1);
	}
	list(tos - h);
}

static void
eval_power(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	power();
}

static void
eval_prime(void)
{
	push(cadr(p1));
	eval();
	prime();
}

extern void printstack(int);

void
eval_print(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	print(pop());
	p1 = cdr(p1);
	while (iscons(p1)) {
		printchar(' ');
		push(car(p1));
		eval();
		print(pop());
		p1 = cdr(p1);
	}
	printchar('\n');
	push(nil);
}

static void
eval_prog(void)
{
	push(cdr(p1));
	prog();
}

static void
eval_quote(void)
{
	push(cadr(p1));
}

static void
eval_rank(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (p1->k == TENSOR)
		push_integer(p1->u.tensor->ndim);
	else
		push(zero);
}

static void
eval_return(void)
{
	push(cadr(p1));
	eval();
	prog_return();
}

//-----------------------------------------------------------------------------
//
//	Example: a[1] = b
//
//	p1	*-------*-----------------------*
//		|	|			|
//		setq	*-------*-------*	b
//			|	|	|
//			index	a	1
//
//	cadadr(p1) -> a
//
//-----------------------------------------------------------------------------

extern void set_component(int);

static void
setq_indexed(void)
{
	int h;
	p4 = cadadr(p1);
	if (p4->k != SYM)
		stop("indexed assignment: error in symbol");
	h = tos;
	push(caddr(p1));
	eval();
	p2 = cdadr(p1);
	while (iscons(p2)) {
		push(car(p2));
		eval();
		p2 = cdr(p2);
	}
	set_component(tos - h);
	p3 = pop();
	p4->u.sym.binding = p3;
	p4->u.sym.binding2 = nil;
	push(nil);
}

static void
eval_setq(void)
{
	if (caadr(p1) == symbol(INDEX)) {
		setq_indexed();
		return;
	}

	if (iscons(cadr(p1))) {
		define_user_function();
		return;
	}

	if (cadr(p1)->k != SYM)
		stop("symbol assignment: error in symbol");

	push(caddr(p1));
	eval();
	p2 = pop();
	cadr(p1)->u.sym.binding = p2;
	cadr(p1)->u.sym.binding2 = nil;

	push(nil);
}

static void
eval_sqrt(void)
{
	push(cadr(p1));
	eval();
	push_rational(1, 2);
	power();
}

static void
eval_stop(void)
{
	stop("user stop");
}

static void
eval_subst(void)
{
	push(cadddr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadr(p1));
	eval();
	subst();
}

static void
eval_tab(void)
{
	push(car(p1));
	push(cadr(p1));
	eval();
	list(2);
}

static void
eval_taylor(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	push(cadddr(p1));
	eval();
	if (iscons(cddddr(p1)))
		push(caddddr(p1));
	else
		push(zero); // default expansion point
	eval();
	taylor();
}

static void
eval_test(void)
{
	p2 = cdr(p1);
	while (iscons(p2)) {
		push(car(p2));
		eval();
		p3 = pop();
		if (isnum(p3) && !iszero(p3)) {
			push(cadr(p2));
			eval();
			return;
		}
		p2 = cddr(p2);
	}
	push(p1);
}

static void
eval_testeq(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (iszero(p2))
			push(one);
		else
			push(zero);
	else
		push(p1);
}

static void
eval_testge(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(zero);
		else
			push(one);
	else
		push(p1);
}

static void
eval_testgt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	swap();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(one);
		else
			push(zero);
	else
		push(p1);
}

static void
eval_testle(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	swap();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(zero);
		else
			push(one);
	else
		push(p1);
}

static void
eval_testlt(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	subtract();
	p2 = pop();
	if (isnum(p2))
		if (isnegativenumber(p2))
			push(one);
		else
			push(zero);
	else
		push(p1);
}

static void
eval_unit(void)
{
	int i, n;
	push(cadr(p1));
	eval();
	n = pop_integer();
	if (n < 2) {
		push(p1);
		return;
	}
	p1 = alloc_tensor(n * n);
	p1->u.tensor->ndim = 2;
	p1->u.tensor->dim[0] = n;
	p1->u.tensor->dim[1] = n;
	for (i = 0; i < n; i++)
		p1->u.tensor->elem[n * i + i] = one;
	push(p1);
}

#if 0

static void eval_zero(void);

static void
eval_unit(void)
{
	int i, j, k;
	eval_zero();
	p1 = pop();
	if (p1->k != TENSOR || p1->u.tensor->ndim < 2) {
		push(p1);
		return;
	}
	for (i = 0; i < p1->u.tensor->dim[0]; i++) {
		k = 0;
		for (j = 0; j < p1->u.tensor->ndim; j++) {
			if (i >= p1->u.tensor->dim[j]) {
				k = -1;
				break;
			}
			k = k * p1->u.tensor->dim[j] + i;
		}
		if (k >= 0)
			p1->u.tensor->elem[k] = one;
	}
	push(p1);
}

#endif

static void
eval_wedge(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	if (iscons(cdddr(p1))) {
		push(cadddr(p1));
		eval();
		wedge3();
	} else
		wedge2();
}

static void eval_cons(void);

void
eval(void)
{
	save();

	p1 = pop();

	switch (p1->k) {

	case CONS:
		eval_cons();
		break;

	case NUM:
		push(p1);
		if (floating)
			bignum_float();
		break;

	case DOUBLE:
		push(p1);
		break;

	case STR:
		push(p1);
		break;

	case TENSOR:
		eval_tensor();
		break;

	case SYM:
		if (floating) {
			p1 = p1->u.sym.binding;
			if (p1 == symbol(PI))
				push_double(M_PI);
			else if (p1 == symbol(E))
				push_double(M_E);
			else
				push(p1);
		} else
			push(p1->u.sym.binding);
		break;

	default:

		// it's a keyword, use last result for arg

		push(p1);
		push(get_symbol("last")->u.sym.binding);
		list(2);
		eval();
		break;
	}

	restore();

	// save last non-nil result

	if (stack[tos - 1] != nil)
		last = stack[tos - 1];
}

static void
eval_cons(void)
{
	switch (car(p1)->k) {

	case SYM:		eval_user_function();	break;
	case ABS:		eval_abs();		break;
	case ADD:		eval_add();		break;
	case ADJ:		eval_adj();		break;
	case ARCCOS:		eval_arccos();		break;
	case ARCCOSH:		eval_arccosh();		break;
	case ARCSIN:		eval_arcsin();		break;
	case ARCSINH:		eval_arcsinh();		break;
	case ARCTAN:		eval_arctan();		break;
	case ARCTANH:		eval_arctanh();		break;
	case ATOMIZE:		eval_atomize();		break;
	case BINDING2:		eval_binding2();	break;
	case BINOMIAL:		eval_binomial();	break;
	case BREAK:		eval_break();		break;
	case CEILING:		eval_ceiling();		break;
	case CHARPOLY:		eval_charpoly();	break;
	case CHECK:		eval_check();		break;
	case CLEAR:		eval_clear();		break;
	case CLS:		eval_cls();		break;
	case COEFF:		eval_coeff();		break;
	case CONDENSE:		eval_condense();	break;
	case CONJ:		eval_conj();		break;
	case CONTRACT:		eval_contract();	break;
	case COS:		eval_cos();		break;
	case COSH:		eval_cosh();		break;
	case DEGREE:		eval_degree();		break;
	case DENOMINATOR:	eval_denominator();	break;
	case DERIVATIVE:	eval_derivative();	break;
	case DET:		eval_det();		break;
	case DIM:		eval_dim();		break;
	case DISPLAY:		eval_display();		break;
	case DIVISORS:		eval_divisors();	break;
	case DO:		eval_do();		break;
	case DOT:		eval_inner();		break;
	case DRAW:		eval_draw();		break;
	case DSOLVE:		eval_dsolve();		break;
	case EIGEN:		eval_eigen();		break;
	case EIGENVAL:		eval_eigenval();	break;
	case EIGENVEC:		eval_eigenvec();	break;
	case EVAL:		eval_eval();		break;
	case EXP:		eval_exp();		break;
	case EXPAND:		eval_expand();		break;
	case EXPCOS:		eval_expcos();		break;
	case EXPSIN:		eval_expsin();		break;
	case FACTOR:		eval_factor();		break;
	case FACTORIAL:		eval_factorial();	break;
	case FACTORPOLY:	eval_factorpoly();	break;
	case FILTER:		eval_filter();		break;
	case FLOATF:		eval_float();		break;
	case FLOOR:		eval_floor();		break;
	case FOR:		eval_for();		break;
	case GCD:		eval_gcd();		break;
	case HERMITE:		eval_hermite();		break;
	case HILBERT:		eval_hilbert();		break;
	case INDEX:		eval_index();		break;
	case INNER:		eval_inner();		break;
	case INTEGRAL:		eval_integral();	break;
	case INV:		eval_inv();		break;
	case INVG:		eval_invg();		break;
	case ISINTEGER:		eval_isinteger();	break;
	case ISPRIME:		eval_isprime();		break;
	case LAGUERRE:		eval_laguerre();	break;
	case LCM:		eval_lcm();		break;
	case LEGENDRE:		eval_legendre();	break;
	case LOG:		eval_log();		break;
	case MOD:		eval_mod();		break;
	case MULTIPLY:		eval_multiply();	break;
	case NUMERATOR:		eval_numerator();	break;
	case OPERATOR:		eval_operator();	break;
	case OUTER:		eval_outer();		break;
	case POWER:		eval_power();		break;
	case PRIME:		eval_prime();		break;
	case PRINT:		eval_print();		break;
	case PRODUCT:		eval_product();		break;
	case PROG:		eval_prog();		break;
	case QUOTE:		eval_quote();		break;
	case RANK:		eval_rank();		break;
	case RATIONALIZE:	eval_rationalize();	break;
	case RETURN:		eval_return();		break;
	case ROOTS:		eval_roots();		break;
	case SETQ:		eval_setq();		break;
	case SAMPLE:		eval_sample();		break;
	case SIMPLIFY:		eval_simplify();	break;
	case SIN:		eval_sin();		break;
	case SINH:		eval_sinh();		break;
	case SQRT:		eval_sqrt();		break;
	case STOP:		eval_stop();		break;
	case SUBST:		eval_subst();		break;
	case SUM:		eval_sum();		break;
	case TAB:		eval_tab();		break;
	case TAN:		eval_tan();		break;
	case TANH:		eval_tanh();		break;
	case TAYLOR:		eval_taylor();		break;
	case TEST:		eval_test();		break;
	case TESTEQ:		eval_testeq();		break;
	case TESTGE:		eval_testge();		break;
	case TESTGT:		eval_testgt();		break;
	case TESTLE:		eval_testle();		break;
	case TESTLT:		eval_testlt();		break;
	case TRACE:		eval_trace();		break;
	case TRANSPOSE:		eval_transpose();	break;
	case UNIT:		eval_unit();		break;
	case WEDGE:		eval_wedge();		break;
	case ZERO:		eval_zero();		break;

	default:
		stop("internal error: eval_cons");
		break;
	}
}

void
eval_noexpand(void)
{
	int x = expanding;
	expanding = 0;
	eval();
	expanding = x;
}

static char *s[] = {

	"1==1",
	"1",

	"1==2",
	"0",

	"a==b",
	"testeq(a,b)",

	"1>=1",
	"1",

	"1>=2",
	"0",

	"2>=1",
	"1",

	"a>=b",
	"testge(a,b)",

	"1>1",
	"0",

	"1>2",
	"0",

	"2>1",
	"1",

	"a>b",
	"testgt(a,b)",

	"1<=1",
	"1",

	"1<=2",
	"1",

	"2<=1",
	"0",

	"a<=b",
	"testle(a,b)",

	"1<1",
	"0",

	"1<2",
	"1",

	"2<1",
	"0",

	"a<b",
	"testlt(a,b)",
};

void
test_test(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

extern void filter(void);
static void filter_f(void);
static void filter_sum(void);
static void filter_tensor(void);

void
eval_filter(void)
{
	p1 = cdr(p1);
	push(car(p1));
	eval();
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		eval();
		filter();
		p1 = cdr(p1);
	}
}

void
filter(void)
{
	save();
	p2 = pop();
	p1 = pop();
	filter_f();
	restore();
}

static void
filter_f(void)
{
	if (car(p1) == symbol(ADD))
		filter_sum();
	else if (p1->k == TENSOR)
		filter_tensor();
	else if (find(p1, p2))
		push_integer(0);
	else
		push(p1);
}

static void
filter_sum(void)
{
	push_integer(0);
	p1 = cdr(p1);
	while (iscons(p1)) {
		push(car(p1));
		push(p2);
		filter();
		add();
		p1 = cdr(p1);
	}
}

static void
filter_tensor(void)
{
	int i, n;
	n = p1->u.tensor->nelem;
	p3 = alloc_tensor(n);
	p3->u.tensor->ndim = p1->u.tensor->ndim;
	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];
	for (i = 0; i < n; i++) {
		push(p1->u.tensor->elem[i]);
		push(p2);
		filter();
		p3->u.tensor->elem[i] = pop();
	}
	push(p3);
}
