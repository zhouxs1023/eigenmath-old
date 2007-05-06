#include "stdafx.h"
#include "defs.h"

extern int test_flag;
jmp_buf stop_return;
static char *errstr;

void
stop(char *str)
{
	errstr = str; // don't print str now, jmp_buf might be redirected
	longjmp(stop_return, 1);
}

void
run(char *s)
{
	int n;

	esc_flag = 0;
	draw_flag = 0;

	if (setjmp(stop_return)) {
		if (errstr) {
			printstr("Stop: ");
			printstr(errstr);
			printstr("\n");
		}
		return;
	}

	init();

	tos = 0;

	frame = stack + TOS;

	if (dash_dash_command(s))
		return;

	while (1) {

		n = scan(s);

		if (n == 0)
			break;

		s += n;

		setup();

		p1 = pop();
		check_stack();

		push(p1);
		top_level_eval();

		p2 = pop();
		check_stack();

		if (p2 == symbol(NIL))
			continue;

		// print string w/o quotes

		if (isstr(p2)) {
			printstr(p2->u.str);
			printstr("\n");
			continue;
		}

		if (equal(symbol(TTY)->u.sym.binding, one) || test_flag) // tty mode?
			printline(p2);
		else {
#ifdef LINUX
			display(p2);
#else
			cmdisplay(p2);
#endif
		}
	}
}

int
dash_dash_command(char *s)
{
	if (strncmp(s, "--mem", 5) == 0) {
		print_mem_info();
		return 1;
	}
	if (strncmp(s, "--gc", 4) == 0) {
		gc();
		printstr("OK\n");
		return 1;
	}
	if (strncmp(s, "--test", 6) == 0) {
		selftest();
		return 1;
	}
	return 0;
}

void
check_stack(void)
{
	if (tos != 0)
		stop("stack error");
	if (frame != stack + TOS)
		stop("frame error");
}

// cannot reference symbols yet

void
echo_input(char *s)
{
	printstr(s);
	printstr("\n");
}

// returns nil on stack if no result to print

void
top_level_eval(void)
{
	save();

	p1 = pop();
	push(p1);
	eval();
	p2 = pop();

	// "draw", "for" and "setq" return "nil", there is no result to print

	if (p2 == symbol(NIL)) {
		push(p2);
		restore();
		return;
	}

	// update "last"

	symbol(LAST)->u.sym.binding = p2;
	symbol(LAST)->u.sym.arglist = symbol(NIL);

	if (!iszero(symbol(BAKE)->u.sym.binding)) {
		push(p2);
		bake();
		p2 = pop();
	}

	// If we evaluated the symbol "i" or "j" and the result was sqrt(-1)

	// then don't do anything.

	// Otherwise if "j" is an imaginary unit then subst.

	// Otherwise if "i" is an imaginary unit then subst.

	if ((p1 == symbol(SYMBOL_I) || p1 == symbol(SYMBOL_J))
	&& isimaginaryunit(p2))
		;
	else if (isimaginaryunit(symbol(SYMBOL_J)->u.sym.binding)) {
		push(p2);
		push(imaginaryunit);
		push_symbol(SYMBOL_J);
		subst();
		p2 = pop();
	} else if (isimaginaryunit(symbol(SYMBOL_I)->u.sym.binding)) {
		push(p2);
		push(imaginaryunit);
		push_symbol(SYMBOL_I);
		subst();
		p2 = pop();
	}

#ifndef LINUX

	// if we evaluated the symbol "a" and got "b" then print "a=b"

	// do not print "a=a"

	if (issymbol(p1) && p1 != p2 && test_flag == 0) {
		push_symbol(SETQ);
		push(p1);
		push(p2);
		list(3);
		p2 = pop();
	}
#endif
	push(p2);

	restore();
}

void
check_esc_flag(void)
{
	if (esc_flag)
		stop("esc key");
}
