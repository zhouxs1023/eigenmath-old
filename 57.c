/* Unfinished SDL drawing tool. */

/*

	symbol dimensions

	. . . . x . . . .

	. x x x x x x x .

	. x . . . . . x .

	. x x x x x x x .

	. . . . x . . . .

	8 x 4 outside

	6 x 2 inside

	each symbol is scaled by 40 for display and print

	drawn with center point reference

	(x - 4, y - 2) . . . . . . . . . . . . . . . . . (x + 4, y - 2)

	.                                                             .

	.                                                             .

	.            (x - 3, y - 1) . . . . (x + 3, y - 1)            .

	.            .                                   .            .

	.            .               (x, y)              .            .

	.            .                                   .            .

	.            (x - 3, y + 1) . . . . (x + 3, y + 1)            .

	.                                                             .

	.                                                             .

	(x - 4, y + 2) . . . . . . . . . . . . . . . . . (x + 4, y + 2)

	printer output

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	.    .    .    .    .    .

	5 x 12 symbols

	320 x 160 pixels per symbol

	5 x 320 = 1600 pixels wide

	1600 pixels / (240 pixels/inch) = 6.67 inches wide

	12 x 160 = 1920 pixels high

	1920 pixels / (216 pixels/inch) = 8.89 inches high

*/

#include <stdio.h>

#include <stdlib.h>

#include <alloc.h>

#include <graphics.h>

#include <setjmp.h>

#include <ctype.h>

#include <math.h>

#include <dos.h>

#include <bios.h>

#include <fcntl.h>

#include <sys\stat.h>

#include <dir.h>

/* int mag = 8; */ /* for page preview */

int mag = 40;

int graphdriver;

int graphmode;

int mousex;

int mousey;

int click;

int bx;

int screenx;

int screeny;

int cursorx;

int cursory;

int cursorflag;

#define BUFSIZE 256

char buf[BUFSIZE];

char *filename;

#define MAX 49

typedef struct symbol {

	struct symbol *next;

	int n, x, y;

	char s[MAX];

} SYMBOL;

SYMBOL root;

SYMBOL *cursorp;

SYMBOL *ep; /* edit pointer */

int editindex;

main(argc, argv)

int argc;

char *argv[];

{

	int x, y;

	SYMBOL *p;

	if (argc == 1)

		filename = "untitled";

	else {

		filename = argv[1];

		load();

	}

	graphics();

	drawscreen();

	for (;;) {

		check_keyboard();

		check_mouse();

		check_view();

		if (click) {

			abandon_edit();

			/* convert mouse coordinates to symbol coordinates */

			x = screenx + mousex / 320 * 320 + 160;

			y = screeny + mousey / 160 * 160 + 80;

			p = root.next;

			while (p) {

				if (p->x == x && p->y == y)

					break;

				p = p->next;

			}

			if (p)

				find_click();

			else {

				select_symbol(x, y);

				drawscreen();

			}

		}

	}

}

/* move when mouse touches edge of screen */

check_view()

{

	if (mousex == 0 && screenx > 0) {

		screenx -= 320;

		abandon_edit();

		drawscreen();

	}

	if (mousex == getmaxx() && screenx < 3 * 320) {

		screenx += 320;

		abandon_edit();

		drawscreen();

	}

	if (mousey == 0 && screeny > 0) {

		screeny -= 160;

		abandon_edit();

		drawscreen();

	}

	if (mousey == getmaxy() && screeny < 9 * 160) {

		screeny += 160;

		abandon_edit();

		drawscreen();

	}

}

SYMBOL *alloc(n)

int n;

{

	SYMBOL *p;

	p = &root;

	while (p->next)

		p = p->next;

	p->next = malloc(n);

	if (p->next == NULL) {

		closegraph();

		printf("out of memory\n");

		exit(1);

	}

	p = p->next;

	p->next = NULL;

	return p;

}

add_symbol(n, x, y)

int n, x, y;

{

	SYMBOL *p;

	p = alloc(sizeof (SYMBOL));

	p->n = n;

	p->x = x;

	p->y = y;

}

graphics()

{

	int i, t, y;

	union REGS regs;

	registerbgidriver(CGA_driver);

	registerbgidriver(EGAVGA_driver);

	registerbgidriver(Herc_driver);

	registerbgidriver(ATT_driver);

	registerbgidriver(PC3270_driver);

	registerbgidriver(IBM8514_driver);

	initgraph(&graphdriver, &graphmode, "");

	/* init mouse driver */

	if (graphdriver == 7) {

		/* herc */

		t = peekb(0x40, 0x49);

		pokeb(0x40, 0x49, 6);

		regs.x.ax = 0;

		int86(0x33, &regs, &regs);

		pokeb(0x40, 0x49, t);

	} else {

		regs.x.ax = 0;

		int86(0x33, &regs, &regs);

	}

	if (regs.x.ax == 0) {

		closegraph();

		printf("mouse required\n");

		exit(1);

	}

	pointeron();

}

pointeroff()

{

	union REGS regs;

	regs.x.ax = 2;

	int86(0x33, &regs, &regs);

}

pointeron()

{

	union REGS regs;

	regs.x.ax = 1;

	int86(0x33, &regs, &regs);

}

esc()

{

	if (kbhit())

		switch (getch()) {

		case 0:

			getch();

			return 0;

		case 27:

			return 1;

		default:

			return 0;

		}

	else

		return 0;

}

drawscreen()

{

	pointeroff();

	setviewport(0, 0, getmaxx(), getmaxy(), 0);

	cleardevice();

	draw_grid();

	draw_list();

	pointeron();

}

draw_grid()

{

	int x, y;

	for (x = 0; x < 6; x++)

		for (y = 0; y < 13; y++)

			putpixel(8 * mag * x, 4 * mag * y, getcolor());

}

draw_list()

{

	SYMBOL *p;

	p = root.next;

	while (p) {

		draw_symbol(p);

		p = p->next;

	}

}

check_mouse()

{

	union REGS regs;

	regs.x.ax = 3;

	int86(0x33, &regs, &regs);

	mousex = regs.x.cx;

	mousey = regs.x.dx;

	if (regs.x.bx && bx == 0)

		click = 1;

	else

		click = 0;

	bx = regs.x.bx;

}

draw_symbol(p)

SYMBOL *p;

{

	int x, y;

	x = p->x - screenx;

	y = p->y - screeny;

	switch (p->n) {

	case 0:

		drawstring(x - stringwidth(p->s) / 2, y, p->s, 0, 0);

		break;

	case 1:

		draw_connector_1(x, y);

		break;

	case 2:

		draw_connector_2(x, y);

		break;

	case 3:

		draw_connector_3(x, y);

		break;

	case 4:

		draw_connector_4(x, y);

		break;

	case 5:

		draw_connector_5(x, y);

		break;

	case 6:

		draw_connector_6(x, y);

		break;

	case 7:

		draw_connector_7(x, y);

		break;

	case 8:

		draw_connector_8(x, y);

		break;

	case 9:

		draw_connector_9(x, y);

		break;

	case 10:

		draw_connector_10(x, y);

		break;

	case 11:

		draw_connector_11(x, y);

		break;

	case 12:

		draw_task_symbol_1(x, y);

		break;

	case 13:

		draw_task_symbol_2(x, y);

		break;

	case 14:

		draw_decision_symbol_1(x, y);

		break;

	case 15:

		draw_state_symbol_1(x, y);

		break;

	case 16:

		draw_state_symbol_2(x, y);

		break;

	case 17:

		draw_input_symbol(x, y);

		break;

	case 18:

		draw_output_symbol(x, y);

		break;

	case 19:

		draw_start_symbol(x, y);

		break;

	case 20:

		draw_return_symbol_1(x, y);

		break;

	case 21:

		draw_return_symbol_2(x, y);

		break;

	default:

		break;

	}

}

/* 

. . . . x . . . .

. . . . x . . . .

. . . . x . . . .

. . . . x . . . .

. . . . x . . . .

*/

draw_connector_1(x, y)

int x, y;

{

	moveto(x, y - 2 * mag);

	linerel(0, 4 * mag);

}

/* 

. . . . . . . . .

. . . . . . . . .

x x x x x x x x x

. . . . . . . . .

. . . . . . . . .

*/

draw_connector_2(x, y)

int x, y;

{

	moveto(x - 4 * mag, y);

	linerel(8 * mag, 0);

}

/* 

. . x . .

. . x . .

. . x x x

. . . . .

. . . . .

*/

draw_connector_3(x, y)

int x, y;

{

	moveto(x, y - 2 * mag);

	linerel(0, 2 * mag);

	linerel(4 * mag, 0);

}

/* 

. . . . .

. . . . .

. . x x x

. . x . .

. . x . .

*/

draw_connector_4(x, y)

int x, y;

{

	moveto(x + 4 * mag, y);

	linerel(-4 * mag, 0);

	linerel(0, 2 * mag);

}

/* 

. . . . .

. . . . .

x x x . .

. . x . .

. . x . .

*/

draw_connector_5(x, y)

int x, y;

{

	moveto(x - 4 * mag, y);

	linerel(4 * mag, 0);

	linerel(0, 2 * mag);

}

/* 

. . x . .

. . x . .

x x x . .

. . . . .

. . . . .

*/

draw_connector_6(x, y)

int x, y;

{

	moveto(x - 4 * mag, y);

	linerel(4 * mag, 0);

	linerel(0, -2 * mag);

}

/* 

. . x . .

. . x . .

x x x x x

. . . . .

. . . . .

*/

draw_connector_7(x, y)

int x, y;

{

	moveto(x - 4 * mag, y);

	linerel(8 * mag, 0);

	moveto(x, y - 2 * mag);

	linerel(0, 2 * mag);

}

/* 

. . . . .

. . . . .

x x x x x

. . x . .

. . x . .

*/

draw_connector_8(x, y)

int x, y;

{

	moveto(x - 4 * mag, y);

	linerel(8 * mag, 0);

	moveto(x, y + 2 * mag);

	linerel(0, -2 * mag);

}

/* 

. . x . .

. . x . .

x x x . .

. . x . .

. . x . .

*/

draw_connector_9(x, y)

int x, y;

{

	moveto(x, y - 2 * mag);

	linerel(0, 4 * mag);

	moveto(x - 4 * mag, y);

	linerel(4 * mag, 0);

}

/* 

. . x . .

. . x . .

. . x x x

. . x . .

. . x . .

*/

draw_connector_10(x, y)

int x, y;

{

	moveto(x, y - 2 * mag);

	linerel(0, 4 * mag);

	moveto(x + 4 * mag, y);

	linerel(-4 * mag, 0);

}

/* 

. . x . .

. . x . .

x x x x x

. . x . .

. . x . .

*/

draw_connector_11(x, y)

int x, y;

{

	moveto(x, y - 2 * mag);

	linerel(0, 4 * mag);

	moveto(x - 4 * mag, y);

	linerel(8 * mag, 0);

}

draw_input_symbol(x, y)

int x, y;

{

	moveto(x - 3 * mag, y - mag);

	linerel(6 * mag, 0);

	linerel(0, 2 * mag);

	linerel(-6 * mag, 0);

	linerel(mag, -mag);

	linerel(-mag, -mag);

	/* top connector */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

	/* bottom connector */

	moveto(x, y + 2 * mag);

	linerel(0, -mag);

}

draw_output_symbol(x, y)

int x, y;

{

	moveto(x - 3 * mag, y - mag);

	linerel(5 * mag, 0);

	linerel(mag, mag);

	linerel(-mag, mag);

	linerel(-5 * mag, 0);

	linerel(0, -2 * mag);

	/* top connector */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

	/* bottom connector */

	moveto(x, y + 2 * mag);

	linerel(0, -mag);

}

draw_decision_symbol_1(x, y)

int x, y;

{

	moveto(x, y - mag);

	linerel(3 * mag, mag);

	linerel(-3 * mag, mag);

	linerel(-3 * mag, -mag);

	linerel(3 * mag, -mag);

	/* top line */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

	/* left line */

	moveto(x - 4 * mag, y);

	linerel(mag, 0);

	/* right line */

	moveto(x + 4 * mag, y);

	linerel(-mag, 0);

}

draw_task_symbol_1(x, y)

int x, y;

{

	moveto(x - 3 * mag, y - mag);

	linerel(6 * mag, 0);

	linerel(0, 2 * mag);

	linerel(-6 * mag, 0);

	linerel(0, -2 * mag);

	/* top connector */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

	/* bottom connector */

	moveto(x, y + 2 * mag);

	linerel(0, -mag);

}

draw_task_symbol_2(x, y)

int x, y;

{

	moveto(x - 3 * mag, y - mag);

	linerel(6 * mag, 0);

	linerel(0, 2 * mag);

	linerel(-6 * mag, 0);

	linerel(0, -2 * mag);

	/* left connector */

	moveto(x - 4 * mag, y);

	linerel(mag, 0);

	/* right connector */

	moveto(x + 4 * mag, y);

	linerel(-mag, 0);

}

draw_state_symbol_1(x, y)

int x, y;

{

	/* left arc */

	ellipse(x - 3 * mag + mag / 2, y, 90, 270, mag / 2, mag);

	/* right arc */

	ellipse(x + 3 * mag - mag / 2, y, 270, 90, mag / 2, mag);

	/* top */

	moveto(x - 3 * mag + mag / 2, y - mag);

	linerel(5 * mag, 0);

	/* bottom */

	moveto(x - 3 * mag + mag / 2, y + mag);

	linerel(5 * mag, 0);

	/* bottom connector */

	moveto(x, y + 2 * mag);

	linerel(0, -mag);

}

draw_state_symbol_2(x, y)

int x, y;

{

	/* left arc */

	ellipse(x - 3 * mag + mag / 2, y, 90, 270, mag / 2, mag);

	/* right arc */

	ellipse(x + 3 * mag - mag / 2, y, 270, 90, mag / 2, mag);

	/* top */

	moveto(x - 3 * mag + mag / 2, y - mag);

	linerel(5 * mag, 0);

	/* bottom */

	moveto(x - 3 * mag + mag / 2, y + mag);

	linerel(5 * mag, 0);

	/* top connector */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

}

draw_start_symbol(x, y)

int x, y;

{

	/* left arc */

	ellipse(x - 2 * mag, y, 90, 270, mag, mag);

	/* right arc */

	ellipse(x + 2 * mag, y, 270, 90, mag, mag);

	/* top */

	moveto(x - 2 * mag, y - mag);

	linerel(4 * mag, 0);

	/* bottom */

	moveto(x - 2 * mag, y + mag);

	linerel(4 * mag, 0);

	/* bottom connector */

	moveto(x, y + 2 * mag);

	linerel(0, -mag);

}

draw_return_symbol_1(x, y)

int x, y;

{

	int a;

	a = 7 * mag / 10;

	circle(x, y, mag);

	moveto(x - a, y - a);

	linerel(2 * a, 2 * a);

	moveto(x + a, y - a);

	linerel(-2 * a, 2 * a);

	/* top connector */

	moveto(x, y - 2 * mag);

	linerel(0, mag);

}

draw_return_symbol_2(x, y)

int x, y;

{

	int a;

	/* one over square root of two */

	a = 7 * mag / 10;

	circle(x, y, mag);

	moveto(x - a, y - a);

	linerel(2 * a, 2 * a);

	moveto(x + a, y - a);

	linerel(-2 * a, 2 * a);

	/* left connector */

	moveto(x - 4 * mag, y);

	linerel(3 * mag, 0);

}

select_symbol(x, y)

int x, y;

{

	int dy, n, t;

	SYMBOL sym;

	sym.n = 0;

	sym.x = x;

	sym.y = y;

	if (y - screeny == 400)

		dy = 0;

	else

		dy = 320;

	draw_symbol_menu(dy);

	for (;;) {

		if (esc())

			return;

		check_mouse();

		if (mousey - dy < 0)

			n = 0;

		else

			n = 8 * ((mousey - dy) / 40) + mousex / 80 + 1;

		if (n != sym.n) {

			pointeroff();

			t = getcolor();

			setcolor(0);

			if (sym.n)

				draw_symbol(&sym);

			sym.n = n;

			setcolor(t);

			if (sym.n)

				draw_symbol(&sym);

			pointeron();

		}

		if (click)

			break;

	}

	if (sym.n > 0 && sym.n < 22)

		add_symbol(n, x, y);

}

draw_symbol_menu(y)

int y;

{

	int i, t;

	pointeroff();

	if (y == 0)

		setviewport(0, 0, 639, 160, 0);

	else

		setviewport(0, 320, 639, 479, 0);

	clearviewport();

	setviewport(0, 0, getmaxx(), getmaxy(), 0);

	/* draw dotted grid */

	setlinestyle(4, 0x1111, 1);

	for (i = 0; i < 5; i++)

		line(0, y + 40 * i, 640, y + 40 * i);

	for (i = 0; i < 8; i++)

		line(80 * i, y, 80 * i, y + 160);

	setlinestyle(0, 0, 1);

	/* draw symbols */

	t = mag;

	mag = 6;

	draw_connector_1(40, y + 20);

	draw_connector_2(80 + 40, y + 20);

	draw_connector_3(2 * 80 + 40, y + 20);

	draw_connector_4(3 * 80 + 40, y + 20);

	draw_connector_5(4 * 80 + 40, y + 20);

	draw_connector_6(5 * 80 + 40, y + 20);

	draw_connector_7(6 * 80 + 40, y + 20);

	draw_connector_8(7 * 80 + 40, y + 20);

	draw_connector_9(40, y + 60);

	draw_connector_10(80 + 40, y + 60);

	draw_connector_11(2 * 80 + 40, y + 60);

	draw_task_symbol_1(3 * 80 + 40, y + 60);

	draw_task_symbol_2(4 * 80 + 40, y + 60);

	draw_decision_symbol_1(5 * 80 + 40, y + 60);

	draw_state_symbol_1(6 * 80 + 40, y + 60);

	draw_state_symbol_2(7 * 80 + 40, y + 60);

	draw_input_symbol(40, y + 100);

	draw_output_symbol(80 + 40, y + 100);

	draw_start_symbol(2 * 80 + 40, y + 100);

	draw_return_symbol_1(3 * 80 + 40, y + 100);

	draw_return_symbol_2(4 * 80 + 40, y + 100);

	setviewport(0, 0, getmaxx(), getmaxy(), 0);

	mag = t;

	pointeron();

}

find_click()

{

	int i, n, w, x, y;

	SYMBOL *p;

	x = screenx + mousex;

	y = screeny + ((mousey - 8) & 0xfff0) + 12 + 8;

	p = root.next;

	while (p) {

		if (p->n == 0) {

			w = stringwidth(p->s);

			if (p->x - w / 2 - 16 < x && x < p->x + w / 2 + 16 && p->y == y)

				break;

		}

		p = p->next;

	}

	/* allocate new string */

	if (p == NULL) {

		p = &root;

		while (p->next)

			p = p->next;

		p->next = (SYMBOL *) malloc(sizeof (SYMBOL));

		if (p->next == NULL)

			outofmemory();

		p = p->next;

		p->next = NULL;

		p->n = 0;

		p->x = screenx + ((mousex + 8) & 0xfff0);

		p->y = screeny + ((mousey - 8) & 0xfff0) + 12 + 8;

		*p->s = 0;

	}

	cursorx = p->x - screenx - stringwidth(p->s) / 2;

	cursory = p->y - screeny;

	n = strlen(p->s);

	for (i = 0; i < n; i++)

		if (cursorx + charwidth(p->s[i]) / 2 < mousex)

			cursorx += charwidth(p->s[i]);

		else

			break;

	ep = p;

	editindex = i;

	show_cursor();

}

outofmemory()

{

	closegraph();

	printf("out of memory\n");

	exit(1);

}

check_keyboard()

{

	int c, i;

	if (!kbhit())

		return;

	c = getch();

	if (c == 0)

		c = -getch();

	if (c == 27) {

		closegraph();

		save();

		exit(1);

	}

	if (c == -59) {

		abandon_edit();

		pointeroff();

		print();

		pointeron();

		screenx = 0;

		screeny = 0;

		drawscreen();

		return;

	}

	if (ep == NULL)

		return;

	pointeroff();

	hide_cursor();

	draw_symbol(ep);

	if (c > 0x1f && c < 0x7f) {

		if (editindex < MAX - 2)

			insert(ep->s, editindex++, c);

	} else switch (c) {

		/* backspace */

		case 8:

			if (editindex > 0) {

				editindex--;

				delete(ep->s, editindex);

			}

			break;

		/* left arrow */

		case -75:

			if (editindex > 0)

				editindex--;

			break;

		/* right arrow */

		case -77:

			if (editindex < strlen(ep->s))

				editindex++;

			break;

		/* delete */

		case -83:

			if (editindex < strlen(ep->s))

				delete(ep->s, editindex);

			break;

		default:

			break;

		}

	draw_symbol(ep);

	/* new cursor location */

	cursorx = ep->x - screenx - stringwidth(ep->s) / 2;

	for (i = 0; i < editindex; i++)

		cursorx += charwidth(ep->s[i]);

	show_cursor();

	pointeron();

}

/* insert char c in string s at location n */

insert(s, n, c)

char *s;

int n, c;

{

	int i, len;

	len = strlen(s);

	for (i = len + 1; i > n; i--)

		s[i] = s[i - 1];

	s[n] = c;

}

/* delete char at location n from string s */

delete(s, n)

char *s;

int n;

{

	int i, len;

	len = strlen(s);

	for (i = n; i < len; i++)

		s[i] = s[i + 1];

}

abandon_edit()

{

	int i, n;

	SYMBOL *p;

	if (ep == NULL)

		return;

	hide_cursor();

	n = strlen(ep->s);

	for (i = 0; i < n; i++)

		if (!isspace(ep->s[i]))

			return;

	p = &root;

	while (p->next != ep)

		p = p->next;

	p->next = p->next->next;

	free(ep);

	ep = NULL;

}

show_cursor()

{

	if (cursorflag == 0) {

		cursorflag = 1;

		pointeroff();

		setwritemode(1); /* xor */

		line(cursorx, cursory - 12, cursorx, cursory - 1);

		setwritemode(0); /* or */

		pointeron();

	}

}

hide_cursor()

{

	if (cursorflag == 1) {

		cursorflag = 0;

		pointeroff();

		setwritemode(1); /* xor */

		line(cursorx, cursory - 12, cursorx, cursory - 1);

		setwritemode(0); /* or */

		pointeron();

	}

}

load()

{

	int n;

	FILE *f;

	SYMBOL *p;

	f = fopen(filename, "r");

	p = &root;

	while (fgets(buf, BUFSIZE, f)) {

		p->next = (SYMBOL *) malloc(sizeof (SYMBOL));

		if (p->next == NULL)

			outofmemory();

		p = p->next;

		p->next = NULL;

		sscanf(buf, "%d%d%d", &p->x, &p->y, &p->n);

		/* zero now in case next fgets fails */

		*p->s = 0;

		if (p->n == 0) {

			fgets(p->s, MAX, f);

			/* remove cr lf */

			n = strlen(p->s);

			if (n && p->s[n - 1] == '\n')

				p->s[n - 1] = 0;

		}

	}

	fclose(f);

}

save()

{

	FILE *f;

	SYMBOL *p;

	if (root.next == NULL)

		return;

	f = fopen(filename, "w");

	p = root.next;

	while (p) {

		fprintf(f, "%d %d %d\n", p->x, p->y, p->n);

		if (p->n == 0)

			fprintf(f, "%s\n", p->s);

		p = p->next;

	}

	fclose(f);

}

/*	scan image height has to be a multiple of 24 to print quad density

		scan line

	.	1
	.		9
	.			17
	.	2
	.		10
	.			18
	.	3
	.		11
	.			19
	.	4
	.		12
	.			20
	.	5
	.		13
	.			21
	.	6
	.		14
	.			22
	.	7
	.		15
	.			23
	.	8
	.		16
	.			24
	.

	use 5 x 24 = 120 because 1920 is divisible by 120

	1920 / 120 = 16

*/

char bigbuf[1920];

print()

{

	int i;

	FILE *f;

	f = fopen("prn", "r+b");

	for (i = 0; i < 16; i++) {

		draw_scan_image(i);

		print_scan_image(f);

		if (esc())

			break;

	}

	fclose(f);

}

draw_scan_image(n)
int n;
{
	cleardevice();

	screeny = 120 * n;

	screenx = 0;
	setviewport(0, 0, 639, 119, 1);
	draw_list();

	screenx = 640;
	setviewport(0, 160, 639, 279, 1);
	draw_list();

	screenx = 1280;
	setviewport(0, 320, 639, 439, 1);
	draw_list();

	setviewport(0, 0, getmaxx(), getmaxy(), 0);
}

print_scan_image(f)
FILE *f;
{
	int y;

	for (y = 0; y < 5; y++) {

		scanbuf(24 * y + 0);
		printbuf(f);

		/* 1/216" line feed */

		fputc(27, f);
		fputc('J', f);
		fputc(1, f);

		scanbuf(24 * y + 1);
		printbuf(f);

		/* 1/216" line feed */

		fputc(27, f);
		fputc('J', f);
		fputc(1, f);

		scanbuf(24 * y + 2);
		printbuf(f);

		/* 21/216" line feed */

		fputc(27, f);
		fputc('J', f);
		fputc(21, f);
	}
}

scanbuf(y)
int y;
{
	int i, m;

	for (i = 0; i < 1920; i++)
		bigbuf[i] = 0;

	m = 0x80;

	for (i = 0; i < 8; i++) {
		scanline(y + 3 * i, m);
		m >>= 1;
	}
}

scanline(y, m)
int y, m;
{
	int x;

	for (x = 0; x < 639; x++) {

		if (getpixel(x, y))
			bigbuf[x] |= m;

		if (getpixel(x, y + 160))
			bigbuf[x + 640] |= m;

		if (getpixel(x, y + 320))
			bigbuf[x + 1280] |= m;
	}

}

printbuf(f)
FILE *f;
{
	int i, n;

	for (n = 1920; n > 0; n--)
		if (bigbuf[n - 1])
			break;

	if (n) {

		/* left margin */

		fputs("          ", f);

		/* 240 dpi */

		fputc(27, f);

		fputc('Z', f);

		fputc(n % 256, f);

		fputc(n / 256, f);

		/* graphic data */

		for (i = 0; i < n; i++)

			if (i & 1)

				fputc(0, f);

			else

				fputc(bigbuf[i], f);

		/* carriage return */

		fputc(13, f);

		/* left margin */

		fputs("          ", f);

		/* 240 dpi */

		fputc(27, f);

		fputc('Z', f);

		fputc(n % 256, f);

		fputc(n / 256, f);

		/* graphic data */

		for (i = 0; i < n; i++)

			if (i & 1)

				fputc(bigbuf[i], f);

			else

				fputc(0, f);

		/* carriage return */

		fputc(13, f);

	}

}

#include <float.h>

#include <math.h>

test()

{

	int dx, dy, i;

	for (i = 0; i < 360; i += 30) {

		dx = 100.0 * cos((double) i * M_PI / 180.0);

		dy = 100.0 * sin((double) i * M_PI / 180.0);

		drawvector(150, 150, 150 + dx, 150 - dy, LIGHTRED);

	}

	getch();

}

drawvector(x1, y1, x2, y2, color)

int x1, y1, x2, y2, color;

{

	int dx, dy;

	double alpha, theta;

	alpha = M_PI / 18.0;

	setcolor(color);

	line(x1, y1, x2, y2);

	dx = x2 - x1;

	dy = y1 - y2;

	if (dx == 0 && dy == 0)

		return;

	/* phi = tan ^ -1 (y / x) is the angle of the vector */

	/* theta = pi / 2 - phi is the opposite angle */

	/* add and subtract a little for the arrow */

	/* sin projects onto x axis */

	/* cos projects onto y axis */

	/* both directions are negative but screen y is already negative */

	theta = M_PI_2 - atan2((double) dy, (double) dx);

	dx = 10.0 * sin(theta - alpha) + 0.5;

	dy = 10.0 * cos(theta - alpha) + 0.5;

	moveto(x2, y2);

	linerel(-dx, dy);

	dx = 10.0 * sin(theta + alpha) + 0.5;

	dy = 10.0 * cos(theta + alpha) + 0.5;

	moveto(x2, y2);

	linerel(-dx, dy);

/*	if (theta == 360) {

		pieslice(x2, y2, 0, 10, 10);

		pieslice(x2, y2, 350, 360, 10);

	} else

		pieslice(x2, y2, theta - 10, theta + 10, 10);

*/

}
