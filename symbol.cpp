// The symbol table is an array of structs.
//
//	U *get_symbol(char *printname)		Adds symbol if not in table.
//
//	char *get_printname(U *)

#include "stdafx.h"
#include "defs.h"

int nsym; // number of symbols in table

#define YMAX 1000
U symtab[YMAX];
static char *printname[YMAX];

U *
get_symbol(char *s)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++)
		if (strcmp(s, printname[i]) == 0)
			return symtab + i;
	if (nsym == YMAX)
		stop("symbol table overflow");
	p = symtab + nsym;
	printname[nsym] = strdup(s);
	nsym++;
	p->k = SYM;
	p->u.sym.binding = p;
	p->u.sym.binding2 = nil;
	return p;
}

char *
get_printname(U *p)
{
	int n;
	n = (int) (p - symtab);
	return printname[n];
}

// keyword functions for quickly finding system symbols

U *stab[300];

U *
symbol(int k)
{
	return stab[k];
}

void
push_symbol(int k)
{
	push(stab[k]);
}

void
define_symbol(char *s, int k)
{
	stab[k] = get_symbol(s);
	stab[k]->k = k;
}

void
define_variable(char *s, int k)
{
	stab[k] = get_symbol(s);
}
