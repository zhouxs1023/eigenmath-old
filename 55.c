/* This program draws a vector that you can drag around. */

#include <stdio.h>

#include <stdlib.h>

#include <graphics.h>

#include <dos.h>

int graphmode;

int graphdriver;

int mousex;

int mousey;

int click;

int bx;

int font;

int ox = 320;

int oy = 250;

graphics()

{

	int i, t, y;

	union REGS regs;

/*	registerbgidriver(CGA_driver);

	registerbgidriver(EGAVGA_driver);

	registerbgidriver(Herc_driver);

	registerbgidriver(ATT_driver);

	registerbgidriver(PC3270_driver);

	registerbgidriver(IBM8514_driver);
*/
	initgraph(&graphdriver, &graphmode, "c:\tc\");

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

/*	font = installuserfont("simp.chr"); */

	font = installuserfont("c:\tc\litt.chr");

	setpalette(0, 0x10);

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


#include <float.h>

#include <math.h>

main()

{

	int dx, dy, i, x, x2, y, y2;

	graphics();

	pointeroff();

	setusercharsize(2, 1, 1, 1);

	settextstyle(installuserfont("tscr.chr"), 0, 0);

	setlinestyle(0, 0, 3);

	outtextxy(320 - textwidth("Vector") / 2, 0, "Vector");

	setlinestyle(0, 0, 1);

	setusercharsize(2, 1, 2, 1);

	settextstyle(font, 0, 0);

	setfillstyle(1, BLUE);

	x = random(100);

	y = random(100);

	setwritemode(0);

	drawaxes();

	drawcoordinates(x, y);

	drawarrow(ox, oy, ox + x, oy - y);

	pointeron();

	for (;;) {

		if (kbhit()) {
			closegraph();
			exit(1);
		}

		check_mouse();

		if (click && inrange()) {

			dx = abs(mousex - ox - x);

			dy = abs(oy - mousey - y);

			if (dx < 10 && dy < 10) {

				pointeroff();

				setcolor(BLACK);

				drawcoordinates(x, y);

				drawarrow(ox, oy, ox + x, oy - y);

				setcolor(WHITE);

				drawaxes();

				setwritemode(1);

				drawcoordinates(x, y);

				pointeron();

				while (bx) {

					check_mouse();

					x2 = mousex - ox;

					y2 = oy - mousey;

					if (x2 < -200)

						x2 = -200;

					if (x2 > 200)

						x2 = 200;

					if (y2 < -200)

						y2 = -200;

					if (y2 > 200)

						y2 = 200;

					if (x != x2 || y != y2) {

						pointeroff();

						drawcoordinates(x, y);

						x = x2;

						y = y2;

						drawcoordinates(x, y);

						pointeron();

					}

				}

				pointeroff();

				setwritemode(0);

				drawcoordinates(x, y);

				drawarrow(ox, oy, ox + x, oy - y);

				pointeron();

			}

		}

	}

}

/*

	for (i = 0; i < 360; i += 30) {

		dx = 100.0 * cos((double) i * M_PI / 180.0);

		dy = 100.0 * sin((double) i * M_PI / 180.0);

		drawarrow(320, 200, 320 + dx, 200 - dy, RED);

	}

	getch();

	closegraph();

	exit(1);

*/

drawarrow(x1, y1, x2, y2)

int x1, y1, x2, y2;

{

	int dx, dy;

	double alpha, theta;

	alpha = M_PI / 18.0;

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

sformat(s, x)

char *s;

int x;

{

	if (x < 0)

		sprintf(s, "-%d.%d", -x / 100, -x % 100);

	else

		sprintf(s, "%d.%d", x / 100, x % 100);

}

drawcoordinates(x, y)

int x, y;

{

	char sx[10], sy[10];

	setlinestyle(4, 0x1111, 1);

	line(ox, oy - y, ox + x, oy - y);

	line(ox + x, oy, ox + x, oy - y);

	setlinestyle(0, 0, 1);

	line(ox, oy, ox + x, oy - y);

	sformat(sx, x);

	sformat(sy, y);

	if (y >= 0)

		outtextxy(ox + x - textwidth(sx) / 2, oy, sx);

	if (y < 0)

		outtextxy(ox + x - textwidth(sx) / 2, oy - textheight(sx) - 2, sx);

	if (x >= 0)

		outtextxy(ox - textwidth(sy), oy - y - textheight(sy), sy);

	if (x < 0)

		outtextxy(ox + 2, oy - y - textheight(sy), sy);

}

drawaxes()

{

	setcolor(GREEN);

	rectangle(ox - 200, oy - 200, ox + 200, oy + 200);

	/* x axis */

	line(ox - 200, oy, ox + 200, oy);

	/* y axis */

	line(ox, oy - 200, ox, oy + 200);

	setcolor(WHITE);

}

inrange()

{

	int x, y;

	x = mousex - ox;

	y = mousey - oy;

	return -200 <= x && x <= 200 && -200 <= y && y <= 200;

}
