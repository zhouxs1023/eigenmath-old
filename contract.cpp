#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Contract tensor
//
//	Input:		tos-3		Tensor
//
//			tos-2		Index
//
//			tos-1		Index
//
//	Output:		On stack, tensor contracted across the two indices
//
//	Example:
//
//	> contract(((a,b),(c,d)),1,2)
//	a+d
//
//-----------------------------------------------------------------------------

#include "defs.h"

static void __contract(void);
static int check_args(int, int);

void
contract(void)
{
	save();
	__contract();
	restore();
}

void
__contract(void)
{
	int h, i, j, k, l, m, n, ndim, nelem;
	int ai[MAXDIM], an[MAXDIM];
	U **a, **b;

	p3 = pop();
	p2 = pop();
	p1 = pop();

	push(p2);
	l = pop_integer();

	push(p3);
	m = pop_integer();

	if (check_args(l, m) == 0) {
		push_symbol(CONTRACT);
		push(p1);
		push(p2);
		push(p3);
		list(4);
		return;
	}

	ndim = p1->u.tensor->ndim;

	l--;
	m--;

	n = p1->u.tensor->dim[l];

	// nelem is the number of elements in "b"

	nelem = 1;
	for (i = 0; i < ndim; i++)
		if (i != l && i != m)
			nelem *= p1->u.tensor->dim[i];

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim - 2;

	j = 0;
	for (i = 0; i < ndim; i++)
		if (i != l && i != m)
			p2->u.tensor->dim[j++] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	for (i = 0; i < ndim; i++) {
		ai[i] = 0;
		an[i] = p1->u.tensor->dim[i];
	}

	for (i = 0; i < nelem; i++) {
		push(_zero);
		for (j = 0; j < n; j++) {
			ai[l] = j;
			ai[m] = j;
			h = 0;
			for (k = 0; k < ndim; k++)
				h = (h * an[k]) + ai[k];
			push(a[h]);
			add();
		}
		b[i] = pop();
		for (j = ndim - 1; j >= 0; j--) {
			if (j == l || j == m)
				continue;
			if (++ai[j] < an[j])
				break;
			ai[j] = 0;
		}
	}

	if (nelem == 1)
		push(b[0]);
	else {
		push(p2);
		check_tensor();
	}
}

static int
check_args(int l, int m)
{
	int n;

	if (!istensor(p1))
		return 0;

	n = p1->u.tensor->ndim;

	if (l < 1 || l > n || m < 1 || m > n || l == m)
		return 0;
	else if (p1->u.tensor->dim[l - 1] != p1->u.tensor->dim[m - 1])
		return 0;
	else
		return 1;
}
