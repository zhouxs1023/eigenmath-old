#include "stdafx.h"
#include "defs.h"

extern int scan(char *);
extern void setup(void);
extern void selftest(void);
extern void cmdisplay(U *);

extern int symbol_level, test_flag;
extern U *varlist;

static void check_frame(void);
static void print_mem_info(void);
static int get_format(void);

jmp_buf stop_return;

static char *errstr;
static char buf[100];

void
stop(char *s)
{
	errstr = s;
	longjmp(stop_return, 1);
}

void
run(char *s)
{
	int n, tty;

	if (strcmp(s, "--mem") == 0) {
		print_mem_info();
		return;
	}

	if (strcmp(s, "--gc") == 0) {
		varlist = nil;
		symbol_level = 0;
		tos = 0;
		frame = stack + TOS;
		gc();
		return;
	}

	if (strcmp(s, "--test") == 0) {
		selftest();
		return;
	}

	if (setjmp(stop_return)) {
		restore_symbols(0);
		if (errstr) {
			printstr(errstr);
			printstr("\n");
		}
		return;
	}

	varlist = nil;

	symbol_level = 0;

	tos = 0;

	frame = stack + TOS;

	while (1) {

		n = scan(s);

		if (n == 0)
			break;

		s += n;

		setup();

		p1 = pop();
		check_frame();
		push(p1);
		eval();
		p2 = pop();
		check_frame();

		symbol(LAST)->u.sym.binding = last;
		symbol(LAST)->u.sym.binding2 = nil;

		// print string w/o quotes

		if (p2->k == STR) {
			printstr(p2->u.str);
			printstr("\n");
			continue;
		}

		// don't print nil unless it was due to eval of a symbol

		if (p2 == nil && !issymbol(p1))
			continue;

		tty = get_format();

		if (tty || test_flag)
			printline(p2);
		else {
#ifdef LINUX
			display(p2);
#else
			if (p1->k == SYM
			&& p1 != symbol(LAST)
			&& p1 != symbol(TTY)
			&& p1 != p2) {
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

static int
get_format(void)
{
	int fmt;
	save();
	p1 = symbol(TTY);
	push(p1->u.sym.binding);
	fmt = pop_integer();
	if (fmt != 0)
		fmt = 1;
	restore();
	return fmt;
}

void
check_frame(void)
{
	if (tos != 0)
		printstr("stack error\n");
	if (frame != stack + TOS)
		printstr("frame error\n");
	tos = 0;
	frame = stack + TOS;
}

void
echo_input(char *s)
{
	int tty;
	tty = get_format();
	if (tty)
		printstr("\n");
	printstr(s);
	printstr("\n");
}

extern int total_count;
extern int free_count;

static void
print_mem_info(void)
{
	sprintf(buf, "total atoms %d (%dM)   free atoms %d   used atoms %d   sizeof %d\n",
		total_count,
		total_count / 1000000,
		free_count,
		total_count - free_count,
		(int) sizeof (U));
	printstr(buf);
}
