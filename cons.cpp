// Cons two things on the stack.

#include "stdafx.h"
#include "defs.h"

void
cons(void)
{
	U *p; // it's ok, we don't do gc when p points to something
	p = alloc();
	p->k = CONS;
	p->u.cons.cdr = pop();
	p->u.cons.car = pop();
	push(p);
}
