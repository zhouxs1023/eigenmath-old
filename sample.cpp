// Sample a function, return a vector.

#include "stdafx.h"

#include "defs.h"

extern int floating;

static void sample(void);

void
eval_sample(void)
{
	int tmp;
	push(cadr(p1));
	eval();
	if (caddr(p1) == nil)
		push_integer(800); // default sample count = 800 samples
	else {
		push(caddr(p1));
		eval();
	}
	if (cadddr(p1) == nil)
		push_integer(8000); // default sampling rate = 8000 samples/sec
	else {
		push(cadddr(p1));
		eval();
	}
	tmp = floating;
	floating = 1;
	sample();
	floating = tmp;
}

static void
sample(void)
{
	int count, i, rate;
	double t;

	rate = pop_integer();

	count = pop_integer();

	p1 = pop(); // func

	if (count < 1)
		stop("sample: error in count (2nd arg)");

	if (rate < 1)
		stop("sample: error in rate (3rd arg)");

	p2 = alloc_tensor(count);

	p2->u.tensor->ndim = 1;
	p2->u.tensor->dim[0] = count;

	for (i = 0; i < count; i++) {
		t = 1.0 / (double) rate * (double) i;
		push(p1);
		push_symbol(SYMBOL_T);
		push_double(t);
		evalat();
		p2->u.tensor->elem[i] = pop();
	}

	push(p2);
}
