#include "stdafx.h"

#include "defs.h"

void
index_function(int n)
{
	int i, k, m, ndim, nelem, t;
	U **s;

	save();

	if (n < 2)
		stop("index error");

	s = stack + tos - n;

	p1 = s[0];

	// index of scalar zero OK

	if (iszero(p1)) {
		tos -= n;
		push(p1);
		restore();
		return;
	}

	if (p1->k != TENSOR)
		stop("tensor expected");

	ndim = p1->u.tensor->ndim;

	m = n - 1;

	if (m > ndim)
		stop("too many indices for tensor");

	k = 0;

	for (i = 0; i < m; i++) {
		push(s[i + 1]);
		t = pop_integer();
		if (t < 1 || t > p1->u.tensor->dim[i])
			stop("index out of range");
		k = k * p1->u.tensor->dim[i] + t - 1;
	}

	if (ndim == m) {
		tos -= n;
		push(p1->u.tensor->elem[k]);
		restore();
		return;
	}

	for (i = m; i < ndim; i++)
		k = k * p1->u.tensor->dim[i] + 0;

	nelem = 1;

	for (i = m; i < ndim; i++)
		nelem *= p1->u.tensor->dim[i];

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim - m;

	for (i = m; i < ndim; i++)
		p2->u.tensor->dim[i - m] = p1->u.tensor->dim[i];

	for (i = 0; i < nelem; i++)
		p2->u.tensor->elem[i] = p1->u.tensor->elem[k + i];

	tos -= n;
	push(p2);
	restore();
}

//-----------------------------------------------------------------------------
//
//	Input:		n		Number of args on stack
//
//			tos-n		Right-hand value
//
//			tos-n+1		Left-hand value
//
//			tos-n+2		First index
//
//			.
//			.
//			.
//
//			tos-1		Last index
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

#define LVALUE p1
#define RVALUE p2
#define TMP p3

void
set_component(int n)
{
	int i, k, m, ndim, t;
	U **s;

	save();

	if (n < 3)
		stop("error in indexed assign");

	s = stack + tos - n;

	RVALUE = s[0];

	LVALUE = s[1];

	if (LVALUE->k != TENSOR)
		stop("error in indexed assign");

	ndim = LVALUE->u.tensor->ndim;

	m = n - 2;

	if (m > ndim)
		stop("error in indexed assign");

	k = 0;

	for (i = 0; i < m; i++) {
		push(s[i + 2]);
		t = pop_integer();
		if (t < 1 || t > LVALUE->u.tensor->dim[i])
			stop("error in indexed assign\n");
		k = k * p1->u.tensor->dim[i] + t - 1;
	}

	for (i = m; i < ndim; i++)
		k = k * p1->u.tensor->dim[i] + 0;

	// copy

	TMP = alloc_tensor(LVALUE->u.tensor->nelem);

	TMP->u.tensor->ndim = LVALUE->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		TMP->u.tensor->dim[i] = LVALUE->u.tensor->dim[i];

	for (i = 0; i < p1->u.tensor->nelem; i++)
		TMP->u.tensor->elem[i] = LVALUE->u.tensor->elem[i];

	LVALUE = TMP;

	if (ndim == m) {
		if (RVALUE->k == TENSOR)
			stop("error in indexed assign");
		LVALUE->u.tensor->elem[k] = RVALUE;
		tos -= n;
		push(LVALUE);
		restore();
		return;
	}

	// see if the rvalue matches

	if (RVALUE->k != TENSOR)
		stop("error in indexed assign");

	if (ndim - m != RVALUE->u.tensor->ndim)
		stop("error in indexed assign");

	for (i = 0; i < RVALUE->u.tensor->ndim; i++)
		if (LVALUE->u.tensor->dim[m + i] != RVALUE->u.tensor->dim[i])
			stop("error in indexed assign");

	// copy rvalue

	for (i = 0; i < RVALUE->u.tensor->nelem; i++)
		LVALUE->u.tensor->elem[k + i] = RVALUE->u.tensor->elem[i];

	tos -= n;

	push(LVALUE);

	restore();
}

static char *s[] = {

	"A11=quote(A11)",
	"A11",

	"A12=quote(A12)",
	"A12",

	"A21=quote(A21)",
	"A21",

	"A22=quote(A22)",
	"A22",

	"B11=quote(B11)",
	"B11",

	"B12=quote(B12)",
	"B12",

	"B21=quote(B21)",
	"B21",

	"B22=quote(B22)",
	"B22",

	"A=((A11,A12),(A21,A22))",
	"A",

	"A[1,1]",
	"A11",

	"A[1,2]",
	"A12",

	"A[2,1]",
	"A21",

	"A[2,2]",
	"A22",

	"A[1]",
	"(A11,A12)",

	"A[2]",
	"(A21,A22)",

	"A[1]=(B11,B12)",
	"((B11,B12),(A21,A22))",

	"A",
	"((B11,B12),(A21,A22))",

	"A[2]=(B21,B22)",
	"((B11,B12),(B21,B22))",

	"A",
	"((B11,B12),(B21,B22))",

	"A=quote(A)",
	"A",
};

void
test_index(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
