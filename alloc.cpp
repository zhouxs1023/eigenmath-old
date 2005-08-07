#include "stdafx.h"
#include "defs.h"

extern int nsym;
extern U symtab[];
extern U *varlist;
static void untag(U *);
static void untag_symbols(void);
static void alloc_next_block(void);

#define BLOCK_COUNT 10
#define BLOCK_SIZE 1000000
#define MEM (BLOCK_COUNT * BLOCK_SIZE)

static U mem[MEM];
static U *free_list;
int total_count;		// total number of atoms
int free_count;			// number of free atoms

void
init_alloc(void)
{
	free_list = nil;
	alloc_next_block();
}

U *
alloc(void)
{
	U *p;
	if (free_list == nil) {
		gc();
		if (free_count < total_count / 2)
			alloc_next_block();
		if (free_list == nil)
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
	int i;

	// tag everything

	for (i = 0; i < total_count; i++)
		mem[i].tag = 1;

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

	free_list = nil;
	free_count = 0;
	for (i = 0; i < total_count; i++) {
		if (mem[i].tag) {
			switch (mem[i].k) {
			case TENSOR:
				free(mem[i].u.tensor);
				break;
			case STR:
				free(mem[i].u.str);
				break;
			case NUM:
				mfree(mem[i].u.q.a);
				mfree(mem[i].u.q.b);
				break;
			}
			mem[i].k = CONS; // so no more free above
			mem[i].u.cons.cdr = free_list;
			free_list = mem + i;
			free_count++;
		}
	}
}

static void
untag_symbols(void)
{
	int i;
	for (i = 0; i < nsym; i++) {
		untag(symtab[i].u.sym.binding);
		untag(symtab[i].u.sym.binding2);
	}
}

static void
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

#if 0
int
count_freelist(void)
{
	int n;
	U *p;
	n = 0;
	p = freelist;
	while (p != nil) {
		n++;
		p = p->u.cons.cdr;
	}
	return n;
}
#endif

void
reset(void)
{
	int i;

	// clear symbols

	nsym = 0;

	// clear bignums, strings and tensors

	for (i = 0; i < total_count; i++) {
		if (mem[i].k == TENSOR)
			free(mem[i].u.tensor);
		else if (mem[i].k == STR)
			free(mem[i].u.str);
		else if (mem[i].k == NUM) {
			mfree(mem[i].u.q.a);
			mfree(mem[i].u.q.b);
		}
		mem[i].k = CONS;
	}

	total_count = 0;
	free_count = 0;

	init();

	defn();
}

static void
alloc_next_block(void)
{
	int i, j;
	if (total_count == MEM)
		return;
	j = total_count;
	total_count += BLOCK_SIZE;
	for (i = j; i < total_count - 1; i++) {
		mem[i].k = CONS; // so no free in gc
		mem[i].u.cons.cdr = mem + i + 1;
	}
	mem[total_count - 1].u.cons.cdr = free_list;
	free_list = mem + j;
	free_count += BLOCK_SIZE;
}
