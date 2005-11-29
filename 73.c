/* This program loads a ``dviscr'' screen dump and writes a bit map
file representing the central object. */

#include <stdio.h>
#include <graphics.h>
#include <dos.h>

#define MINX 0
#define MAXX 639

#define MINY 0
#define MAXY 399

main(argc, argv)
int argc;
char *argv[];
{
	graphics();

	setwritemode(1);

	input(argv[1]);

	output();

	closegraph();
}

/* go to graphics mode */

graphics()
{
	int graphmode = 0, graphdriver = 0;

	initgraph(&graphdriver, &graphmode, "c:\\tc");
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

	for (y = 0; y < 400; y++)
		for (x = 0; x < 640; x += 8) {

			c = ~fgetc(f);

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

	printf("%d pixels wide\n", r - l + 1);

	printf("%d pixels high\n", b - t + 1);

	write_bitmap(l, r, t, b);
}

/* find left side */

left()
{
	int x, y;

	for (x = MINX; x <= MAXX; x++) {

		line(x, MINY, x, MAXY);

		for (y = MINY; y <= MAXY; y++)

			if (getpixel(x, y) == 0) {

				line(x, MINY, x, MAXY);

				return x;
			}

		line(x, MINY, x, MAXY);
	}
}

/* find right side */

right()
{
	int x, y;

	for (x = MAXX; x >= MINX; x--) {

		line(x, MINY, x, MAXY);

		for (y = MINY; y <= MAXY; y++)

			if (getpixel(x, y) == 0) {

				line(x, MINY, x, MAXY);

				return x;
			}

		line(x, MINY, x, MAXY);
	}
}

/* find top side */

top()
{
	int x, y;

	for (y = MINY; y <= MAXY; y++) {

		line(MINX, y, MAXX, y);

		for (x = MINX; x <= MAXX; x++)

			if (getpixel(x, y) == 0) {

				line(MINX, y, MAXX, y);

				return y;
			}

		line(MINX, y, MAXX, y);
	}
}

/* find bottom side */

bottom()
{
	int x, y;

	for (y = MAXY; y >= MINY; y--) {

		line(MINX, y, MAXX, y);

		for (x = MINX; x <= MAXX; x++)

			if (getpixel(x, y) == 0) {

				line(MINX, y, MAXX, y);

				return y;
			}

		line(MINX, y, MAXX, y);
	}
}

/* run length format */

write_bitmap(x1, x2, y1, y2)
int x1, x2, y1, y2;
{
	int b, n, x, y;

	for (y = y1; y <= y2; y++) {

		b = getpixel(x1, y);

		printf("%d", b);

		n = 1;

		for (x = x1 + 1; x <= x2; x++) {

			if (b == getpixel(x, y))
				n++;
			else {
				printf(" %d", n);
				n = 1;
				b = getpixel(x, y);
			}
		}

		printf(" %d\n", n);
	}
}
