// Create a list from n things on the stack.

#include "stdafx.h"
#include "defs.h"

void
list(int n)
{
	int i;
	push(Nil);
	for (i = 0; i < n; i++)
		cons();
}
