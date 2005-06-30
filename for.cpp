//	Input:		tos-4		Index variable
//
//			tos-3		Initial value
//
//			tos-2		Final value
//
//			tos-1		Statement
//
//	Output:		Result on stack

#include "stdafx.h"
#include "defs.h"
#define A p1
#define J p2
#define K p3
#define B p4
static int level;
static jmp_buf break_jmp_buf;

void
for_function(void)
{
	int i, j, k;
	jmp_buf save_jmp_buf;
	volatile int save_tos, mark;
	U ** volatile save_frame;

	memcpy(save_jmp_buf, break_jmp_buf, sizeof break_jmp_buf);

	if (setjmp(break_jmp_buf)) {
		memcpy(break_jmp_buf, save_jmp_buf, sizeof save_jmp_buf);
		stack[save_tos] = pop();
		tos = save_tos + 1;
		frame = save_frame;
		restore();
		restore_symbols(mark);
		level--;
		return;
	}

	level++;

	save();

	B = pop();
	K = pop();
	J = pop();
	A = pop();

	save_tos = tos;
	save_frame = frame;

	if (A->k != SYM)
		stop("variable expected in 1st arg of \"for\" function");
	push(A);
	mark = save_symbols(1);

	push(J);
	eval();
	j = pop_integer();
	if (j == (int) 0x80000000)
		stop("integer value expected in 2nd arg of \"for\" function");

	push(K);
	eval();
	k = pop_integer();
	if (k == (int) 0x80000000)
		stop("integer value expected in 3rd arg of \"for\" function");

	for (i = j; i <= k; i++) {

		push_integer(i);

		A->u.sym.binding = pop();
		A->u.sym.binding2 = nil;

		push(B);
		eval();
		pop();
	}

	push(nil);

	restore_symbols(mark);

	restore();

	level--;
}

void
break_function(void)
{
	if (level == 0)
		stop("break without for");
	longjmp(break_jmp_buf, 1);
}

static char *s[] = {

	"a=123",
	"",

	"b=for(a,1,10,c=a)",
	"",

	"a",
	"123",

	"b",
	"nil",

	"c",
	"10",

	"b=for(a,1,10,test(a==10,break(456)))",
	"",

	"a",
	"123",

	"b",
	"456",

	"a=quote(a)",
	"",

	"b=quote(b)",
	"",

	"c=quote(c)",
	"",
};

void
test_for(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
