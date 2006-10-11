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

		top_level_eval();

		p2 = pop();
		check_stack();

		// print string w/o quotes

		if (isstr(p2)) {
			printstr(p2->u.str);
			printstr("\n");
			continue;
		}

		// don't print nil unless it was due to eval of a symbol

		if (p2 == symbol(NIL) && (iskeyword(p1) || !issymbol(p1)))
			continue;

		if (!iszero(symbol(BAKE)->u.sym.binding)) {
			push(p2);
			bake();
			p2 = pop();
		}

		if (equal(symbol(TTY)->u.sym.binding, one) || test_flag) // tty mode?
			printline(p2);
		else {
#ifdef LINUX
			display(p2);
#else
			if (issymbol(p1) && !iskeyword(p1) && p1 != p2) {
				// print as a = b
				push_symbol(SETQ);
				push(p1);
				push(p2);
				list(3);
				p2 = pop();
			}
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

// the main idea is to not propagate denormal expressions

void
top_level_eval(void)
{
	// handle bare keywords that yield denormals

	if (p1 == symbol(FACTOR)) {
		push(symbol(LAST)->u.sym.binding);
		guess();
		factor();
		return;
	}

	if (p1 == symbol(SIMPLIFY)) {
		push(symbol(LAST)->u.sym.binding);
		simplify();
		return;
	}

	if (p1 == symbol(CONDENSE)) {
		push(symbol(LAST)->u.sym.binding);
		Condense();
		return;
	}

	if (p1 == symbol(RATIONALIZE)) {
		push(symbol(LAST)->u.sym.binding);
		rationalize();
		return;
	}

	// handle functions that yield denormals

	if (car(p1) == symbol(FACTOR)) {
		push(cadr(p1));
		eval();
		symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
		symbol(LAST)->u.sym.arglist = symbol(NIL);
		push(caddr(p1));
		eval();
		factor();
		return;
	}

	if (car(p1) == symbol(SIMPLIFY)) {
		push(cadr(p1));
		eval();
		symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
		symbol(LAST)->u.sym.arglist = symbol(NIL);
		simplify();
		return;
	}

	if (car(p1) == symbol(CONDENSE)) {
		push(cadr(p1));
		eval();
		symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
		symbol(LAST)->u.sym.arglist = symbol(NIL);
		Condense();
		return;
	}

	if (car(p1) == symbol(RATIONALIZE)) {
		push(cadr(p1));
		eval();
		symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
		symbol(LAST)->u.sym.arglist = symbol(NIL);
		rationalize();
		return;
	}

	// default case

	push(p1);
	eval();
	symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
	symbol(LAST)->u.sym.arglist = symbol(NIL);
}

void
check_esc_flag(void)
{
	if (esc_flag)
		stop("esc key");
}
