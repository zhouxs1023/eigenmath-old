/* This program loads a scanned letter (VGA screen dump) and writes
bitmap and metric files. */

#include <stdio.h>
#include <graphics.h>
#include <dos.h>

#define MINX 20
#define MAXX 100

#define MINY 20
#define MAXY 120

main(argc, argv)
int argc;
char *argv[];
{
	graphics();

	input(argv[1]);

	output();

	closegraph();
}

/* go to graphics mode */

graphics()
{
	int i, t, y, graphmode = 0, graphdriver = 0;
	union REGS regs;

	registerbgidriver(CGA_driver);
	registerbgidriver(EGAVGA_driver);
	registerbgidriver(Herc_driver);
	registerbgidriver(ATT_driver);
	registerbgidriver(PC3270_driver);
	registerbgidriver(IBM8514_driver);

	initgraph(&graphdriver, &graphmode, "");

	switch (graphdriver) {
	case VGA:
	case EGA:
		setbkcolor(1);
		break;
	case EGAMONO:
		setcolor(1);
		break;
	default:
		break;
	}

	/* init mouse driver */

/*	if (graphdriver == 7) {

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
		printf("no mouse\n");
		exit();
	}
*/

}

input(s)
char *s;
{
	int c, x, y;
	FILE *f;

	f = fopen(s, "rb");

	fgetc(f);
	fgetc(f);
	fgetc(f);
	fgetc(f);
	fgetc(f);
	fgetc(f);
	fgetc(f);

	for (y = 0; y < 200; y++)
		for (x = 0; x < 640; x += 8) {

			c = fgetc(f);

			if (c & 0x80)
				putpixel(x, y, getcolor());

			if (c & 0x40)
				putpixel(x + 1, y, getcolor());

			if (c & 0x20)
				putpixel(x + 2, y, getcolor());

			if (c & 0x10)
				putpixel(x + 3, y, getcolor());

			if (c & 0x08)
				putpixel(x + 4, y, getcolor());

			if (c & 0x04)
				putpixel(x + 5, y, getcolor());

			if (c & 0x02)
				putpixel(x + 6, y, getcolor());

			if (c & 0x01)
				putpixel(x + 7, y, getcolor());
		}

	fclose(f);
}

output()
{
	int b, l, r, t;

	t = top();

	b = bottom();

	l = left();

	r = right();

	write_bitmap(l, t, r, b);

	write_metric(l, t, r, b);
}

/* find left side */

left()
{
	int x, y;

	for (x = MINX; x < MAXX; x++)

		for (y = MINY; y < MAXY; y++)

			if (getpixel(x, y))

				return x;
}

/* find right side */

right()
{
	int x, y;

	for (x = MAXX; x > MINX; x--)

		for (y = MINY; y < MAXY; y++)

			if (getpixel(x, y))

				return x;
}

/* find top side */

top()
{
	int x, y;

	for (y = MINY; y < MAXY; y++)

		for (x = MINX; x < MAXX; x++)

			if (getpixel(x, y))

				return y;
}

/* find bottom side */

bottom()
{
	int x, y;

	for (y = MAXY; y > MINY; y--)

		for (x = MINX; x < MAXX; x++)

			if (getpixel(x, y))

				return y;
}

write_bitmap(x1, y1, x2, y2)
int x1, y1, x2, y2;
{
	int i, n, y;
	FILE *f;

	n = (x2 - x1 + 8) / 8;

	for (y = y1; y <= y2; y++) {

		for (i = 0; i < n; i++)

			printf(" db %d\n", scanbyte(x1 + 8 * i, y));
	}
}

scanbyte(x, y)
int x, y;
{
	int i, b = 0;

	for (i = 0; i < 8; i++) {
		if (getpixel(x + i, y))
			b |= 0x100;
		b >>= 1;
	}

	return b;
}

write_metric(x1, y1, x2, y2)
int x1, y1, x2, y2;
{
	int n;
	FILE *f;

	n = (x2 - x1 + 8) / 8;

	n *= y2 - y1 + 1;

	f = fopen("temp", "a");

	fprintf(f, "%d %d %d\n", n, y1, x2 - x1 + 1);

	fclose(f);
}
