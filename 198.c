/* Experimental code that formats a mathematical expression. */

#include "font.h"

#define THICK_HLINE 128
#define THIN_HLINE 129

#define MAXX 1000
#define MAXY 1000

char buf[MAXY][MAXX];

struct glyph {
	int c, font, x, y, h, d, w;
} fmt_tab[1000];

int x_offset, y_offset;
int min_x, max_x, min_y, max_y;
int fmt_index, fmt_level, fmt_x;
char *fmtin;

fmt(char *s)
{
	fmtin = s;
	fmt_index = 0;
	fmt_level = 0;
	fmt_x = 0;
	fmt_scan_fraction();
	setup_offsets();
	draw();
}

fmt_scan_fraction()
{
	int k1, k2, x;
	while (*fmtin) {
		k1 = fmt_index;
		x = fmt_x;
		fmt_scan_power();
		while (*fmtin == '/') {
			fmtin++;
			if (*fmtin == 0)
				fmt_emit_char('/');
			else {
				k2 = fmt_index;
				fmt_x = x;
				fmt_scan_power();
				fmt_emit_fraction(x, k1, k2);
			}
		}
	}
}

fmt_scan_power()
{
	int k1, k2;
	k1 = fmt_index;
	if (*fmtin == '(')
		fmt_scan_subexpr();
	else
		fmt_emit_char(*fmtin++);
	if (*fmtin == '^') {
		fmtin++;
		if (*fmtin == 0)
			fmt_emit_char('^');
		else {
			k2 = fmt_index;
			fmt_level++;
			fmt_scan_power();
			fmt_level--;
			fmt_emit_power(k1, k2);
		}
	}
}

fmt_scan_subexpr()
{
	int k, tmp, x;
	k = fmt_index;
	fmtin++;
	x = fmt_x;
	x += fmt_char_width('(');
	while (*fmtin && *fmtin != ')')
		fmt_scan_fraction();
	tmp = fmt_x;
	fmt_x = x;
	fmt_emit_char('(');
	fmt_x = tmp;
	if (*fmtin == ')') {
		fmt_emit_char(')');
		fmtin++;
	}
}

fmt_emit_fraction(int x, int k1, int k2)
{
	int dx, dy, k;
	int d1, h1, w1;
	int d2, h2, w2;

printf("emit fraction k1=%d k2=%d\n", k1, k2);

	fmt_size(k1, k2, &h1, &d1, &w1);
	fmt_size(k2, fmt_index, &h2, &d2, &w2);

	if (w2 > w1)
		dx = (w2 - w1) / 2;
	else
		dx = 0;

	dy = -d1 - fmt_center_h() - fmt_hline_pad();

	fmt_move(k1, k2, dx, dy);

	if (w2 > w1)
		dx = 0;
	else
		dx = (w1 - w2) / 2;

	dy = h2 - fmt_center_h() + fmt_hline_pad();

	fmt_move(k2, fmt_index, dx, dy);

	k = fmt_index++;

	if (fmt_level == 0)
		fmt_tab[k].c = THICK_HLINE;
	else
		fmt_tab[k].c = THIN_HLINE;

	fmt_tab[k].x = x;
	fmt_tab[k].y = -fmt_center_h();

	if (w1 > w2)
		fmt_tab[k].w = w1;
	else
		fmt_tab[k].w = w2;

	fmt_x = x + fmt_tab[k].w;
}

fmt_emit_power(int k1, int k2)
{
	int d1, h1, w1;
	int d2, h2, w2;

	fmt_size(k1, k2, &h1, &d1, &w1);
	fmt_size(k2, fmt_index, &h2, &d2, &w2);

	fmt_move(k2, fmt_index, 0, -x_height());
}

fmt_move(int j, int k, int dx, int dy)
{
	int i;
	for (i = j; i < k; i++) {
		fmt_tab[i].x += dx;
		fmt_tab[i].y += dy;
	}
}

fmt_size(int j, int k, int *h, int *d, int *w)
{
	int i;
	int min_x, max_x, min_y, max_y;
	min_x = 10000;
	max_x = -10000;
	min_y = 10000;
	max_y = -10000;
	for (i = j; i < k; i++) {
		if (fmt_tab[i].x < min_x)
			min_x = fmt_tab[i].x;
		if (fmt_tab[i].x + fmt_tab[i].w > max_x)
			max_x = fmt_tab[i].x + fmt_tab[i].w;
		if (fmt_tab[i].y - fmt_tab[i].h < min_y)
			min_y = fmt_tab[i].y - fmt_tab[i].h;
		if (fmt_tab[i].y + fmt_tab[i].d > max_y)
			max_y = fmt_tab[i].y + fmt_tab[i].d;
	}
	*h = -min_y;
	*d = max_y;
	*w = max_x - min_x;

printf("fmt size h=%d d=%d w=%d\n", *h, *d, *w);
}

fmt_emit_char(int c)
{
	int font, h, d;
	METRIC *p;
	if (fmt_level == 0) {
		font = 9;
		p = cmmi10_metric;
	} else {
		font = 10;
		p = cmmi7_metric;
	}
	fmt_tab[fmt_index].c = c;
	fmt_tab[fmt_index].font = font;
	fmt_tab[fmt_index].x = fmt_x;
	fmt_tab[fmt_index].y = 0;
	h = p[c].y_offset + 1;
	if (h < 0)
		h = 0;
	fmt_tab[fmt_index].h = h;
	d = p[c].nrow - h;
	if (d < 0)
		d = 0;
	fmt_tab[fmt_index].d = d;
	fmt_tab[fmt_index].w = p[c].ncol;
	fmt_x += p[c].ncol;
	fmt_index++;
fmt_x += space();
}

fmt_center_h()
{
	METRIC *p;
	if (fmt_level == 0)
		p = cmmi10_metric;
	else
		p = cmmi7_metric;
	return 5;
}

fmt_char_width(int c)
{
	METRIC *p;
	if (fmt_level == 0)
		p = cmmi10_metric;
	else
		p = cmmi7_metric;
	return p[c].ncol;
}

fmt_hline_pad()
{
	return 1;
}

space()
{
	if (fmt_level == 0)
		return 5;
	else
		return 3;
}

x_height()
{
	METRIC *p;
	if (fmt_level == 0)
		p = cmmi10_metric;
	else
		p = cmmi7_metric;
	return p['x'].nrow;
}

draw_char(struct glyph *g)
{
	int b, i, j, m, ncol, nrow, x, y;
	METRIC *metric;
	char *bitmap, *p;

	if (g->c == THICK_HLINE || g->c == THIN_HLINE) {
		draw_line(g);
		return;
	}

	metric = font[g->font].metric;
	bitmap = font[g->font].bitmap;
	x = g->x;
	y = g->y - metric[g->c].y_offset;
	p = bitmap + metric[g->c].index;
	ncol = metric[g->c].ncol;
	nrow = metric[g->c].nrow;
	b = *p++;
	m = 0x80;
	for (i = 0; i < nrow; i++)
		for (j = 0; j < ncol; j++) {
			if (b & m)
				set_pixel(x + j, y + i);
			m >>= 1;
			if (m == 0) {
				m = 0x80;
				b = *p++;
			}
		}
}

draw_line(struct glyph *g)
{
	int i, j;
	int dx, dy, x, y;
	x = g->x;
	y = g->y;
	dx = g->w;
	dy = 1;
	if (dx < 0) {
		dx = -dx;
		x -= dx;
	}
	if (dy < 0) {
		dy = -dy;
		y -= dy;
	}
	for (i = 0; i < dy; i++)
		for (j = 0; j < dx; j++)
			set_pixel(x + j, y + i);
}

set_pixel(x, y)
int x, y;
{
	x += x_offset;
	y += y_offset;
	if (x < 0 || x >= MAXX || y < 0 || y >= MAXY)
		return;
	buf[y][x] = 1;
/*
	if (x < min_x)
		min_x = x;
	if (x > max_x)
		max_x = x;
	if (y < min_y)
		min_y = y;
	if (y > max_y)
		max_y = y;
*/
}

setup_offsets()
{
	int i;
//	int min_x, max_x, min_y, max_y;
	min_x = 10000;
	max_x = -10000;
	min_y = 10000;
	max_y = -10000;
	for (i = 0; i < fmt_index; i++) {
		if (fmt_tab[i].x < min_x)
			min_x = fmt_tab[i].x;
		if (fmt_tab[i].x + fmt_tab[i].w > max_x)
			max_x = fmt_tab[i].x + fmt_tab[i].w;
		if (fmt_tab[i].y - fmt_tab[i].h < min_y)
			min_y = fmt_tab[i].y - fmt_tab[i].h + 1;
		if (fmt_tab[i].y + fmt_tab[i].d > max_y)
			max_y = fmt_tab[i].y + fmt_tab[i].d;
	}
	x_offset = 0;
	y_offset = -min_y;
printf("min_y %d\n", min_y);
}

draw()
{
	int i;
	for (i = 0; i < fmt_index; i++)
		draw_char(fmt_tab + i);
}

main(int argc, char *argv[])
{
	int i, j;

	fmt(argv[1]);

	for (i = 0; i < fmt_index; i++)
		printf("c=%d font=%d x=%d y=%d h=%d d=%d w=%d\n",
			fmt_tab[i].c,
			fmt_tab[i].font,
			fmt_tab[i].x,
			fmt_tab[i].y,
			fmt_tab[i].h,
			fmt_tab[i].d,
			fmt_tab[i].w);

	printf("min_x=%d max_x=%d min_y=%d max_y=%d\n", min_x, max_x, min_y, max_y);

	for (i = 0; i < 50; i++) {
		printf("%2d ", i);
		for (j = 0; j < 72; j++)
			if (buf[i][j] == 1)
				printf("*");
			else
				printf(".");
		printf("\n");
	}
}

test(int c)
{
	printf("x_offset=%d\n", cmmi10_metric['x'].x_offset);
	printf("y_offset=%d\n", cmmi10_metric['x'].y_offset);
	printf("nrow=%d\n", cmmi10_metric['x'].nrow);
	printf("ncol=%d\n", cmmi10_metric['x'].ncol);

	printf("x_offset=%d\n", cmr10_metric['x'].x_offset);
	printf("y_offset=%d\n", cmr10_metric['x'].y_offset);
	printf("nrow=%d\n", cmr10_metric['x'].nrow);
	printf("ncol=%d\n", cmr10_metric['x'].ncol);
}
