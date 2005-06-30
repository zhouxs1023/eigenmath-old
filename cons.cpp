// Cons two things on the stack.

#include "stdafx.h"
#include "defs.h"

void
cons(void)
{
	U *p; // auto var ok, no garbage collection is done here
	p = alloc();
	p->k = CONS;
	p->u.cons.cdr = pop();
	p->u.cons.car = pop();
	push(p);
}
