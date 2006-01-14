// Symbolic addition

#include "stdafx.h"
#include "defs.h"
static void add_f(void);
static void combine_terms(void);
static void parse_p1(void);
static void parse_p2(void);

void
add()
{
	if (esc_flag)
		stop("escape key stop");
	if (tos < 2)
		stop("stack underflow");
	if (isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
		add_numbers();
	else {
		save();
		add_f();
		restore();
	}
}

static void
add_f(void)
{
	int h, n;

	// pop operands

	p2 = pop();
	p1 = pop();

	h = tos;

	// is either operand nil?

	if (p1 == Nil || p2 == Nil) {
		push(Nil);
		return;
	}

	// is either operand zero?

	if (iszero(p1)) {
		push(p2);
		return;
	}

	if (iszero(p2)) {
		push(p1);
		return;
	}

	// normalize operands

	if (isadd(p1))
		p1 = cdr(p1);
	else {
		push(p1);
		list(1);
		p1 = pop();
	}

	if (isadd(p2))
		p2 = cdr(p2);
	else {
		push(p2);
		list(1);
		p2 = pop();
	}

	if (isnum(car(p1)) && isnum(car(p2))) {
		push(car(p1));
		push(car(p2));
		add_numbers();
		p7 = pop();
		if (!iszero(p7))
			push(p7);
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	parse_p1();
	parse_p2();

	while (iscons(p1) && iscons(p2)) {

		if (istensor(car(p1)) && istensor(car(p2))) {
			push(car(p1));
			push(car(p2));
			tensor_plus_tensor();
			p7 = pop();
			if (p7 != Nil) {
				push(p7);
				p1 = cdr(p1);
				p2 = cdr(p2);
				parse_p1();
				parse_p2();
				continue;
			}
		}

		switch (cmp_expr(p3, p4)) {
		case -1:
			push(car(p1));
			p1 = cdr(p1);
			parse_p1();
			break;
		case 1:
			push(car(p2));
			p2 = cdr(p2);
			parse_p2();
			break;
		case 0:
			combine_terms();
			p1 = cdr(p1);
			p2 = cdr(p2);
			parse_p1();
			parse_p2();
			break;
		default:
			stop("internal error 1");
			break;
		}
	}

	// push remaining terms, if any

	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
	}

	while (iscons(p2)) {
		push(car(p2));
		p2 = cdr(p2);
	}

	// sum of n terms

	n = tos - h;

	switch (n) {
	case 0:
		push(zero);
		break;
	case 1:
		break;
	default:
		list(n);
		p7 = pop();
		push_symbol(ADD);
		push(p7);
		cons();
		break;
	}
}

// Decompose a term into numerical and non-numerical parts.
//
// input:	car(p1)		term
//
// output:	p3		term's non-numerical factor(s)
//
//		p5		term's numerical factor (possibly 1)

static void
parse_p1(void)
{
	p3 = car(p1);
	p5 = one;
	if (car(p3) == symbol(MULTIPLY) && isnum(cadr(p3))) {
		p5 = cadr(p3);
		p3 = cddr(p3);
		if (cdr(p3) == Nil)
			p3 = car(p3);
		else {
			push_symbol(MULTIPLY);
			push(p3);
			cons();
			p3 = pop();
		}
	}
}

// Decompose a term into numerical and non-numerical parts.
//
// input:	car(p2)		term
//
// output:	p4		term's non-numerical factor(s)
//
//		p6		term's numerical factor (possibly 1)

static void
parse_p2(void)
{
	p4 = car(p2);
	p6 = one;
	if (car(p4) == symbol(MULTIPLY) && isnum(cadr(p4))) {
		p6 = cadr(p4);
		p4 = cddr(p4);
		if (cdr(p4) == Nil)
			p4 = car(p4);
		else {
			push_symbol(MULTIPLY);
			push(p4);
			cons();
			p4 = pop();
		}
	}
}

// Combine two terms by adding coefficients.
//
// input:	p4	Common factor or list of common factors.
//
//		p5	Numeric coefficient of first term.
//
//		p6	Numeric coefficient of second term.
//
// output:	((p5 + p6) * p4) on stack
//
//		If result is zero then nothing is pushed on stack.
//
//		p7 mangled
//
// test cases:	1	a-a		(result has zero coeff)
//
//		2	2*a-a		(result has unity coeff, one factor)
//
//		3	2*a*b-*a*b	(result has unity coeff, two factors)
//
//		4	a+a		(result has other coeff, one factor)
//
//		5	a*b+a*b		(result has other coeff, two factors)

static void
combine_terms(void)
{
	push(p5);
	push(p6);
	add_numbers();
	p7 = pop();
	if (iszero(p7))
		return;
	if (isplusone(p7)) {
		push(p4);
		return;
	}
	if (car(p4) == symbol(MULTIPLY)) {
		push(car(p4));
		push(p7);
		push(cdr(p4));
		cons();
		cons();
		return;
	}
	push_symbol(MULTIPLY);
	push(p7);
	push(p4);
	list(3);
}

void
addk(int k)
{
	int h, i;
	if (k == 1)
		return;
	if (k == 0) {
		push(zero);
		return;
	}
	h = tos - k;
	push(stack[h]);
	for (i = 1; i < k; i++) {
		push(stack[h + i]);
		add();
	}
	stack[h] = pop();
	tos = h + 1;
}

//-----------------------------------------------------------------------------
//
//	Symbolic subtraction
//
//	Input:		Minuend and subtrahend on stack
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
subtract(void)
{
	if (tos < 2)
		stop("stack underflow");
	if (isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
		subtract_numbers();
	else {
		negate();
		add();
	}
}
