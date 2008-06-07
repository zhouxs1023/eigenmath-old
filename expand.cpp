// Partial fraction expansion

#include "stdafx.h"
#include "defs.h"

void
eval_expand(void)
{
	push(cadr(p1));			// 1st arg
	eval();

	push(caddr(p1));		// 2nd arg
	eval();
	p2 = pop();
	if (p2 == symbol(NIL))
		guess();
	else
		push(p2);

	expand();
}

#define A p2
#define B p3
#define F p4
#define P p5
#define Q p6
#define T p7
#define X p8

void
expand(void)
{
	int n;

	save();

	X = pop();
	F = pop();

	if (istensor(F)) {
		expand_tensor();
		restore();
		return;
	}

	// if sum of terms then sum over the expansion of each term

	if (car(F) == symbol(ADD)) {
		push_integer(0);
		p1 = cdr(F);
		while (iscons(p1)) {
			push(car(p1));
			push(X);
			expand();
			add();
			p1 = cdr(p1);
		}
		restore();
		return;
	}

	// B = numerator

	push(F);
	numerator();
	B = pop();

	// A = denominator (denominator function expands)

	push(F);
	denominator();
	A = pop();

	normalize_denominator();

	// quotient

	push(B);
	push(A);
	push(X);
	divpoly();
	Q = pop();

	// remainder B = B - A * quotient(B, A)

	push(B);
	push(A);
	push(Q);
	multiply();
	subtract();
	B = pop();

	// if the remainder is zero then we're done

	if (iszero(B)) {
		push(Q);
		restore();
		return;
	}

	factor_denominator(); // A = factor(A)

	n = push_all_expansion_terms();
printf("n=%d\n", n);

	solve_numerators(n);

	expansion_denominators();

	inner();

	push(Q);
	add();

	restore();
}

void
expand_tensor(void)
{
	int i;
	push(F);
	copy_tensor();
	F = pop();
	for (i = 0; i < F->u.tensor->nelem; i++) {
		push(F->u.tensor->elem[i]);
		push(X);
		expand();
		F->u.tensor->elem[i] = pop();
	}
	push(F);
}

// Multiply A and B to remove negative exponents in A.

void
normalize_denominator(void)
{
	int i, k, n, x;

	k = factors(A);

	// find the smallest exponent

	n = 0;

	for (i = tos - k; i < tos; i++) {
		p1 = stack[i];
		if (car(p1) != symbol(POWER))
			continue;
		if (cadr(p1) != X)
			continue;
		push(caddr(p1));
		x = pop_integer();
		if (x == (int) 0x80000000)
			continue;
		if (n > x)
			n = x;
	}

	tos -= k;

	if (n >= 0)
		return;

	push(B);
	push(X);
	push_integer(-n);
	power();
	multiply();
	B = pop();

	push(A);
	push(X);
	push_integer(-n);
	power();
	multiply();
	A = pop();
}

// The correct way to factor
//
//	  2
//	6x  + 5x + 1
//
// is
//	(2x + 1) * (3x + 1)
//
// and that is how factorpoly() works.
//
// However, for this application we would prefer the factorization to be
//
//	6 * (x + 1/2) * (x + 1/3)

void
factor_denominator(void)
{
	push(A);
	push(X);
	factorpoly();
	A = pop();

	push_integer(1);

	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			A = car(p1);
			factor_denominator_1();
			p1 = cdr(p1);
		}
	} else
		factor_denominator_1();

	A = pop();
}

void
factor_denominator_1(void)
{
	int h, n;

	if (find(A, X) == 0) {
		push(A);
		multiply_noexpand();
		return;
	}

	if (car(A) == symbol(POWER)) {

		push(caddr(A));
		n = pop_integer();
		A = cadr(A);

		h = tos;	// T = coeff. of leading term
		push(A);
		push(X);
		coeff();
		T = pop();
		tos = h;

		push(T);
		push_integer(n);
		power();
		multiply_noexpand();

		push(symbol(POWER));
		push(A);
		push(T);
		divide();
		push_integer(n);
		list(3);

		multiply_noexpand();

		return;
	}

	h = tos;	// T = coeff. of leading term
	push(A);
	push(X);
	coeff();
	T = pop();
	tos = h;

	push(T);
	multiply_noexpand();

	push(A);
	push(T);
	divide();
	multiply_noexpand();
}








// Returns the number of terms pushed on the stack.

int
push_all_expansion_terms(void)
{
	int h = tos;
	if (car(A) == symbol(MULTIPLY)) {
		p1 = cdr(A);
		while (iscons(p1)) {
			F = car(p1);
			push_terms_per_factor();
			p1 = cdr(p1);
		}
	} else {
		F = A;
		push_terms_per_factor();
	}
	return tos - h;
}

// There is a pattern here...
//
//	Factor F	push        push         push          push
//
//	x		A/x         .            .             .
//
//	x^2		A/x         A/x^2        .             .
//
//	x+1		A/(x+1)     .            .             .
//
//	(x+1)^2		A/(x+1)     A/(x+1)^2    .             .
//
//	x^2+x+1		A/(x^2+x+1) Ax/(x^2+x+1) .             .
//
//	(x^2+x+1)^2	A/(x^2+x+1) Ax/(x^2+x+1) A/(x^2+x+1)^2 Ax/(x^2+x+1)^2
//
//
// For T = A/F and F = P ^ N we have
//
//	Factor F	push        push         push          push
//
//	x		T           .            .             .
//
//	x^2		TP          T            .             .
//
//	x+1		T           .            .             .
//
//	(x+1)^2		TP          T            .             .
//
//	x^2+x+1		T           TX           .             .
//
//	(x^2+x+1)^2	TP          TPX          T             TX
//
//
// Hence we want
//
//	push(T * (P ^ i) * (X ^ j))
//
// for all i, j such that
//
//	i = 0, 1, ..., N - 1
//
//	j = 0, 1, ..., deg(P) - 1

void
push_terms_per_factor(void)
{	int d, i, j, n;
	if (!find(F, X))
		return;
	trivial_divide();
	if (ispower(F)) {
		push(caddr(F));
		n = pop_integer();
		P = cadr(F);
	} else {
		n = 1;
		P = F;
	}
	push(P);
	push(X);
	degree();
	d = pop_integer();
	for (i = 0; i < n; i++) {
		for (j = 0; j < d; j++) {
			push(T);
			push(P);
			push_integer(i);
			power();
			multiply();
			push(X);
			push_integer(j);
			power();
			multiply();
		}
	}
}

// Remove factor F from A.

void
trivial_divide(void)
{
	int h;
	if (car(A) == symbol(MULTIPLY)) {
		push(p1);
		h = tos;
		p1 = cdr(A);
		while (iscons(p1)) {
			if (!equal(car(p1), F))
				push(car(p1));
			p1 = cdr(p1);
		}
		multiply_all(tos - h);
		swap();
		p1 = pop();
	} else
		push_integer(1);
	T = pop();
}

// Returns result vector in T.

void
solve_numerators(int n)
{
	int i, j;
	U **a = stack + tos - n;
	T = alloc_tensor(n * n);
	T->u.tensor->ndim = 2;
	T->u.tensor->dim[0] = n;
	T->u.tensor->dim[1] = n;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			push(a[j]);
			push(X);
			push_integer(i);
			power();
			divide();
			push(X);
			filter();
			T->u.tensor->elem[n * i + j] = pop();
		}
	}
	tos -= n;
	push(T);
	inv();
	T = alloc_tensor(n);
	T->u.tensor->ndim = 1;
	T->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++) {
		push(B);
		push(X);
		push_integer(i);
		power();
		divide();
		push(X);
		filter();
		T->u.tensor->elem[i] = pop();
	}
	push(T);
	inner();
}

void
expansion_denominators(void)
{
	int h = tos, i, n;
	if (car(A) == symbol(MULTIPLY)) {
		T = cdr(A);
		while (iscons(T)) {
			F = car(T);
			push_expansion_fractions_per_factor();
			T = cdr(T);
		}
	} else {
		F = A;
		push_expansion_fractions_per_factor();
	}
	n = tos - h;
	T = alloc_tensor(n);
	T->u.tensor->ndim = 1;
	T->u.tensor->dim[0] = n;
	for (i = 0; i < n; i++)
		T->u.tensor->elem[i] = stack[h + i];
	tos = h;
	push(T);
peek();
}

void
push_expansion_fractions_per_factor(void)
{	int d, i, j, n = 1;
	if (!find(F, X))
		return;
	if (ispower(F)) {
		push(caddr(F));
		n = pop_integer();
		F = cadr(F);
	}
	push(F);
	push(X);
	degree();
	d = pop_integer();
	for (i = n; i > 0; i--) {
		for (j = 0; j < d; j++) {
			push(F);
			push_integer(i);
			power();
			reciprocate();
			push(X);
			push_integer(j);
			power();
			multiply();
		}
	}
}

#if SELFTEST

static char *s[] = {

	// general cases

	"expand(1/(x+1)/(x+2))",
	"1/(x+1)-1/(x+2)",

	"expand((2x^3-x+2)/(x^2-2x+1))",
	"4+2*x+5/(x-1)+3/(x^2-2*x+1)",

	"expand(1/x^2/(x-1))",
	"-1/(x^2)-1/x+1/(x-1)",

	"p=5s+2",
	"",

	"q=(s+1)(s+2)^2",
	"",

	"expand(p/q)",
	"-3/(s+1)+3/(s+2)+8/(s^2+4*s+4)",

	// ensure denominators are expanded (result seems preferable that way)

	"q=(x-1)(x-2)^3",
	"",

	"expand(1/q)",
	"1/(x^3-6*x^2+12*x-8)+1/(x-2)-1/(x-1)-1/(x^2-4*x+4)",

	// fractional poles

	"expand(1/(x+1/2)/(x+1/3))",
	"6/(x+1/3)-6/(x+1/2)",

	// expand tensor

	"f=1/(x+1)/(x+2)",
	"",

	"g=1/(x+1)-1/(x+2)",
	"",

	"expand(((f,f),(f,f)))-((g,g),(g,g))",
	"((0,0),(0,0))",

	// denominator normalized?

	"expand(1/(1+1/x))",
	"1-1/(x+1)",

	// poles at zero

	"expand(1/x/(x+1))",
	"1/x-1/(x+1)",

	"expand(1/x^2/(x+1))",
	"x^(-2)-1/x+1/(x+1)",

	// other corner cases

	"expand(1/x)",
	"1/x",

	"expand(1/x^2)",
	"x^(-2)",

	"expand(1/(x^2-4x+4))",
	"1/(x^2-4*x+4)",
};

void
test_expand(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

#endif
