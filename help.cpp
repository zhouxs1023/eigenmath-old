#include "stdafx.h"

#include <stdio.h>

extern void run(char *);

static char *exponent_help =
"clear()\n"
"\"Type ^ for exponent, like this:\"\n"
"\"x^2\"\n"
"x^2\n";

static char *help_multiply =
"clear()\n"
"\"Type a space to indicate multiplication, like this:\"\n"
"\"A (B + C)\"\n"
"A (B + C)\n"
"\"Without a space, the computer thinks A is a function.\"\n"
"\"A(B + C)\"\n"
"A(B + C)\n";

static char *draw_help =
"clear()\n"
"\"For example, type x^2 then click the draw button.\"\n"
"\"draw(x^2)\"\n"
"draw(x^2)\n";

static char *factor_polynomial_help =
"clear()\n"
"\"For example, type x^2 + 3 x + 2 then click the factor button.\"\n"
"\"factor(x^2 + 3 x + 2)\"\n"
"factor(x^2 + 3 x + 2)\n";

static char *factor_number_help =
"clear()\n"
"\"For example, type 900 then click the factor button.\"\n"
"\"factor(900)\"\n"
"factor(900)\n";

static char *help_symbol =
"clear()\n"
"\"For example, type\"\n"
"\"x = a + b\"\n"
"x = a + b\n"
"\"Enter the symbol to see its value.\"\n"
"\"x\"\n"
"x\n";

static char *help_function =
"clear()\n"
"\"For example, type\"\n"
"\"f(x) = x^2 + 3\"\n"
"f(x) = x^2 + 3\n"
"\"Pass the function an argument to have it evaluated.\"\n"
"\"f(a + b)\"\n"
"f(a + b)\n"
"\"Enter the symbol to see its value.\"\n"
"\"f\"\n"
"f\n";

static char *help_special_note_1 =
"clear()\n"
"\"Normally a function is not evaluated when it is defined.\"\n"
"\"The eval function can be used to change this.\"\n"
"\"Here is a before-and-after example.\"\n"
"\"f(x) = taylor(cos(x),x,6)\"\n"
"f(x) = taylor(cos(x),x,6)\n"
"\"f\"\n"
"f\n"
"\"f(0)\"\n"
"f(0)\n";

static char *help_special_note_2 =
"\"f(x) = eval(taylor(cos(x),x,6))\"\n"
"f(x) = eval(taylor(cos(x),x,6))\n"
"\"f\"\n"
"f\n"
"\"f(0)\"\n"
"f(0)\n";

static char *help_type_vector =
"clear()\n"
"\"For example, type\"\n"
"\"X = (10,20)\"\n"
"X = (10,20)\n"
"\"Enter the symbol to see its value.\"\n"
"\"X\"\n"
"X\n";

static char *help_type_matrix =
"clear()\n"
"\"For example, type\"\n"
"\"A = ((1,2),(3,4))\"\n"
"A = ((1,2),(3,4))\n"
"\"Enter the symbol to see its value.\"\n"
"\"A\"\n"
"A\n";

static char *help_matrix_times_vector =
"clear()\n"
"\"Example:\"\n"
"\"A = ((A11,A12),(A21,A22))\"\n"
"A = ((A11,A12),(A21,A22))\n"
"\"X = (X1,X2)\"\n"
"X = (X1,X2)\n"
"\"dot(A,X)\"\n"
"dot(A,X)\n";

static char *help_invert_matrix =
"clear()\n"
"\"Example:\"\n"
"\"A = ((1,2),(3,4))\"\n"
"A = ((1,2),(3,4))\n"
"\"inv(A)\"\n"
"inv(A)\n"
"\"dot(A,inv(A))\"\n"
"dot(A,inv(A))\n";

static char *help_draw_circle =
"clear()\n"
"\"r(t) = 5 (sin(t),cos(t))\"\n"
"r(t) = 5 (sin(t),cos(t))\n"
"\"draw(r(t),t)\"\n"
"draw(r(t),t)\n";

static void
do_help_topic(char *s)
{
	run(s);
}

void
do_help(int n)
{
	switch (n) {
	case 1:
		do_help_topic(exponent_help);
		break;
	case 2:
		do_help_topic(help_multiply);
		break;
	case 3:
		do_help_topic(draw_help);
		break;
	case 4:
		do_help_topic(factor_polynomial_help);
		break;
	case 5:
		do_help_topic(factor_number_help);
		break;
	case 6:
		do_help_topic(help_symbol);
		break;
	case 7:
		do_help_topic(help_function);
		break;
	case 8:
		do_help_topic(help_special_note_1);
		do_help_topic(help_special_note_2);
		break;
	case 9:
		do_help_topic(help_type_vector);
		break;
	case 10:
		do_help_topic(help_type_matrix);
		break;
	case 11:
		do_help_topic(help_matrix_times_vector);
		break;
	case 12:
		do_help_topic(help_invert_matrix);
		break;
	case 13:
		do_help_topic(help_draw_circle);
		break;
	}
}
