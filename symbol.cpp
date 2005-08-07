// The symbol table is an array of structs.

#include "stdafx.h"
#include "defs.h"
#define YMAX 1000
U symtab[YMAX];
char *printname[YMAX];
int nsym; // number of symbols in table;

void
std_symbol(char *s, int n)
{
	U *p;
	p = symtab + n;
	p->k = SYM;
	p->u.sym.binding = p;
	p->u.sym.binding2 = nil;
	printname[n] = strdup(s);
}

U *
usr_symbol(char *s)
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

void
clear_symbols(void)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++) {
		p = symtab + i;
		p->u.sym.binding = p;
		p->u.sym.binding2 = nil;
	}
}

U *
symbol(int k)
{
	return symtab + k;
}

void
push_symbol(int k)
{
	push(symtab + k);
}

int
symbol_index(U *p)
{
	return (int) (p - symtab);
}
