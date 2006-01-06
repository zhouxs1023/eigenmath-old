#include "stdafx.h"
#include "defs.h"

// up to 100 blocks of 100,000 atoms

#define M 100
#define N 100000

U *free_list;
U *mem[M];
int mcount;
int total_count;
int free_count;

void
init_alloc(void)
{
	alloc_next_block();
}

U *
alloc(void)
{
	U *p;
	if (free_count == 0) {
		gc();
		if (free_count < total_count / 2)
			alloc_next_block();
		if (free_count == 0)
			stop("atom space exhausted");
	}
	p = free_list;
	free_list = free_list->u.cons.cdr;
	free_count--;
	return p;
}

U *
alloc_tensor(int nelem)
{
	int i;
	U *p;
	p = alloc();
	p->k = TENSOR;
	p->u.tensor = (T *) malloc(sizeof (T) + nelem * sizeof (U *));
	if (p->u.tensor == NULL)
		out_of_memory();
	p->u.tensor->nelem = nelem;
	for (i = 0; i < nelem; i++)
		p->u.tensor->elem[i] = zero;
	return p;
}

// garbage collector

void
gc(void)
{
	int i, j;
	U *p;

	// tag everything

	for (i = 0; i < mcount; i++) {
		p = mem[i];
		for (j = 0; j < N; j++)
			p[j].tag = 1;
	}

	// untag what's used

	untag_symbols();

	untag(p1);
	untag(p2);
	untag(p3);
	untag(p4);
	untag(p5);
	untag(p6);
	untag(p7);
	untag(p8);

	untag(one);
	untag(zero);
	untag(imaginaryunit);

	untag(varlist);
	untag(table_of_integrals);
	untag(table_of_fourier);

	for (i = 0; i < tos; i++)
		untag(stack[i]);

	for (i = (int) (frame - stack); i < TOS; i++)
		untag(stack[i]);

	// collect everything that's still tagged

	free_count = 0;

	for (i = 0; i < mcount; i++) {
		p = mem[i];
		for (j = 0; j < N; j++) {
			if (p[j].tag == 0)
				continue;
			// still tagged so it's unused, put on free list
			switch (p[j].k) {
			case TENSOR:
				free(p[j].u.tensor);
				break;
			case STR:
				free(p[j].u.str);
				break;
			case NUM:
				mfree(p[j].u.q.a);
				mfree(p[j].u.q.b);
				break;
			}
			p[j].k = CONS; // so no double free occurs above
			p[j].u.cons.cdr = free_list;
			free_list = p + j;
			free_count++;
		}
	}
}

void
untag_symbols(void)
{
	int i;
	for (i = 0; i < nsym; i++) {
		untag(symtab[i].u.sym.binding);
		untag(symtab[i].u.sym.binding2);
	}
}

void
untag(U *p)
{
	int i;

	while (iscons(p) && p->tag == 1) {
		p->tag = 0;
		untag(p->u.cons.car);
		p = p->u.cons.cdr;
	}

	if (p->tag != 1)
		return;

	p->tag = 0;

	if (istensor(p)) {
		for (i = 0; i < p->u.tensor->nelem; i++)
			untag(p->u.tensor->elem[i]);
	}
}

// get memory for 100,000 atoms

void
alloc_next_block(void)
{
	int i;
	U *p;
	if (mcount == M)
		return;
	p = (U *) malloc(N * sizeof (struct U));
	if (p == NULL)
		return;
	mem[mcount++] = p;
	for (i = 0; i < N; i++) {
		p[i].k = CONS; // so no free in gc
		p[i].u.cons.cdr = p + i + 1;
	}
	p[N - 1].u.cons.cdr = free_list;
	free_list = p;
	free_count += N;
	total_count += N;
}
