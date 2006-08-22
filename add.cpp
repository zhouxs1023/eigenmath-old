// Symbolic addition

#include "stdafx.h"
#include "defs.h"

void
add()
{
	int h;
	if (esc_flag)
		stop("escape key stop");
	if (tos < 2)
		stop("stack underflow");
	if (isnum(stack[tos - 2]) && isnum(stack[tos - 1]))
		add_numbers();
	else {
		save();
		p2 = pop();
		p1 = pop();
		h = tos;
		push_terms(p1);
		push_terms(p2);
		yyadd(tos - h);
		restore();
	}
}

static int flag;

/* Compare terms for order, clobbers p1 and p2. */

static int
yycmp(const void *q1, const void *q2)
{
	int i, t;

	p1 = *((U **) q1);
	p2 = *((U **) q2);

	/* numbers can be combined */

	if (isnum(p1) && isnum(p2)) {
		flag = 1;
		return 0;
	}

	/* congruent tensors can be combined */

	if (istensor(p1) && istensor(p2)) {
		if (p1->u.tensor->ndim < p2->u.tensor->ndim)
			return -1;
		if (p1->u.tensor->ndim > p2->u.tensor->ndim)
			return 1;
		for (i = 0; i < p1->u.tensor->ndim; i++) {
			if (p1->u.tensor->dim[i] < p2->u.tensor->dim[i])
				return -1;
			if (p1->u.tensor->dim[i] > p2->u.tensor->dim[i])
				return 1;
		}
		flag = 1;
		return 0;
	}

	if (car(p1) == symbol(MULTIPLY)) {
		p1 = cdr(p1);
		if (isnum(car(p1))) {
			p1 = cdr(p1);
			if (cdr(p1) == symbol(NIL))
				p1 = car(p1);
		}
	}

	if (car(p2) == symbol(MULTIPLY)) {
		p2 = cdr(p2);
		if (isnum(car(p2))) {
			p2 = cdr(p2);
			if (cdr(p2) == symbol(NIL))
				p2 = car(p2);
		}
	}

	t = cmp_expr(p1, p2);

	if (t == 0)
		flag = 1;

	return t;
}

/* Add n terms, returns one expression on the stack. */

void
yyadd(int n)
{
	int h;
	U **s;

	h = tos - n;

	s = stack + h;

	while (n > 1) {

		flag = 0;

		qsort(s, n, sizeof (U *), yycmp);

		if (flag == 0)
			break;

		n = combine_terms(s, n);
	}

	tos = h + n;

	switch (n) {
	case 0:
		push_integer(0);
		break;
	case 1:
		break;
	default:
		list(n);
		p1 = pop();
		push_symbol(ADD);
		push(p1);
		cons();
		break;
	}
}

/* Compare adjacent terms in s[] and combine if possible.

	Returns the number of terms remaining in s[].

	n	number of terms in s[] initially
*/

int
combine_terms(U **s, int n)
{
	int i, j, t;

	for (i = 0; i < n - 1; i++) {

		p3 = s[i];
		p4 = s[i + 1];

		if (istensor(p3) && istensor(p4)) {
			push(p3);
			push(p4);
			tensor_plus_tensor();
			p1 = pop();
			if (p1 != symbol(NIL)) {
				s[i] = p1;
				for (j = i + 1; j < n - 1; j++)
					s[j] = s[j + 1];
				n--;
				i--;
			}
			continue;
		}

		if (istensor(p3) || istensor(p4))
			continue;

		if (isnum(p3) && isnum(p4)) {
			push(p3);
			push(p4);
			add_numbers();
			p1 = pop();
			if (iszero(p1)) {
				for (j = i; j < n - 2; j++)
					s[j] = s[j + 2];
				n -= 2;
			} else {
				s[i] = p1;
				for (j = i + 1; j < n - 1; j++)
					s[j] = s[j + 1];
				n--;
			}
			i--;
			continue;
		}

		if (isnum(p3) || isnum(p4))
			continue;

		p1 = one;
		p2 = one;

		t = 0;	// t indicates a denormalized term

		if (car(p3) == symbol(MULTIPLY)) {
			p3 = cdr(p3);
			t = 1;
			if (isnum(car(p3))) {
				p1 = car(p3);
				p3 = cdr(p3);
				if (cdr(p3) == symbol(NIL)) {
					p3 = car(p3);
					t = 0;
				}
			}
		}

		if (car(p4) == symbol(MULTIPLY)) {
			p4 = cdr(p4);
			if (isnum(car(p4))) {
				p2 = car(p4);
				p4 = cdr(p4);
				if (cdr(p4) == symbol(NIL))
					p4 = car(p4);
			}
		}

		if (!equal(p3, p4))
			continue;

		push(p1);
		push(p2);
		add_numbers();

		p1 = pop();

		if (iszero(p1)) {
			for (j = i; j < n - 2; j++)
				s[j] = s[j + 2];
			n -= 2;
			i--;
			continue;
		}

		push(p1);

		if (t) {
			push(symbol(MULTIPLY));
			push(p3);
			cons();
		} else
			push(p3);

		multiply();

		s[i] = pop();

		for (j = i + 1; j < n - 1; j++)
			s[j] = s[j + 1];

		n--;
		i--;
	}

	return n;
}

void
addk(int k)
{
	int h, i;
	U **s;

	if (k == 1)
		return;

	if (k == 0) {
		push(zero);
		return;
	}

	save();

	s = stack + tos - k;

	h = tos;

	for (i = 0; i < k; i++)
		push_terms(s[i]);

	yyadd(tos - h);

	p1 = pop();

	tos -= k;

	push(p1);

	restore();
}

void
push_terms(U *p)
{
	if (car(p) == symbol(ADD)) {
		p = cdr(p);
		while (iscons(p)) {
			push(car(p));
			p = cdr(p);
		}
	} else if (!iszero(p))
		push(p);
}

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
