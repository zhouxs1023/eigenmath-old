#include "stdafx.h"

#include "defs.h"

static void promote_tensor(void);
static int compatible(U *, U *);

//-----------------------------------------------------------------------------
//
//	Called from the "eval" module to evaluate tensor elements.
//
//	p1 points to the tensor operand.
//
//-----------------------------------------------------------------------------

void
eval_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b;

	//---------------------------------------------------------------------
	//
	//	create a new tensor for the result
	//
	//---------------------------------------------------------------------

	nelem = p1->u.tensor->nelem;

	ndim = p1->u.tensor->ndim;

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	//---------------------------------------------------------------------
	//
	//	b = eval(a)
	//
	//---------------------------------------------------------------------

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		eval();
		b[i] = pop();
	}

	//---------------------------------------------------------------------
	//
	//	push the result
	//
	//---------------------------------------------------------------------

	push(p2);

	promote_tensor();

	check_tensor();
}

//-----------------------------------------------------------------------------
//
//	Add tensors
//
//	Input:		Operands on stack
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
tensor_plus_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b, **c;

	save();

	p2 = pop();
	p1 = pop();

	// are the dimension lists equal?

	ndim = p1->u.tensor->ndim;

	if (ndim != p2->u.tensor->ndim) {
		push(nil);
		restore();
		return;
	}

	for (i = 0; i < ndim; i++)
		if (p1->u.tensor->dim[i] != p2->u.tensor->dim[i]) {
			push(nil);
			restore();
			return;
		}

	// create a new tensor for the result

	nelem = p1->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	// c = a + b

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;
	c = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		push(b[i]);
		add();
		c[i] = pop();
	}

	// push the result

	push(p3);

	check_tensor();

	restore();
}

//-----------------------------------------------------------------------------
//
//	careful not to reorder factors
//
//-----------------------------------------------------------------------------

void
tensor_times_scalar(void)
{
	int i, ndim, nelem;
	U **a, **b;

	save();

	p2 = pop();
	p1 = pop();

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(a[i]);
		push(p2);
		multiply();
		b[i] = pop();
	}

	push(p3);
	restore();
}

void
scalar_times_tensor(void)
{
	int i, ndim, nelem;
	U **a, **b;

	save();

	p2 = pop();
	p1 = pop();

	ndim = p2->u.tensor->ndim;
	nelem = p2->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p2->u.tensor->dim[i];

	a = p2->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		push(p1);
		push(a[i]);
		multiply();
		b[i] = pop();
	}

	push(p3);

	restore();
}

int
is_square_matrix(U *p)
{
	if (istensor(p) && p->u.tensor->ndim == 2 && p->u.tensor->dim[0] == p->u.tensor->dim[1])
		return 1;
	else
		return 0;
}

//-----------------------------------------------------------------------------
//
//	compute the adjunct of tos
//
//-----------------------------------------------------------------------------

void
adj(void)
{
	int i, j, n;

	save();

	p1 = pop();

	if (!is_square_matrix(p1)) {
		push_symbol(ADJ);
		push(p1);
		list(2);
		restore();
		return;
	}

	n = p1->u.tensor->dim[0];

	p2 = alloc_tensor(n * n);

	p2->u.tensor->ndim = 2;
	p2->u.tensor->dim[0] = n;
	p2->u.tensor->dim[1] = n;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			cofactor(p1, n, i, j);
			p2->u.tensor->elem[n * j + i] = pop(); /* transpose */
		}

	push(p2);

	restore();
}

void
cofactor(U *p, int n, int row, int col)
{
	int i, j;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			if (i != row && j != col)
				push(p->u.tensor->elem[n * i + j]);
	determinant(n - 1);
	if ((row + col) % 2)
		negate();
}

//-----------------------------------------------------------------------------
//
//	transpose tos-2 along indices tos-1 and tos
//
//-----------------------------------------------------------------------------

void
transpose(void)
{
	int i, j, k, l, m, ndim, nelem, t;
	int ai[MAXDIM], an[MAXDIM];
	U **a, **b;

	save();

	p3 = pop();
	p2 = pop();
	p1 = pop();

	if (!istensor(p1))
		goto dont_evaluate;

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	push(p2);
	l = pop_integer();

	push(p3);
	m = pop_integer();

	if (l < 1 || l > ndim || m < 1 || m > ndim)
		goto dont_evaluate;

	l--;
	m--;

	p2 = alloc_tensor(nelem);

	p2->u.tensor->ndim = ndim;

	for (i = 0; i < ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	p2->u.tensor->dim[l] = p1->u.tensor->dim[m];
	p2->u.tensor->dim[m] = p1->u.tensor->dim[l];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	for (i = 0; i < ndim; i++) {
		ai[i] = 0;
		an[i] = p1->u.tensor->dim[i];
	}

	//---------------------------------------------------------------------
	//
	//	copy elements from a to b
	//
	//---------------------------------------------------------------------

	for (i = 0; i < nelem; i++) {

		t = ai[l]; ai[l] = ai[m]; ai[m] = t;
		t = an[l]; an[l] = an[m]; an[m] = t;

		k = 0;
		for (j = 0; j < ndim; j++)
			k = (k * an[j]) + ai[j];

		t = ai[l]; ai[l] = ai[m]; ai[m] = t;
		t = an[l]; an[l] = an[m]; an[m] = t;

		b[k] = a[i];

		for (j = ndim - 1; j >= 0; j--) {
			if (++ai[j] < an[j])
				break;
			ai[j] = 0;
		}
	}

	push(p2);
	restore();
	return;

dont_evaluate:

	push_symbol(TRANSPOSE);
	push(p1);
	push(p2);
	push(p3);
	list(4);
	restore();
}

#if 0

//-----------------------------------------------------------------------------
//
//	tos-3		tensor
//
//	tos-2		index
//
//	tos-1		index
//
//-----------------------------------------------------------------------------

void
contract(void)
{
	int h, i, j, k, l, m, n, ndim, nelem;
	int ai[MAXDIM], an[MAXDIM];
	U **a, **b;

	save();

	p3 = pop();
	p2 = pop();
	p1 = pop();

	if (!istensor(p1))
		goto out;

	ndim = p1->u.tensor->ndim;

	push(p2);
	l = pop_integer();

	push(p3);
	m = pop_integer();

	if (l < 1 || l > ndim || m < 1 || m > ndim)
		goto out;

	if (l == m)
		goto out;

	l--;
	m--;

	n = p1->u.tensor->dim[l];

	if (n != p1->u.tensor->dim[m])
		goto out;

	//---------------------------------------------------------------------
	//
	//	nelem is the number of elements in "b"
	//
	//---------------------------------------------------------------------

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

	if (nelem == 1) {
		push(b[0]);
		restore();
		return;
	}

	push(p2);
	check_tensor();
	restore();
	return;

out:	push(_contract);
	push(p1);
	push(p2);
	push(p3);
	list(4);
	restore();
}

#endif

//-----------------------------------------------------------------------------
//
//	gradient of tensor
//
//-----------------------------------------------------------------------------

void
d_tensor_tensor(void)
{
	int i, j, ndim, nelem;
	U **a, **b, **c;

	ndim = p1->u.tensor->ndim;
	nelem = p1->u.tensor->nelem;

	if (ndim + 1 >= MAXDIM)
		goto dont_evaluate;

	p3 = alloc_tensor(nelem * p2->u.tensor->nelem);

	p3->u.tensor->ndim = ndim + 1;

	for (i = 0; i < ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	p3->u.tensor->dim[ndim] = p2->u.tensor->dim[0];

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;
	c = p3->u.tensor->elem;

	for (i = 0; i < nelem; i++) {
		for (j = 0; j < p2->u.tensor->nelem; j++) {
			push(a[i]);
			push(b[j]);
			derivative();
			c[i * p2->u.tensor->nelem + j] = pop();
		}
	}

	push(p3);

	check_tensor();

	return;

dont_evaluate:

	push_symbol(DERIVATIVE);
	push(p1);
	push(p2);
	list(3);
}

//-----------------------------------------------------------------------------
//
//	gradient of scalar
//
//-----------------------------------------------------------------------------

void
d_scalar_tensor(void)
{
	int i;
	U **a, **b;

	p3 = alloc_tensor(p2->u.tensor->nelem);

	p3->u.tensor->ndim = 1;

	p3->u.tensor->dim[0] = p2->u.tensor->dim[0];

	a = p2->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < p2->u.tensor->nelem; i++) {
		push(p1);
		push(a[i]);
		derivative();
		b[i] = pop();
	}

	push(p3);

	check_tensor();
}

//-----------------------------------------------------------------------------
//
//	Derivative of tensor
//
//-----------------------------------------------------------------------------

void
d_tensor_scalar(void)
{
	int i;
	U **a, **b;

	p3 = alloc_tensor(p1->u.tensor->nelem);

	p3->u.tensor->ndim = p1->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(a[i]);
		push(p2);
		derivative();
		b[i] = pop();
	}

	push(p3);

	check_tensor();
}

int
compare_tensors(U *p1, U *p2)
{
	int i;

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

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		if (equal(p1->u.tensor->elem[i], p2->u.tensor->elem[i]))
			continue;
		if (lessp(p1->u.tensor->elem[i], p2->u.tensor->elem[i]))
			return -1;
		else
			return 1;
	}

	return 0;
}

#if 0

// complex conjugate of tensor

void
conj_tensor(void)
{
	int i;
	U **a, **b;

	p3 = alloc_tensor(p1->u.tensor->nelem);

	p3->u.tensor->ndim = p1->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	a = p1->u.tensor->elem;
	b = p3->u.tensor->elem;

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		push(a[i]);
		sconj();
		b[i] = pop();
	}

	push(p3);
}

#endif

//-----------------------------------------------------------------------------
//
//	Replace zero tensor with scalar zero
//
//	Input:		Tensor on stack
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
check_tensor(void)
{
	int i, n;
	U **a;
	if (!istensor(stack[tos - 1]))
		return;
	n = stack[tos - 1]->u.tensor->nelem;
	a = stack[tos - 1]->u.tensor->elem;
	for (i = 0; i < n; i++)
		if (!iszero(a[i]))
			return;
	stack[tos - 1] = _zero;
}

//-----------------------------------------------------------------------------
//
//	Raise a tensor to a power
//
//	Input:		p1	tensor
//
//			p2	exponent
//
//	Output:		Result on stack
//
//-----------------------------------------------------------------------------

void
power_tensor(void)
{
	int i, k, n;

	// first and last dims must be equal

	k = p1->u.tensor->ndim - 1;

	if (p1->u.tensor->dim[0] != p1->u.tensor->dim[k]) {
		push_symbol(POWER);
		push(p1);
		push(p2);
		list(3);
		return;
	}

	push(p2);

	n = pop_integer();

	if (n == (int) 0x80000000) {
		push_symbol(POWER);
		push(p1);
		push(p2);
		list(3);
		return;
	}

	if (n == 0) {
		if (p1->u.tensor->ndim != 2)
			stop("power(tensor,0) with tensor rank not equal to 2");
		n = p1->u.tensor->dim[0];
		p1 = alloc_tensor(n * n);
		p1->u.tensor->ndim = 2;
		p1->u.tensor->dim[0] = n;
		p1->u.tensor->dim[1] = n;
		for (i = 0; i < n; i++)
			p1->u.tensor->elem[n * i + i] = _one;
		push(p1);
		return;
	}

	if (n < 0) {
		n = -n;
		push(p1);
		inv();
		p1 = pop();
	}

	push(p1);

	for (i = 1; i < n; i++) {
		push(p1);
		inner();
		if (iszero(stack[tos - 1]))
			break;
	}
}

void
copy_tensor(void)
{
	int i;

	save();

	p1 = pop();

	p2 = alloc_tensor(p1->u.tensor->nelem);

	p2->u.tensor->ndim = p1->u.tensor->ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p2->u.tensor->dim[i] = p1->u.tensor->dim[i];

	for (i = 0; i < p1->u.tensor->nelem; i++)
		p2->u.tensor->elem[i] = p1->u.tensor->elem[i];

	push(p2);

	restore();
}

// Tensors with elements that are also tensors get promoted to a higher rank.

static void
promote_tensor(void)
{
	int i, j, k, nelem, ndim;

	save();

	p1 = pop();

	if (!istensor(p1)) {
		push(p1);
		restore();
		return;
	}

	p2 = p1->u.tensor->elem[0];

	for (i = 1; i < p1->u.tensor->nelem; i++)
		if (!compatible(p2, p1->u.tensor->elem[i]))
			stop("Cannot promote tensor due to inconsistent tensor components.");

	if (!istensor(p2)) {
		push(p1);
		restore();
		return;
	}

	ndim = p1->u.tensor->ndim + p2->u.tensor->ndim;

	if (ndim > MAXDIM)
		stop("tensor rank > 24");

	nelem = p1->u.tensor->nelem * p2->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	for (j = 0; j < p2->u.tensor->ndim; j++)
		p3->u.tensor->dim[i + j] = p2->u.tensor->dim[j];

	k = 0;

	for (i = 0; i < p1->u.tensor->nelem; i++) {
		p2 = p1->u.tensor->elem[i];
		for (j = 0; j < p2->u.tensor->nelem; j++)
			p3->u.tensor->elem[k++] = p2->u.tensor->elem[j];
	}

	push(p3);

	restore();
}

static int
compatible(U *p, U *q)
{
	int i;

	if (!istensor(p) && !istensor(q))
		return 1;

	if (!istensor(p) || !istensor(q))
		return 0;

	if (p->u.tensor->ndim != q->u.tensor->ndim)
		return 0;

	for (i = 0; i < p->u.tensor->ndim; i++)
		if (p->u.tensor->dim[i] != q->u.tensor->dim[i])
			return 0;

	return 1;
}

static char *s[] = {

	"#test_tensor",

	"a=(1,2,3)",
	"",

	"b=(4,5,6)",
	"",

	"c=(7,8,9)",
	"",

	"rank((a,b,c))",
	"2",

	"(a,b,c)",
	"((1,2,3),(4,5,6),(7,8,9))",

	"a=quote(a)",
	"",

	"b=quote(b)",
	"",

	"c=quote(c)",
	"",
};

void
test_tensor(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}

