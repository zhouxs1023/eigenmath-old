#include "stdafx.h"
#include "defs.h"

int expomode;
int trigmode;
static char errstr[24];

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
		if (symbol_index(p1) < NIL) {
			// bare keyword, eval using last result
			push(p1);
			push(symbol(LAST));
			list(2);
			eval();
		} else if (floating) {
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
		sprintf(errstr, "atom %d?", p1->k);
		stop(errstr);
		break;
	}
	if (stack[tos - 1] != symbol(NIL))
		symbol(YYLAST)->u.sym.binding = stack[tos - 1];
	restore();
}

void
eval_cons(void)
{
	if (!issymbol(car(p1))) {
		sprintf(errstr, "form %d?", car(p1)->k);
		stop(errstr);
	}
	switch (symbol_index(car(p1))) {
	case ABS:		eval_abs();		break;
	case ADD:		eval_add();		break;
	case ADJ:		eval_adj();		break;
	case AND:		eval_and();		break;
	case ARCCOS:		eval_arccos();		break;
	case ARCCOSH:		eval_arccosh();		break;
	case ARCSIN:		eval_arcsin();		break;
	case ARCSINH:		eval_arcsinh();		break;
	case ARCTAN:		eval_arctan();		break;
	case ARCTANH:		eval_arctanh();		break;
	case ARG:		eval_arg();		break;
	case ATOMIZE:		eval_atomize();		break;
	case BESSELJ:		eval_besselj();		break;
	case BESSELY:		eval_bessely();		break;
	case BINDING2:		eval_binding2();	break;
	case BINOMIAL:		eval_binomial();	break;
	case BREAK:		eval_break();		break;
	case CARAC:		eval_carac();		break;
	case CEILING:		eval_ceiling();		break;
	case CHECK:		eval_check();		break;
	case CLEAR:		eval_clear();		break;
	case CLS:		eval_cls();		break;
	case COEFF:		eval_coeff();		break;
	case COFACTOR:		eval_cofactor();	break;
	case CONDENSE:		eval_condense();	break;
	case CONJ:		eval_conj();		break;
	case CONTRACT:		eval_contract();	break;
	case CONVOLUTION:	eval_convolution();	break;
	case COS:		eval_cos();		break;
	case COSH:		eval_cosh();		break;
	case DEGREE:		eval_degree();		break;
	case DENOMINATOR:	eval_denominator();	break;
	case DERIVATIVE:	eval_derivative();	break;
	case DET:		eval_det();		break;
	case DIM:		eval_dim();		break;
	case DIRAC:		eval_dirac();		break;
	case DISPLAY:		eval_display();		break;
	case DIVISORS:		eval_divisors();	break;
	case DO:		eval_do();		break;
	case DOT:		eval_inner();		break;
	case DRAW:		eval_draw();		break;
	case DSOLVE:		eval_dsolve();		break;
	case EIGEN:		eval_eigen();		break;
	case EIGENVAL:		eval_eigenval();	break;
	case EIGENVEC:		eval_eigenvec();	break;
	case ERF:		eval_erf();		break;
	case ERFC:		eval_erfc();		break;
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
	case FOURIER:		eval_fourier();		break;
	case GAMMA:		eval_gamma();		break;
	case GCD:		eval_gcd();		break;
	case HEAVISIDE:		eval_heaviside();	break;
	case HERMITE:		eval_hermite();		break;
	case HILBERT:		eval_hilbert();		break;
	case IMAG:		eval_imag();		break;
	case INDEX:		eval_index();		break;
	case INNER:		eval_inner();		break;
	case INTEGRAL:		eval_integral();	break;
	case INV:		eval_inv();		break;
	case INVFOURIER:	eval_invfourier();	break;
	case INVG:		eval_invg();		break;
	case ISINTEGER:		eval_isinteger();	break;
	case ISPRIME:		eval_isprime();		break;
	case LAGUERRE:		eval_laguerre();	break;
	case LCM:		eval_lcm();		break;
	case LEGENDRE:		eval_legendre();	break;
	case LOG:		eval_log();		break;
	case MAG:		eval_mag();		break;
	case MOD:		eval_mod();		break;
	case MULTIPLY:		eval_multiply();	break;
	case NOT:		eval_not();		break;
	case NUMBER:		eval_number();		break;
	case NUMERATOR:		eval_numerator();	break;
	case OPERATOR:		eval_operator();	break;
	case OR:		eval_or();		break;
	case OUTER:		eval_outer();		break;
	case POWER:		eval_power();		break;
	case PRIME:		eval_prime();		break;
	case PRINT:		eval_print();		break;
	case PRODUCT:		eval_product();		break;
	case PROG:		eval_prog();		break;
	case QUOTE:		eval_quote();		break;
	case QUOTIENT:		eval_quotient();	break;
	case RANK:		eval_rank();		break;
	case RATIONALIZE:	eval_rationalize();	break;
	case REAL:		eval_real();		break;
	case YYRECT:		eval_rect();		break;
	case RETURN:		eval_return();		break;
	case ROOTS:		eval_roots();		break;
	case SETQ:		eval_setq();		break;
	case SGN:		eval_sgn();		break;
	case SIMFAC:		eval_simfac();		break;
	case SIMPLIFY:		eval_simplify();	break;
	case SIN:		eval_sin();		break;
	case SINH:		eval_sinh();		break;
	case SQRT:		eval_sqrt();		break;
	case STOP:		eval_stop();		break;
	case SUBST:		eval_subst();		break;
	case SUM:		eval_sum();		break;
	case SUMMARIZE:		eval_summarize();	break;
	case TAB:		eval_tab();		break;
	case TAN:		eval_tan();		break;
	case TANH:		eval_tanh();		break;
	case TAYLOR:		eval_taylor();		break;
	case TCHEBYCHEVT:	eval_tchebychevT();	break;
	case TCHEBYCHEVU:	eval_tchebychevU();	break;
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
	default:		eval_user_function();	break;
	}	
}

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

void
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

void
eval_break(void)
{
	push(cadr(p1));
	eval();
	break_function();
}

// checks a predicate, i.e. check(A = B)

void
eval_check(void)
{
	push(cadr(p1));
	eval_predicate();
	p1 = pop();
	if (iszero(p1))
		stop("check(arg): arg is zero");
	push_integer(1);
}

void
eval_cls(void)
{
	clear_term();
	push(symbol(NIL));
}

void
eval_conj(void)
{
	push(cadr(p1));
	eval();
	conjugate();
}

void
eval_degree(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	degree();
}

void
eval_det(void)
{
	push(cadr(p1));
	eval();
	det();
}

void
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
	if (!istensor(p2) || n < 1 || n > p2->u.tensor->ndim)
		push(p1);
	else
		push_integer(p2->u.tensor->dim[n - 1]);
}

void
eval_divisors(void)
{
	push(cadr(p1));
	eval();
	divisors();
}

void
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

void
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

void
eval_eval(void)
{
	push(cadr(p1));
	eval();
	eval();
}

void
eval_exp(void)
{
	push(cadr(p1));
	eval();
	exponential();
}

void
eval_expand(void)
{
	push(cadr(p1));
	eval();
	expand();
}

void
eval_factorial(void)
{
	push(cadr(p1));
	eval();
	factorial();
}

void
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

void
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

void
eval_for(void)
{
	push(cadr(p1));
	push(caddr(p1));
	push(cadddr(p1));
	push(caddddr(p1));
	for_function();
}

void
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

void
eval_hermite(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	hermite();
}

void
eval_hilbert(void)
{
	push(cadr(p1));
	eval();
	hilbert();
}

void
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

void
eval_inv(void)
{
	push(cadr(p1));
	eval();
	inv();
}

void
eval_invg(void)
{
	push(cadr(p1));
	eval();
	invg();
}

void
eval_isinteger(void)
{
	int n;
	push(cadr(p1));
	eval();
	p1 = pop();
	if (isrational(p1)) {
		if (isinteger(p1))
			push(one);
		else
			push(zero);
		return;
	}
	if (isdouble(p1)) {
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

void
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

void
eval_lcm(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	lcm();
}

void
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

void
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

void
eval_number(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (p1->k == NUM || p1->k == DOUBLE)
		push_integer(1);
	else
		push_integer(0);
}

void
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

void
eval_power(void)
{
	push(cadr(p1));
	eval();
	push(caddr(p1));
	eval();
	power();
}

void
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
	push(symbol(NIL));
}

void
eval_prog(void)
{
	push(cdr(p1));
	prog();
}

void
eval_quote(void)
{
	push(cadr(p1));
}

void
eval_rank(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (istensor(p1))
		push_integer(p1->u.tensor->ndim);
	else
		push(zero);
}

void
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

void
setq_indexed(void)
{
	int h;
	p4 = cadadr(p1);
	if (!issymbol(p4))
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
	p4->u.sym.binding2 = symbol(NIL);
	push(symbol(NIL));
}

void
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

	if (!issymbol(cadr(p1)))
		stop("symbol assignment: error in symbol");

	push(caddr(p1));
	eval();
	p2 = pop();
	cadr(p1)->u.sym.binding = p2;
	cadr(p1)->u.sym.binding2 = symbol(NIL);

	push(symbol(NIL));
}

void
eval_sqrt(void)
{
	push(cadr(p1));
	eval();
	push_rational(1, 2);
	power();
}

void
eval_stop(void)
{
	stop("user stop");
}

void
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

void
eval_tab(void)
{
	push(car(p1));
	push(cadr(p1));
	eval();
	list(2);
}

void
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

void
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

void
eval_noexpand(void)
{
	int x = expanding;
	expanding = 0;
	eval();
	expanding = x;
}

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

void
filter_f(void)
{
	if (car(p1) == symbol(ADD))
		filter_sum();
	else if (istensor(p1))
		filter_tensor();
	else if (find(p1, p2))
		push_integer(0);
	else
		push(p1);
}

void
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

void
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

// like eval() except "=" is evaluated as "=="

void
eval_predicate(void)
{
	save();
	p1 = pop();
	if (car(p1) == symbol(SETQ))
		eval_testeq();
	else {
		push(p1);
		eval();
	}
	restore();
}
