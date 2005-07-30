//-----------------------------------------------------------------------------
//
//	Author : philippe.billet@noos.fr
//
//	erfc(x)
//	
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"
static void yyerfc(void);

void
eval_erfc(void)
{
	push(cadr(p1));
	eval();
	yerfc();
}

void
yerfc(void)
{
	save();
	yyerfc();
	restore();
}

static void
yyerfc(void)
{
	//double d;

	p1 = pop();
#if 0
	if (p1->k == DOUBLE) {
		d = erfc(p1->u.d);
		push_double(d);
		return;
	}
#endif
	push_symbol(ERFC);
	push(p1);
	list(2);
	return;
}

static char *s[] = {

	"erfc(a)",
	"erfc(a)",
#if 0
	"float(erfc(0))",
	"1",
	
	"float(erfc(1))",
	"0.157299",
#endif
};

void
test_erfc(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
