#include "stdafx.h"
#include "defs.h"

extern int nsym;
extern U symtab[];

void
eval_clear(void)
{
	clear();
	push(nil);
}

void
clear(void)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++) {
		p = symtab + i;
		if (p->k == SYM) {
			p->u.sym.binding = p;
			p->u.sym.binding2 = nil;
		}
	}
	defn();
	clear_term();
}
