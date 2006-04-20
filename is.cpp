#include "stdafx.h"
#include "defs.h"

int
iszero(U *p)
{
	int i;
	switch (p->k) {
	case NUM:
		if (MZERO(p->u.q.a))
			return 1;
		break;
	case DOUBLE:
		if (p->u.d == 0.0)
			return 1;
		break;
	case TENSOR:
		for (i = 0; i < p->u.tensor->nelem; i++)
			if (!iszero(p->u.tensor->elem[i]))
				return 0;
		return 1;
	default:
		break;
	}
	return 0;
}

int
isnegativenumber(U *p)
{
	switch (p->k) {
	case NUM:
		if (MSIGN(p->u.q.a) == -1)
			return 1;
		break;
	case DOUBLE:
		if (p->u.d < 0.0)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

int
isplusone(U *p)
{
	switch (p->k) {
	case NUM:
		if (MEQUAL(p->u.q.a, 1) && MEQUAL(p->u.q.b, 1))
			return 1;
		break;
	case DOUBLE:
		if (p->u.d == 1.0)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

int
isminusone(U *p)
{
	switch (p->k) {
	case NUM:
		if (MEQUAL(p->u.q.a, -1) && MEQUAL(p->u.q.b, 1))
			return 1;
		break;
	case DOUBLE:
		if (p->u.d == -1.0)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

int
isinteger(U *p)
{
	if (p->k == NUM && MEQUAL(p->u.q.b, 1))
		return 1;
	else
		return 0;
}

int
isnonnegativeinteger(U *p)
{
	if (isrational(p) && MEQUAL(p->u.q.b, 1) && MSIGN(p->u.q.a) == 1)
		return 1;
	else
		return 0;
}

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
	if (car(p) == symbol(ADD)) {
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

int
isnegativeterm(U *p)
{
	if (isnegativenumber(p))
		return 1;
	else if (car(p) == symbol(MULTIPLY) && isnegativenumber(cadr(p)))
		return 1;
	else
		return 0;
}

int
isimaginarynumber(U *p)
{
	if ((car(p) == symbol(MULTIPLY)
	&& length(p) == 3
	&& isnum(cadr(p))
	&& equal(caddr(p), imaginaryunit))
	|| equal(p, imaginaryunit))
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

int
iseveninteger(U *p)
{
	if (isinteger(p) && (p->u.q.a[0] & 1) == 0)
		return 1;
	else
		return 0;
}

int
isnegative(U *p)
{
	if (car(p) == symbol(ADD) && isnegativeterm(cadr(p)))
		return 1;
	else if (isnegativeterm(p))
		return 1;
	else
		return 0;
}

// returns 1 if there's a symbol somewhere

int
issymbolic(U *p)
{
	if (issymbol(p))
		return 1;
	else {
		while (iscons(p)) {
			if (issymbolic(car(p)))
				return 1;
			p = cdr(p);
		}
		return 0;
	}
}

// i.e. 2, 2^3, etc.

int
isintegerfactor(U *p)
{
	if (isinteger(p) || car(p) == symbol(POWER)
	&& isinteger(cadr(p))
	&& isinteger(caddr(p)))
		return 1;
	else
		return 0;
}

int
isoneover(U *p)
{
	if (car(p) == symbol(POWER)
	&& isminusone(caddr(p)))
		return 1;
	else
		return 0;
}

int
isfraction(U *p)
{
	if (p->k == NUM && !MEQUAL(p->u.q.b, 1))
		return 1;
	else
		return 0;
}

int
equaln(U *p, int n)
{
	switch (p->k) {
	case NUM:
		if (MEQUAL(p->u.q.a, n) && MEQUAL(p->u.q.b, 1))
			return 1;
		break;
	case DOUBLE:
		if (p->u.d == (double) n)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}

int
equalq(U *p, int a, int b)
{
	switch (p->k) {
	case NUM:
		if (MEQUAL(p->u.q.a, a) && MEQUAL(p->u.q.b, b))
			return 1;
		break;
	case DOUBLE:
		if (p->u.d == (double) a / b)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}
