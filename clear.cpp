#include "stdafx.h"
#include "defs.h"

void
eval_clear(void)
{
	clear_term();
	clear_symbols();
	defn();
	push(nil);
}

// clear from application GUI code

void
clear(void)
{
	run("clear");
}
