#include "stdafx.h"
#include "defs.h"

void
eval_clear(void)
{
	clear();
	push(nil);
}

void
clear(void)
{
	clear_term();
	clear_symbols();
	defn();
}
