// Guess which symbol to use for derivative, integral, etc.

#include "stdafx.h"
#include "defs.h"

void
guess(void)
{
	if (find(stack[tos - 1], symbol(SYMBOL_X)))
		push_symbol(SYMBOL_X);
	else if (find(stack[tos - 1], symbol(SYMBOL_T)))
		push_symbol(SYMBOL_T);
	else
		push_symbol(SYMBOL_X);
}
