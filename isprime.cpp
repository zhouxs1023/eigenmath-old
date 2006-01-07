#include "stdafx.h"
#include "defs.h"

void
eval_isprime(void)
{
	push(cadr(p1));
	eval();
	p1 = pop();
	if (isnonnegativeinteger(p1)) {
		if (mprime(p1->u.q.a))
			push(symbol(YYTRUE));
		else
			push(symbol(YYFALSE));
	} else
		stop("isprime: nonnegative integer arg expected");
}

static char *s[] = {

	"isprime(13)",
	"true",

	"isprime(14)",
	"false",

	// from the Prime Curios web page

	"isprime(9007199254740991)",
	"false",

	// The largest prime that JavaScript supports

	"isprime(2^53 - 111)",
	"true",

	// misc. primes

	"isprime(2^50-71)",
	"true",

	"isprime(2^40-87)",
	"true",
};

void
test_isprime(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
