#include "stdafx.h"

#include "defs.h"

static int level;
static jmp_buf return_jmp_buf;

void
prog(void)
{
	int h;
	jmp_buf save_jmp_buf;
	volatile int save_tos, mark;
	U ** volatile save_frame;

	memcpy(save_jmp_buf, return_jmp_buf, sizeof return_jmp_buf);

	if (setjmp(return_jmp_buf)) {
		memcpy(return_jmp_buf, save_jmp_buf, sizeof save_jmp_buf);
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

	p1 = pop();

	save_tos = tos;
	save_frame = frame;

	h = tos;

	// save local vars

	while (iscons(cdr(p1))) {
		push(car(p1));
		p1 = cdr(p1);
	}

	mark = save_symbols(tos - h);

	push(car(p1));
	eval();

	restore_symbols(mark);

	restore();

	level--;
}

void
prog_return(void)
{
	if (level == 0)
		stop("return without prog");
	longjmp(return_jmp_buf, 1);
}

static char *s[] = {

	"a=123",
	"",

	"b=456",
	"",

	// return occurs in for, check that both a and b are restored

	"prog(a,do(a=10,for(b,1,a,test(b==5,return(789)))))",
	"789",

	"a",
	"123",

	"b",
	"456",

	"a=quote(a)",
	"",

	"b=quote(b)",
	"",
};

void
test_prog(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
