/* Cofactor of a matrix component. The cofactor is the determinant of the
   matrix with row i and column j removed times (-1)^(i+j) */

#include "stdafx.h"
#include "defs.h"

void
eval_cofactor(void)
{
	int i, j, n;
	push(car(p1));
	eval();
	p2 = pop();
	if (istensor(p2) && p2->u.tensor->ndim == 2 && p2->u.tensor->dim[0] == p2->u.tensor->dim[1])
		;
	else
		stop("cofactor: 1st arg: square matrix expected");
	n = p2->u.tensor->dim[0];
	push(cadr(p1));
	eval();
	i = pop_integer();
	if (i == (int) 0x80000000)
		stop("cofactor: 2nd arg: integer expected");
	push(caddr(p1));
	eval();
	j = pop_integer();
	if (j == (int) 0x80000000)
		stop("cofactor: 3rd arg: integer expected");
	cofactor(p2, n, i, j);
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
