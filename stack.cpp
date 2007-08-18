//-----------------------------------------------------------------------------
//
//	 _______
//	|	| <- stack
//	|	|
//	|_______|
//	|	| <- stack + tos
//	|	|
//	|	|
//	|_______|
//	|	| <- frame
//	|_______|
//		  <- stack + TOS
//
//	The stack grows from low memory towards high memory. This is so that
//	multiple expressions can be pushed on the stack and then accessed as an
//	array.
//
//	The frame area holds local variables and grows from high memory towards
//	low memory. The frame area makes local variables visible to the garbage
//	collector.
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "defs.h"

U **frame, *stack[TOS];
int tos;

void
push(U *p)
{
	if (stack + tos >= frame)
		stop("stack overflow");
	stack[tos++] = p;
}

U *
pop()
{
	if (tos == 0)
		stop("stack underflow");
	return stack[--tos];
}

void
push_frame(int n)
{
	int i;
	frame -= n;
	if (frame < stack + tos)
		stop("frame overflow, circular reference?");
	for (i = 0; i < n; i++)
		frame[i] = symbol(NIL);
}

void
pop_frame(int n)
{
	frame += n;
	if (frame > stack + TOS)
		stop("frame underflow");
}

void
save(void)
{
	frame -= 8;
	if (frame < stack + tos)
		stop("frame overflow, circular reference?");
	frame[0] = p1;
	frame[1] = p2;
	frame[2] = p3;
	frame[3] = p4;
	frame[4] = p5;
	frame[5] = p6;
	frame[6] = p7;
	frame[7] = p8;
}

void
restore(void)
{
	p1 = frame[0];
	p2 = frame[1];
	p3 = frame[2];
	p4 = frame[3];
	p5 = frame[4];
	p6 = frame[5];
	p7 = frame[6];
	p8 = frame[7];
	frame += 8;
	if (frame > stack + TOS)
		stop("frame underflow");
}

void
restore_frame(U **save_frame)
{
	frame = save_frame - 8;
	restore();
}

void
swap(void)
{
	U *tmp;
	tmp = stack[tos - 1];
	stack[tos - 1] = stack[tos - 2];
	stack[tos - 2] = tmp;
}
