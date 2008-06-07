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
#define C p4
#define F p5
#define P p6
#define Q p7
#define T p8
#define X p9

void
expand(void)
{
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

	// A = denominator

	push(F);
	denominator();
	A = pop();

	remove_negative_exponents();

	// Q = quotient

	push(B);
	push(A);
	push(X);
	divpoly();
	Q = pop();

	// remainder B = B - A * Q

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

	// factor A

	push(A);
	push(X);
	factorpoly();
	A = pop();

	get_expansion_C();
	get_expansion_B();
	get_expansion_A();

	if (istensor(C)) {
		push(C);
		inv();
		push(B);
		inner();
		push(A);
		inner();
	} else {
		push(B);
		push(C);
		divide();
		push(A);
		multiply();
	}

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

void
remove_negative_exponents(void)
{
	int h, i, j, k, n;

	h = tos;
	factors(A);
	factors(B);
	n = tos - h;

	// find the smallest exponent

	j = 0;
	for (i = 0; i < n; i++) {
		p1 = stack[h + i];
		if (car(p1) != symbol(POWER))
			continue;
		if (cadr(p1) != X)
			continue;
		push(caddr(p1));
		k = pop_integer();
		if (k == (int) 0x80000000)
			continue;
		if (k < j)
			j = k;
	}

	tos = h;

	if (j == 0)
		return;

	// A = A X^j

	push(A);
	push(X);
	push_integer(j);
	power();
	multiply();
	A = pop();

	// B = B X^j

	push(B);
	push(X);
	push_integer(j);
	power();
	multiply();
	B = pop();
}

// Returns the expansion coefficient matrix C.

void
get_expansion_C(void)
{
	int h, i, j, n;
	U **a;
	h = tos;
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
	n = tos - h;
	if (n == 1) {
		C = pop();
		return;
	}
	C = alloc_tensor(n * n);
	C->u.tensor->ndim = 2;
	C->u.tensor->dim[0] = n;
	C->u.tensor->dim[1] = n;
	a = stack + h;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			push(a[j]);
			push(X);
			push_integer(i);
			power();
			divide();
			push(X);
			filter();
			C->u.tensor->elem[n * i + j] = pop();
		}
	}
	tos -= n;
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

// Returns the expansion coefficient vector B.

void
get_expansion_B(void)
{
	int i, n;
	if (!istensor(C))
		return;
	n = C->u.tensor->dim[0];
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
	B = T;
}

// Returns the expansion fractions in A.

void
get_expansion_A(void)
{
	int h, i, n;
	if (!istensor(C)) {
		push(A);
		reciprocate();
		A = pop();
		return;
	}
	h = tos;
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
	A = T;
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
