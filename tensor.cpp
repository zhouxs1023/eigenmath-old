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
//	outer product of tos and tos-1
//
//	the args themselves may also be outer products
//
//-----------------------------------------------------------------------------

static void __outer(void);

void
outer(void)
{
	int h, n;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	if (car(p1) == symbol(OUTER)) {
		push_cars(cdr(p1));
		p1 = pop();
	}

	p3 = p2;

	if (car(p2) == symbol(OUTER))
		p2 = cadr(p2);

	push(p1);
	push(p2);

	if (istensor(p1) && istensor(p2))
		__outer();
	else if (isnum(p1) && istensor(p2))
		scalar_times_tensor();
	else if (istensor(p1) && isnum(p2))
		tensor_times_scalar();

	if (car(p3) == symbol(OUTER))
		push_cars(cddr(p3));

	n = tos - h;

	if (n > 1) {
		list(n);
		p1 = pop();
		push_symbol(OUTER);
		push(p1);
		cons();
	}

	restore();
}

//-----------------------------------------------------------------------------
//
//	compute the outer product to tos-1 and tos
//
//	both args are tensors
//
//-----------------------------------------------------------------------------

static void
__outer(void)
{
	int i, j, k, ndim, nelem;

	save();

	p2 = pop();
	p1 = pop();

	ndim = p1->u.tensor->ndim + p2->u.tensor->ndim;

	if (ndim > MAXDIM)
		maxdim_error();

	nelem = p1->u.tensor->nelem * p2->u.tensor->nelem;

	p3 = alloc_tensor(nelem);

	p3->u.tensor->ndim = ndim;

	for (i = 0; i < p1->u.tensor->ndim; i++)
		p3->u.tensor->dim[i] = p1->u.tensor->dim[i];

	j = i;

	for (i = 0; i < p2->u.tensor->ndim; i++)
		p3->u.tensor->dim[j + i] = p2->u.tensor->dim[i];

	k = 0;

	for (i = 0; i < p1->u.tensor->nelem; i++)
		for (j = 0; j < p2->u.tensor->nelem; j++) {
			push(p1->u.tensor->elem[i]);
			push(p2->u.tensor->elem[j]);
			multiply();
			p3->u.tensor->elem[k++] = pop();
		}

	push(p3);

	restore();
}

//-----------------------------------------------------------------------------
//
//	Inner product (aka dot product)
//
//	Input:		Two operands on the stack
//
//	Output:		Result on stack
//
//	Note:
//
//	The operands themselves may be inner products.
//
//	The code is designed to handle the following case:
//
//		inner(A,B,C,D)
//
//	If B and C evaluate to tensors but A and D are symbols, then the result
//	should be
//
//		inner(A,R,D)
//
//	where R is the inner product of B and C.
//
//-----------------------------------------------------------------------------

static void __inner(void);

void
inner(void)
{
	int h, n;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	if (car(p1) == symbol(DOT)) {
		push_cars(cdr(p1));
		p1 = pop();
	}

	p3 = p2;

	if (car(p2) == symbol(DOT))
		p2 = cadr(p2);

	push(p1);
	push(p2);

	if (istensor(p1) && istensor(p2))
		__inner();
	else if (isnum(p1) && istensor(p2))
		scalar_times_tensor();
	else if (istensor(p1) && isnum(p2))
		tensor_times_scalar();

	if (car(p3) == symbol(DOT))
		push_cars(cddr(p3));

	n = tos - h;

	if (n > 1) {
		list(n);
		p1 = pop();
		push_symbol(DOT);
		push(p1);
		cons();
	}

	check_tensor();

	restore();
}

//-----------------------------------------------------------------------------
//
//	compute the dot product of tos-1 and tos
//
//	both args are tensors
//
//-----------------------------------------------------------------------------

static void
__inner(void)
{
	int ak, bk, i, j, k, n, ndim;
	U **a, **b, **c;

	save();

	p2 = pop();
	p1 = pop();

	n = p1->u.tensor->dim[p1->u.tensor->ndim - 1];

	ndim = p1->u.tensor->ndim + p2->u.tensor->ndim - 2;

	if (n != p2->u.tensor->dim[0] || ndim >= MAXDIM) {
		push_symbol(DOT);
		push(p1);
		push(p2);
		list(3);
		restore();
		return;
	}

	a = p1->u.tensor->elem;
	b = p2->u.tensor->elem;

	//---------------------------------------------------------------------
	//
	//	ak is the number of rows in tensor A
	//
	//	bk is the number of columns in tensor B
	//
	//	Example:
	//
	//	A[3][3][4] B[4][4][3]
	//
	//	  3  3				ak = 3 * 3 = 9
	//
	//	                4  3		bk = 4 * 3 = 12
	//
	//---------------------------------------------------------------------

	ak = 1;
	for (i = 0; i < p1->u.tensor->ndim - 1; i++)
		ak *= p1->u.tensor->dim[i];

	bk = 1;
	for (i = 1; i < p2->u.tensor->ndim; i++)
		bk *= p2->u.tensor->dim[i];

	p3 = alloc_tensor(ak * bk);

	c = p3->u.tensor->elem;

	// new method copied from ginac
#if 1
	for (i = 0; i < ak; i++) {
		for (j = 0; j < n; j++) {
			if (iszero(a[i * n + j]))
				continue;
			for (k = 0; k < bk; k++) {
				push(a[i * n + j]);
				push(b[j * bk + k]);
				multiply();
				push(c[i * bk + k]);
				add();
				c[i * bk + k] = pop();
			}
		}
	}
#else
	for (i = 0; i < ak; i++) {
		for (j = 0; j < bk; j++) {
			push(_zero);
			for (k = 0; k < n; k++) {
				push(a[i * n + k]);
				push(b[k * bk + j]);
				multiply();
				add();
			}
			c[i * bk + j] = pop();
		}
	}
#endif
	//---------------------------------------------------------------------
	//
	//	Note on understanding "k * bk + j"
	//
	//	k * bk because each element of a column is bk locations apart
	//
	//	+ j because the beginnings of all columns are in the first bk 
	//	locations
	//
	//	Example: n = 2, bk = 6
	//
	//	b111	<- 1st element of 1st column
	//	b112	<- 1st element of 2nd column
	//	b113	<- 1st element of 3rd column
	//	b121	<- 1st element of 4th column
	//	b122	<- 1st element of 5th column
	//	b123	<- 1st element of 6th column
	//
	//	b211	<- 2nd element of 1st column
	//	b212	<- 2nd element of 2nd column
	//	b213	<- 2nd element of 3rd column
	//	b221	<- 2nd element of 4th column
	//	b222	<- 2nd element of 5th column
	//	b223	<- 2nd element of 6th column
	//
	//---------------------------------------------------------------------

	if (ndim == 0)
		push(p3->u.tensor->elem[0]);
	else {
		p3->u.tensor->ndim = ndim;
		for (i = 0; i < p1->u.tensor->ndim - 1; i++)
			p3->u.tensor->dim[i] = p1->u.tensor->dim[i];
		j = i;
		for (i = 0; i < p2->u.tensor->ndim - 1; i++)
			p3->u.tensor->dim[j + i] = p2->u.tensor->dim[i + 1];
		push(p3);
	}

	restore();
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

#if 0

void
component(int n)
{
	int i, k, m, ndim, nelem, t;
	U **s;

	if (n == 0) {
		push(_index);
		return;
	}

	save();

	s = stack + tos - n;

	p1 = s[0];

	if (!istensor(p1))
		goto dont_evaluate;

	ndim = p1->u.tensor->ndim;

	m = n - 1;

	if (m > ndim)
		goto dont_evaluate;

	k = 0;

	for (i = 0; i < m; i++) {
		push(s[i + 1]);
		t = pop_integer();
		if (t < 0 || t >= p1->u.tensor->dim[i])
			goto dont_evaluate;
		k = k * p1->u.tensor->dim[i] + t;
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
	return;

dont_evaluate:

	list(n);
	p1 = pop();
	push(_component);
	push(p1);
	cons();
	restore();
}

#endif

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