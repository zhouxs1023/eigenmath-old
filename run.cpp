#include "stdafx.h"
#include "defs.h"

extern int symbol_level;
extern int test_flag;
extern U *varlist;
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

	if (setjmp(stop_return)) {
		restore_symbols(0);
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

	symbol_level = 0;

	varlist = symbol(NIL);

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
		eval();

		p2 = pop();
		check_stack();

		symbol(LAST)->u.sym.binding = symbol(YYLAST)->u.sym.binding;
		symbol(LAST)->u.sym.binding2 = symbol(NIL);

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
