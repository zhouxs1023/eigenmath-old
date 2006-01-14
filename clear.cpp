#include "stdafx.h"
#include "defs.h"

void
eval_clear(void)
{
	clear_term();
	clear_symbols();
	defn();
	push(Nil);
}

// clear from application GUI code

void
clear(void)
{
	run("clear");
}
