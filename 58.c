/* Additional module for drawing text using Macintosh fonts.
Must be linked with SDL drawing tool.  Font files must also
be linked, like ``geneva12.obj.'' */

#include <stdio.h>

typedef struct {
	unsigned int offset;
	unsigned char dy, w;
} METRIC;

extern METRIC far geneva12_metric[256];
extern unsigned char far geneva12_bitmap[];

extern METRIC far geneva14_metric[256];
extern unsigned char far geneva14_bitmap[];

extern METRIC far geneva18_metric[256];
extern unsigned char far geneva18_bitmap[];

extern METRIC far geneva24_metric[256];
extern unsigned char far geneva24_bitmap[];

METRIC *metric = geneva18_metric;
unsigned char *bitmap = geneva18_bitmap;

drawstring(x, y, s, italic, bold)
int x, y;
unsigned char *s;
int italic, bold;
{
	while (*s) {
		if (*s == ' ')
			x += metric['n'].w + 3;
		else {
			x += 2;
			drawchar(x, y, *s, italic, bold);
			x += metric[*s].w + 1;
		}
		s++;
	}
}

stringwidth(s)
char *s;
{
	int w = 0;
	if (s == NULL)
		return 0;
	while (*s)
		w += metric[*s++].w + 3;
	return w;
}

drawchar(x, y, c, italic, bold)
int x, y, c, italic, bold;
{
	int color, dx, dy, h, i, j, k, n, w, x2, y2;
	unsigned int m, offset;

	w = (metric[c].w + 7) / 8;

	if (w == 0)
		return;

	offset = metric[c].offset;

	n = metric[c + 1].offset - offset;

	h = metric[c].dy;

	color = getcolor();

	for (i = 0; i < n; i++) {

		m = bitmap[offset + i];

		dx = (i % w) * 8;
		dy = (i / w) - h;

		x2 = x + dx - (italic * dy) / 2;
		y2 = y + dy;

		for (k = 0; k < 8; k++) {

			if (m & 1) {
				putpixel(x2 + k, y2, color ^ getpixel(x2 + k, y2));

/*				if (bold)
					putpixel(x2 + k + 1, y2, color);
*/			}
			m >>= 1;
		}
	}
}

charwidth(c)
int c;
{
	if (c == ' ')
		return metric['n'].w + 3;
	else
		return metric[c].w + 3;
}
