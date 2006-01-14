// The symbol table is a simple array of struct U.

#include "stdafx.h"
#include "defs.h"
#define NSYM 1000
U symtab[NSYM];
char *printname[NSYM];
int nsym; // number of symbols in table;

// put symbol at index n

void
std_symbol(char *s, int n)
{
	U *p;
	p = symtab + n;
	p->k = SYM;
	p->u.sym.binding = p;
	p->u.sym.binding2 = Nil;
	printname[n] = s;
}

// symbol lookup, create symbol if need be

U *
usr_symbol(char *s)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++)
		if (strcmp(s, printname[i]) == 0)
			return symtab + i;
	if (nsym == NSYM)
		stop("symbol table overflow");
	p = symtab + nsym;
	printname[nsym] = strdup(s);
	nsym++;
	p->k = SYM;
	p->u.sym.binding = p;
	p->u.sym.binding2 = Nil;
	return p;
}

// get the symbol's printname

char *
get_printname(U *p)
{
	int n;
	n = (int) (p - symtab);
	return printname[n];
}

// get symbol's index from ptr

int
symbol_index(U *p)
{
	return (int) (p - symtab);
}

// get symbol's ptr from index

U *
symbol(int k)
{
	return symtab + k;
}

// push indexed symbol

void
push_symbol(int k)
{
	push(symtab + k);
}

void
clear_symbols(void)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++) {
		p = symtab + i;
		p->u.sym.binding = p;
		p->u.sym.binding2 = Nil;
	}
}
