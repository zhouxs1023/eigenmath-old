#include "stdafx.h"

//-----------------------------------------------------------------------------
//
//	Look up the nth prime
//
//	Input:		n on stack (0 < n < 10001)
//
//	Output:		nth prime on stack
//
//-----------------------------------------------------------------------------

#include "defs.h"

static void primef(void);

void
prime(void)
{
	save();
	primef();
	restore();
}

static void
primef(void)
{
	int n;

	n = pop_integer();

	if (n < 1 || n > MAXPRIMETAB)
		stop("prime: Argument out of range.");

	n = primetab[n - 1];

	push_integer(n);
}
