#include "stdafx.h"
#include "defs.h"

extern int text_width(int, char *);
extern void get_height_width(int *, int *, int, char *s);

static void displayf(void);
static void emit_expr(U *);
static void emit_term(U *);
static void emit_multiply(U *, int);
static void emit_factor(U *);
static void emit_power(U *);
static void emit_denominator(U *, int);
static void emit_subexpr(U *);
static void fixup_power(int, int);
static void move(int, int, int, int);
static void get_size(int, int, int *, int *, int *);
static void emit_function(U *);
static void emit_symbol(U *);
static void emit_string(U *);
static void fixup_fraction(int, int, int);
static void emit_number(U *, int);
static void emit_str(int, char *);
static void emit_char(int, int);
static int count_denominators(U *);
static void emit_fraction(U *, int);
static void emit_numerical_fraction(U *);
static void emit_tensor(U *);
static int isdenominator(U *);
static void emit_flat_tensor(U *);
static void emit_tensor_inner(U *, int, int *);
static void emit_top_expr(U *);
static void emit_index_function(U *);
static void emit_factorial_function(U *);
static void normy(void);
static int xheight(void);
static void emit_thick_space(void);
static void emit_thin_space(void);
static int mheight(void);
static int shim(void);
static int hrule_thickness(void);
static void emit_hrule(int);
static int isnegativeterm(U *);
static int isgreek(char *);
static void do_groups(void);
static void do_group(int, int);
static void emit_numerators(U *);
static void emit_denominators(U *);
static void cmdisplay_done(void);
static int subscript_dy(void);
static void emit_minus_sign_space(void);
static void emit_thin_space_maybe(void);
static void emit_paren_space_maybe(void);

#define SMALL_FONT 1
#define DEFAULT_FONT 2
#define TIMES_FONT 3
#define ITALIC_TIMES_FONT 4
#define SYMBOL_FONT 5
#define ITALIC_SYMBOL_FONT 6
#define SMALL_TIMES_FONT 7
#define SMALL_ITALIC_TIMES_FONT 8
#define SMALL_SYMBOL_FONT 9
#define SMALL_ITALIC_SYMBOL_FONT 10

#define DRAW_HRULE 20
#define DRAW_LEFT_DELIMETER 21
#define DRAW_RIGHT_DELIMETER 22

extern struct {
	int ascent, descent, width;
} text_metric[11];

#define NGREEK 34

static struct {
	char *s;
	int font, n;
} greek[NGREEK] = {
	{"Gamma",	SYMBOL_FONT,	71},
	{"Delta",	SYMBOL_FONT,	68},
	{"Theta",	SYMBOL_FONT,	81},
	{"Lambda",	SYMBOL_FONT,	76},
	{"Xi",		SYMBOL_FONT,	88},
	{"Pi",		SYMBOL_FONT,	80},
	{"Sigma",	SYMBOL_FONT,	83},
	{"Upsilon",	SYMBOL_FONT,	85},
	{"Phi",		SYMBOL_FONT,	70},
	{"Psi",		SYMBOL_FONT,	89},
	{"Omega",	SYMBOL_FONT,	87},
	{"alpha",	ITALIC_SYMBOL_FONT,	97},
	{"beta",	ITALIC_SYMBOL_FONT,	98},
	{"gamma",	ITALIC_SYMBOL_FONT,	103},
	{"delta",	ITALIC_SYMBOL_FONT,	100},
	{"epsilon",	ITALIC_SYMBOL_FONT,	101},
	{"zeta",	ITALIC_SYMBOL_FONT,	122},
	{"eta",		ITALIC_SYMBOL_FONT,	104},
	{"theta",	ITALIC_SYMBOL_FONT,	113},
	{"iota",	ITALIC_SYMBOL_FONT,	105},
	{"kappa",	ITALIC_SYMBOL_FONT,	107},
	{"lambda",	ITALIC_SYMBOL_FONT,	108},
	{"mu",		ITALIC_SYMBOL_FONT,	109},
	{"nu",		ITALIC_SYMBOL_FONT,	110},
	{"xi",		ITALIC_SYMBOL_FONT,	120},
	{"pi",		ITALIC_SYMBOL_FONT,	112},
	{"rho",		ITALIC_SYMBOL_FONT,	114},
	{"sigma",	ITALIC_SYMBOL_FONT,	115},
	{"tau",		ITALIC_SYMBOL_FONT,	116},
	{"upsilon",	ITALIC_SYMBOL_FONT,	117},
	{"phi",		ITALIC_SYMBOL_FONT,	102},
	{"chi",		ITALIC_SYMBOL_FONT,	99},
	{"psi",		ITALIC_SYMBOL_FONT,	121},
	{"omega",	ITALIC_SYMBOL_FONT,	119},
};

extern void shipout(unsigned char *, int, int);

#define YMAX 100000

static struct {
	int cmd, h, w, x, y;
	char *s;
} chartab[YMAX];

static int indx, level, xpos;

void
cmdisplay(U *p)
{
	indx = 0;
	level = 0;
	xpos = 0;
	push(p);
	displayf();
	cmdisplay_done();
}

void
eval_display(void)
{
	indx = 0;
	level = 0;
	xpos = 0;

	// special form: display(symbol)

	if (cadr(p1)->k == SYM && cadr(p1) != symbol(LAST) && cddr(p1) == nil) {
		push(cadr(p1));
		eval();
		p2 = pop();
		if (cadr(p1) != p2) {
			push_symbol(SETQ);
			push(cadr(p1));
			push(p2);
			list(3);
		} else
			push(p2);
		displayf();
	} else {
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			eval();
			p2 = pop();
			push(p2);
			displayf();
			p1 = cdr(p1);
		}
	}

	cmdisplay_done();

	push(nil);
}

static void
displayf(void)
{
	save();
	p1 = pop();
	emit_top_expr(p1);
	restore();
}

#define SEG 1000

static void
cmdisplay_done(void)
{
	int h, i, k, len, n, w, y;
	unsigned char *buf;

	do_groups();

	normy();

	get_size(0, indx, &h, &w, &y);

	// figure out how much space is needed

	n = 1; // extra for \0 that strcpy writes into buffer

	for (i = 0; i < indx; i++) {
		if (chartab[i].cmd == 32)
			continue; // we're not going to output spaces
		else if (chartab[i].s)
			n = n + 13 + (int) strlen(chartab[i].s);
		else
			n += 17;
	}

	buf = (unsigned char *) malloc(n);

	if (buf == 0)
		stop("malloc failed");

	k = 0;

	for (i = 0; i < indx; i++) {

		if (chartab[i].cmd == 32)
			continue;

		buf[k++] = (unsigned char) chartab[i].cmd;

		buf[k++] = (unsigned char) (chartab[i].x >> 24);
		buf[k++] = (unsigned char) (chartab[i].x >> 16);
		buf[k++] = (unsigned char) (chartab[i].x >> 8);
		buf[k++] = (unsigned char) chartab[i].x;

		buf[k++] = (unsigned char) (chartab[i].y >> 24);
		buf[k++] = (unsigned char) (chartab[i].y >> 16);
		buf[k++] = (unsigned char) (chartab[i].y >> 8);
		buf[k++] = (unsigned char) chartab[i].y;

		if (chartab[i].s) {
			len = (int) strlen(chartab[i].s);
			buf[k++] = (unsigned char) (len >> 24);
			buf[k++] = (unsigned char) (len >> 16);
			buf[k++] = (unsigned char) (len >> 8);
			buf[k++] = (unsigned char) len;
			strcpy((char *) buf + k, chartab[i].s);
			k += len;
		} else {
			buf[k++] = (unsigned char) (chartab[i].w >> 24);
			buf[k++] = (unsigned char) (chartab[i].w >> 16);
			buf[k++] = (unsigned char) (chartab[i].w >> 8);
			buf[k++] = (unsigned char) chartab[i].w;
			buf[k++] = (unsigned char) (chartab[i].h >> 24);
			buf[k++] = (unsigned char) (chartab[i].h >> 16);
			buf[k++] = (unsigned char) (chartab[i].h >> 8);
			buf[k++] = (unsigned char) chartab[i].h;
		}
	}

	buf[k++] = 0; // end of buffer

	shipout(buf, w, h);
}

static void
emit_top_expr(U *p)
{
	if (car(p) == symbol(SETQ)) {
		emit_expr(cadr(p));
		emit_thick_space();
		emit_char(SYMBOL_FONT, '=');
		emit_thick_space();
		emit_expr(caddr(p));
		return;
	}

	if (p->k == TENSOR)
		emit_tensor(p);
	else
		emit_expr(p);
}

static void
emit_expr(U *p)
{
	if (car(p) == symbol(ADD)) {
		save();
		p1 = cdr(p);
		if (isnegativeterm(car(p1))) {
			emit_char(SYMBOL_FONT,  '-');
			//emit_very_thin_space();
		}
		emit_term(car(p1));
		p1 = cdr(p1);
		while (iscons(p1)) {
			if (isnegativeterm(car(p1))) {
				emit_thick_space();
				emit_char(SYMBOL_FONT, '-');
				emit_thick_space();
			} else {
				emit_thick_space();
				emit_char(SYMBOL_FONT, '+');
				emit_thick_space();
			}
			emit_term(car(p1));
			p1 = cdr(p1);
		}
		restore();
	} else {
		if (isnegativeterm(p)) {
			emit_char(SYMBOL_FONT, '-');
			//emit_very_thin_space();
		}
		emit_term(p);
	}
}

static int
isnegativeterm(U *p)
{
	if (isnegativenumber(p))
		return 1;
	else if (car(p) == symbol(MULTIPLY) && isnegativenumber(cadr(p)))
		return 1;
	else
		return 0;
}

static void
emit_term(U *p)
{
	int n;
	if (car(p) == symbol(MULTIPLY)) {
		n = count_denominators(p);
		if (n && level == 0)
			emit_fraction(p, n);
		else
			emit_multiply(p, n);
	} else
		emit_factor(p);
}

static int
isdenominator(U *p)
{
	if (car(p) == symbol(POWER) && cadr(p) != symbol(E) && isnegativeterm(caddr(p)))
		return 1;
	else
		return 0;
}

static int
count_denominators(U *p)
{
	int count = 0;
	U *q;
	p = cdr(p);
	while (iscons(p)) {
		q = car(p);
		if (isdenominator(q))
			count++;
		p = cdr(p);
	}
	return count;
}

// n is the number of denominators, not counting a fraction like 1/2

static void
emit_multiply(U *p, int n)
{
	if (n == 0) {
		p = cdr(p);
		if (isplusone(car(p)) || isminusone(car(p)))
			p = cdr(p);
		emit_factor(car(p));
		p = cdr(p);
		while (iscons(p)) {
#ifndef MAC
			emit_thin_space_maybe(); // because sigma sqrt(a + b) looks funny
#endif
			emit_thin_space();
			emit_factor(car(p));
			p = cdr(p);
		}
	} else {
		emit_numerators(p);
		emit_char(TIMES_FONT, '/');
		// need grouping if more than one denominator
		if (n > 1 || isfraction(cadr(p))) {
			emit_char(TIMES_FONT, '(');
			emit_denominators(p);
			emit_char(TIMES_FONT, ')');
		} else
			emit_denominators(p);
	}
}

#define A p3
#define B p4

// sign of term has already been emitted

static void
emit_fraction(U *p, int d)
{
	int count, k1, k2, n, x;

	emit_minus_sign_space();

	save();

	A = _one;
	B = _one;

	// handle numerical coefficient

	if (cadr(p)->k == NUM) {
		push(cadr(p));
		numerator();
		absval();
		A = pop();
		push(cadr(p));
		denominator();
		B = pop();
	}

	if (cadr(p)->k == DOUBLE) {
		push(cadr(p));
		absval();
		A = pop();
	}

	// count numerators

	if (isplusone(A))
		n = 0;
	else
		n = 1;
	p1 = cdr(p);
	if (isnum(car(p1)))
		p1 = cdr(p1);
	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2))
			;
		else
			n++;
		p1 = cdr(p1);
	}

	// emit numerators

	x = xpos;

	k1 = indx;

	count = 0;

	// emit numerical coefficient

	if (!isplusone(A)) {
		emit_number(A, 0);
		count++;
	}

	// skip over "multiply"

	p1 = cdr(p);

	// skip over numerical coefficient, already handled

	if (isnum(car(p1)))
		p1 = cdr(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2))
			;
		else {
			if (count > 0)
				emit_thin_space();
			if (n == 1)
				emit_expr(p2);
			else
				emit_factor(p2);
			count++;
		}
		p1 = cdr(p1);
	}

	if (count == 0)
		emit_char(TIMES_FONT, '1');

	// emit denominators

	k2 = indx;

	count = 0;

	if (!isplusone(B)) {
		emit_number(B, 0);
		count++;
		d++;
	}

	p1 = cdr(p);

	if (car(p1)->k == NUM)
		p1 = cdr(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		if (isdenominator(p2)) {
			if (count > 0)
				emit_thin_space();
			emit_denominator(p2, d);
			count++;
		}
		p1 = cdr(p1);
	}

	fixup_fraction(x, k1, k2);

	restore();
}

// p points to a multiply

static void
emit_numerators(U *p)
{
	int n;

	save();

	p1 = _one;

	p = cdr(p);

	if (car(p)->k == NUM) {
		push(car(p));
		numerator();
		absval();
		p1 = pop();
		p = cdr(p);
	} else if (car(p)->k == DOUBLE) {
		push(car(p));
		absval();
		p1 = pop();
		p = cdr(p);
	}

	n = 0;

	if (!isplusone(p1)) {
		emit_number(p1, 0);
		n++;
	}

	while (iscons(p)) {
		if (isdenominator(car(p)))
			;
		else {
			if (n > 0)
				emit_thin_space();
			emit_factor(car(p));
			n++;
		}
		p = cdr(p);
	}

	if (n == 0)
		emit_char(TIMES_FONT, '1');

	restore();
}

// p points to a multiply

static void
emit_denominators(U *p)
{
	int n;

	save();

	n = 0;

	p = cdr(p);

	if (isfraction(car(p))) {
		push(car(p));
		denominator();
		p1 = pop();
		emit_number(p1, 0);
		n++;
		p = cdr(p);
	}

	while (iscons(p)) {
		if (isdenominator(car(p))) {
			if (n > 0)
				emit_thin_space();
			emit_denominator(car(p), 0);
			n++;
		}
		p = cdr(p);
	}

	restore();
}

static void
emit_factor(U *p)
{
	if (p->k == TENSOR) {
		if (level == 0)
			emit_tensor(p);
		else
			emit_flat_tensor(p);
		return;
	}

	if (p->k == DOUBLE) {
		emit_number(p, 0);
		return;
	}

	if (car(p) == symbol(ADD) || car(p) == symbol(MULTIPLY)) {
		emit_subexpr(p);
		return;
	}

	if (car(p) == symbol(POWER)) {
		emit_power(p);
		return;
	}

	if (iscons(p)) {
		emit_function(p);
		return;
	}

	if (isnum(p)) {
		if (level == 0)
			emit_numerical_fraction(p);
		else
			emit_number(p, 0);
		return;
	}

	if (p->k == STR) {
		emit_string(p);
		return;
	}

	emit_symbol(p);
}

static void
emit_numerical_fraction(U *p)
{
	int k1, k2, x;

	save();

	push(p);
	numerator();
	absval();
	A = pop();

	push(p);
	denominator();
	B = pop();

	if (isplusone(B)) {
		emit_number(A, 0);
		restore();
		return;
	}

	level++;
	emit_minus_sign_space();
	x = xpos;
	k1 = indx;
	emit_number(A, 0);
	k2 = indx;
	emit_number(B, 0);
	level--;

	fixup_fraction(x, k1, k2);

	restore();
}

// if it's a factor then it doesn't need parens around it, i.e. 1/sin(theta)^2

static int
isfactor(U *p)
{
	// sqrt ?

//	if (car(p) == symbol(POWER) && equal(caddr(p), plus_one_half))
//		return 1;

	if (iscons(p) && car(p) != symbol(ADD) && car(p) != symbol(MULTIPLY) && car(p) != symbol(POWER))
		return 1;
	if (issymbol(p))
		return 1;
	if (isfraction(p))
		return 0;
	if (isnegativenumber(p))
		return 0;
	if (isnum(p))
		return 1;
	if (istensor(p))
		return 1;
	return 0;
}

static void
emit_power(U *p)
{
	int k1, k2, x;

	if (cadr(p) == symbol(E)) {
		emit_str(TIMES_FONT, "exp");
		emit_char(TIMES_FONT, '(');
		emit_expr(caddr(p));
		emit_char(TIMES_FONT, ')');
		return;
	}

	if (equal(p, unit_imaginary)) {
		emit_symbol(symbol(IM)->u.sym.binding);
		return;
	}

	// special case: level > 0 which means we are already in a superscript
	//
	//                                                      1
	// so, display something like x^(-1) as 1/x instead of ---
	//                                                      x

	if (level > 0 && isminusone(caddr(p))) {
		emit_char(TIMES_FONT, '1');
		emit_char(TIMES_FONT, '/');
		if (isfactor(cadr(p)))
			emit_factor(cadr(p));
		else
			emit_subexpr(cadr(p));
		return;
	}

	// special case: something like x^(-1)
	//
	//             1              -1
	// display as --- instead of x
	//             x

	if (isnegativeterm(caddr(p))) {
		emit_minus_sign_space();
		x = xpos;
		k1 = indx;
		emit_char(TIMES_FONT, '1');
		k2 = indx;
		emit_denominator(p, 1);
		fixup_fraction(x, k1, k2);
		return;
	}

	k1 = indx;
	if (isfactor(cadr(p)))
		emit_factor(cadr(p));
	else
		emit_subexpr(cadr(p));
	k2 = indx;
	level++;
	emit_thin_space_maybe();
	emit_expr(caddr(p));
	level--;
	fixup_power(k1, k2);
}

// if n == 1 then emit as expr (no parens)

// p is a power

static void
emit_denominator(U *p, int n)
{
	int k1, k2;

	// special case: 1 over something

	if (isminusone(caddr(p))) {
		if (n == 1)
			emit_expr(cadr(p));
		else
			emit_factor(cadr(p));
		return;
	}

	k1 = indx;

	// emit base

	if (isfactor(cadr(p)))
		emit_factor(cadr(p));
	else
		emit_subexpr(cadr(p));

	k2 = indx;

	// emit exponent, don't emit minus sign

	// to get here, caddr(p) must be a term, not an expr

	// see isdenominator()

	level++;

	emit_thin_space_maybe();

	emit_term(caddr(p));

	level--;

	fixup_power(k1, k2);
}

static void
emit_function(U *p)
{
	if (car(p) == symbol(INDEX) && cadr(p)->k == SYM) {
		emit_index_function(p);
		return;
	}

	if (car(p) == symbol(FACTORIAL)) {
		emit_factorial_function(p);
		return;
	}

	emit_symbol(car(p));
	emit_char(TIMES_FONT, '(');
	p = cdr(p);
	if (iscons(p)) {
		emit_expr(car(p));
		p = cdr(p);
		while (iscons(p)) {
			emit_char(TIMES_FONT, ',');
			emit_expr(car(p));
			p = cdr(p);
		}
	}
	emit_char(TIMES_FONT, ')');
}

static void
emit_index_function(U *p)
{
	p = cdr(p);
	if (caar(p) == symbol(ADD) || caar(p) == symbol(MULTIPLY) || caar(p) == symbol(POWER) || caar(p) == symbol(FACTORIAL))
		emit_subexpr(car(p));
	else
		emit_expr(car(p));
	emit_char(TIMES_FONT, '[');
	p = cdr(p);
	if (iscons(p)) {
		emit_expr(car(p));
		p = cdr(p);
		while(iscons(p)) {
			emit_char(TIMES_FONT, ',');
			emit_expr(car(p));
			p = cdr(p);
		}
	}
	emit_char(TIMES_FONT, ']');
}

static void
emit_factorial_function(U *p)
{
	p = cadr(p);
	if (car(p) == symbol(ADD) || car(p) == symbol(MULTIPLY) || car(p) == symbol(POWER) || car(p) == symbol(FACTORIAL))
		emit_subexpr(p);
	else
		emit_expr(p);
	emit_char(TIMES_FONT, '!');
}

static void
emit_subexpr(U *p)
{
	emit_char(TIMES_FONT, '(');
	emit_expr(p);
	emit_char(TIMES_FONT, ')');
}

static void
emit_symbol(U *p)
{
	int i, k, n, w;
	char *s;

	if (!issym(p)) {	// should not get here, but just in case...
		emit_char(TIMES_FONT, '(');
		emit_expr(p);
		emit_char(TIMES_FONT, ')');
		return;
	}

	if (p == symbol(E)) {
		emit_str(TIMES_FONT, "exp");
		emit_char(TIMES_FONT, '(');
		emit_char(TIMES_FONT, '1');
		emit_char(TIMES_FONT, ')');
		return;
	}

	if (p == symbol(DERIVATIVE)) {
		emit_char(SYMBOL_FONT, 182);
		return;
	}

	s = get_printname(p);

	if (iskeyword(p)) {
		emit_str(TIMES_FONT, s);
		return;
	}

	// parse greek letters

	k = indx;

	n = isgreek(s);

	if (n == -1)
		if (isalpha(*s))
			emit_char(ITALIC_TIMES_FONT, *s++);
		else
			emit_char(TIMES_FONT, *s++);
	else {
		emit_char(greek[n].font, greek[n].n);
		s += strlen(greek[n].s);
	}

	// feynman slash?

	if (strncmp(s, "slash", 5) == 0) {
		emit_char(SYMBOL_FONT, '/');
		w = chartab[k].w - chartab[indx - 1].w;
		chartab[indx - 1].x = chartab[k].x + w / 2;
		xpos = chartab[k].x + chartab[k].w;
		s += 5;
	}

	k = indx;

	level++;

	while (*s) {
		n = isgreek(s);
		if (n == -1)
			if (isalpha(*s))
				emit_char(ITALIC_TIMES_FONT, *s++);
			else
				emit_char(TIMES_FONT, *s++);
		else {
			emit_char(greek[n].font, greek[n].n);
			s += strlen(greek[n].s);
		}
	}

	level--;

	// subscript

	for (i = k; i < indx; i++)
		chartab[i].y += subscript_dy();
}

static void
emit_string(U *p)
{
	//emit_str(TIMES_FONT, "\"");
	emit_str(TIMES_FONT, p->u.str);
	//emit_str(TIMES_FONT, "\"");
}

void
fixup_fraction(int x, int k1, int k2)
{
	int dx, dy, w;
	int h1, w1, y1;
	int h2, w2, y2;

	get_size(k1, k2, &h1, &w1, &y1);
	get_size(k2, indx, &h2, &w2, &y2);

	// We want to shift the numerator horizontally so it's centered.

	// Rounding up looks better.

	if (w2 > w1)
		dx = (w2 - w1 + 1) / 2;	// shift numerator right
	else
		dx = 0;

	// We want to shift the numerator vertically so it's above the rule.
	//
	//	h1 + shim + mheight = 1 - y
	//
	//	h1 + shim + mheight = 1 - y1 - dy
	//
	//	dy = 1 - y1 - h1 - shim - mheight

	dy = 1 - y1 - h1 - shim() - mheight();

	move(k1, k2, dx, dy);

	if (w2 > w1)
		dx = -w1;
	else
		dx = -w1 + (w1 - w2) / 2;

	//	y2 + dy = -mheight + hrule_thickness + shim
	//
	//	dy = -mheight + hrule_thickness + shim - y2

	dy = -mheight() + shim() - y2;

	move(k2, indx, dx, dy);

	if (w2 > w1)
		w = w2;
	else
		w = w1;

	xpos = x;

	emit_hrule(w);
}

//	Indices k1 through k2-1 are the glyph on the left.
//
//	Indices k2 through index-1 are the glyph to be superscripted.

static void
fixup_power(int k1, int k2)
{
	int dy;
	int h1, w1, y1;
	int h2, w2, y2;

	get_size(k1, k2, &h1, &w1, &y1);
	get_size(k2, indx, &h2, &w2, &y2);

	// move bottom of superscript to x height
	//
	// Example:
	//
	//  y = -9	****	]
	//	-8	*	]
	//	-7	***	] h2 = 5
	//	-6	*	]
	//	-5	****	]
	//	-4		]
	//	-3		]
	//	-2		] xheight = 5
	//	-1		]
	//	 0		]
	//	 1
	//
	// want y such that
	//
	//	h2 + xheight = 1 - y
	//
	// already have y2 which we need to adjust by dy so that y = y2 + dy
	//
	//	h2 + xheight = 1 - y2 - dy
	//
	//	dy = 1 - y2 - h2 - xheight

	dy = 1 - y2 - h2 - xheight();

	// if y1 < y2 + dy then boost up some more

	if (y1 < y2 + dy)
		dy = y1 - y2;

	move(k2, indx, 0, dy);
}

static void
move(int j, int k, int dx, int dy)
{
	int i;
	for (i = j; i < k; i++) {
		chartab[i].x += dx;
		chartab[i].y += dy;
	}
}

// finds the bounding rectangle and vertical position

static void
get_size(int j, int k, int *h, int *w, int *y)
{
	int i;
	int min_x, max_x, min_y, max_y;
	min_x = chartab[j].x;
	max_x = chartab[j].x + chartab[j].w - 1;
	min_y = chartab[j].y;
	max_y = chartab[j].y + chartab[j].h - 1;
	for (i = j + 1; i < k; i++) {
		if (chartab[i].x < min_x)
			min_x = chartab[i].x;
		if (chartab[i].x + chartab[i].w - 1 > max_x)
			max_x = chartab[i].x + chartab[i].w - 1;
		if (chartab[i].y < min_y)
			min_y = chartab[i].y;
		if (chartab[i].y + chartab[i].h - 1 > max_y)
			max_y = chartab[i].y + chartab[i].h - 1;
	}
	*h = max_y - min_y + 1;
	*w = max_x - min_x + 1;
	*y = min_y;
}

static void
emit_char(int font, int c)
{
	char s[2];
	s[0] = c;
	s[1] = 0;
	emit_str(font, s);
}

static void
emit_str(int font, char *s)
{
	int h, w;

	if (indx == YMAX)
		stop("The result has more than 10,000 symbols.\nPlease use print() or set tty = 1 and then repeat the computation.");

	if (strlen(s) > 4000)
		stop("The result has more than 4,000 digits.\nPlease use print() or set tty = 1 and then repeat the computation.");

	if (*s == '(' || *s == ')' || *s == '[' || *s == ']')
		emit_paren_space_maybe();

	if (level)
		font += 4;

	get_height_width(&h, &w, font, s);

	if (chartab[indx].s)
		free(chartab[indx].s);

	chartab[indx].s = strdup(s);
	chartab[indx].cmd = font;
	chartab[indx].x = xpos;
	chartab[indx].y = -text_metric[font].ascent;
	//chartab[indx].h = text_metric[font].ascent + text_metric[font].descent;
	//chartab[indx].w = text_width(font, s);

	chartab[indx].h = h;
	chartab[indx].w = w;

	xpos += chartab[indx].w;

	indx++;
}

void
emit_number(U *p, int emit_sign)
{
	int i, k1, k2, len;
	char *s;
	static char buf[100];
	switch (p->k) {
	case NUM:
		s = mstr(p->u.q.a);
		if (*s == '-' && emit_sign == 0)
			s++;
		emit_str(TIMES_FONT, s);
		s = mstr(p->u.q.b);
		if (strcmp(s, "1") == 0)
			break;
		emit_char(TIMES_FONT, '/');
		emit_str(TIMES_FONT, s);
		break;
	case DOUBLE:
		len = sprintf(buf, "%g", p->u.d);
		for (i = 0; i < len; i++)
			if (buf[i] == 'E' || buf[i] == 'e') {
				buf[i] = 0;
				break;
			}
		s = buf;
		if (*s == '-' && emit_sign == 0)
			s++;
		emit_str(TIMES_FONT, s);
		if (i < len) {
			s = buf + i + 1;
			emit_char(SYMBOL_FONT, 180);
			k1 = indx;
			emit_str(TIMES_FONT, "10");
			k2 = indx;
			level++;
			if (*s == '+')
				s++;
			else if (*s == '-') {
				emit_char(SYMBOL_FONT, '-');
				s++;
			}
			// don't emit leading zeroes
			while (*s && *s == '0')
				s++;
			if (*s)
				emit_str(TIMES_FONT, s);
			level--;
			fixup_power(k1, k2);
		}
		break;
	default:
		break;
	}
}

#define SPACE_BETWEEN_COLUMNS 20
#define SPACE_BETWEEN_ROWS 20

static void
emit_tensor(U *p)
{
	int h, i, j, k, n, w, x, y;
	int nrow, ncol;
	int dw, dx, dy;
	int col_width[100], ymin[100], ymax[100];

	struct {
		int y, h, w, index1, index2;
	} elem[10000];

	if (p->u.tensor->ndim > 2)
		stop("Tensor rank > 2 in display().\nTry using print() or set tty=1.");

	nrow = p->u.tensor->dim[0];

	if (p->u.tensor->ndim == 2)
		ncol = p->u.tensor->dim[1];
	else
		ncol = 1;

	n = nrow * ncol;

	if (nrow > 100 || ncol > 100)
		stop("too many tensor components to display, try using print() or set tty=1");

	emit_char(TIMES_FONT, '(');

	// horizontal coordinate of the matrix

	x = xpos;

	// emit each component

	for (i = 0; i < n; i++) {
		elem[i].index1 = indx;
		emit_expr(p->u.tensor->elem[i]);
		elem[i].index2 = indx;
		get_size(elem[i].index1, indx, &elem[i].h, &elem[i].w, &elem[i].y);
		xpos = x; // put all components at the same x
	}

	// calculate the height of each row

	for (i = 0; i < nrow; i++) {
		ymin[i] = elem[i * ncol].y;
		ymax[i] = elem[i * ncol].y + elem[i * ncol].h;
		for (j = 1; j < ncol; j++) {
			k = i * ncol + j;
			if (elem[k].y < ymin[i])
				ymin[i] = elem[k].y;
			y = elem[k].y + elem[k].h;
			if (y > ymax[i])
				ymax[i] = y;
		}
	}

	// calculate the width of each column

	for (i = 0; i < ncol; i++) {
		col_width[i] = elem[i].w;
		for (j = 1; j < nrow; j++) {
			k = j * ncol + i;
			if (elem[k].w > col_width[i])
				col_width[i] = elem[k].w;
		}
	}

	// this is the overall height of the matrix

	h = (nrow - 1) * SPACE_BETWEEN_ROWS;
	for (i = 0; i < nrow; i++)
		h += ymax[i] - ymin[i];

	// this is the overall width of the matrix

	w = (ncol - 1) * SPACE_BETWEEN_COLUMNS;
	for (i = 0; i < ncol; i++)
		w += col_width[i];

	// this is the y coordinate of the entire tensor

	y = -(h / 2) - mheight();

	// move rows up and down

	for (i = 0; i < nrow; i++) {
		dy = y - ymin[i];
		for (j = 0; j < ncol; j++) {
			k = i * ncol + j;
			move(elem[k].index1, elem[k].index2, 0, dy);
		}
		y += ymax[i] - ymin[i] + SPACE_BETWEEN_ROWS;
	}

	// move columns to the right

	dw = 0;
	for (i = 0; i < nrow; i++) {
		dw = 0;
		for (j = 0; j < ncol; j++) {
			k = i * ncol + j;
			dx = dw + (col_width[j] - elem[k].w) / 2;
			move(elem[k].index1, elem[k].index2, dx, 0);
			dw += col_width[j] + SPACE_BETWEEN_COLUMNS;
		}
	}

	xpos = x + w;

	emit_char(TIMES_FONT, ')');
}

static void
emit_flat_tensor(U *p)
{
	int k = 0;
	emit_tensor_inner(p, 0, &k);
}

static void
emit_tensor_inner(U *p, int j, int *k)
{
	int i;
	emit_char(TIMES_FONT, '(');
	for (i = 0; i < p->u.tensor->dim[j]; i++) {
		if (j + 1 == p->u.tensor->ndim) {
			emit_expr(p->u.tensor->elem[*k]);
			*k = *k + 1;
		} else
			emit_tensor_inner(p, j + 1, k);
		if (i + 1 < p->u.tensor->dim[j])
			emit_char(TIMES_FONT, ',');
	}
	emit_char(TIMES_FONT, ')');
}

static void
emit_hrule(int w)
{
	if (indx == YMAX)
		stop("display buffer overflow, try using print() or set tty=1");
	if (chartab[indx].s)
		free(chartab[indx].s);
	chartab[indx].s = NULL;
	chartab[indx].cmd = DRAW_HRULE;
	chartab[indx].x = xpos;
	chartab[indx].y = -mheight();
	chartab[indx].w = w;
	chartab[indx].h = hrule_thickness();
	xpos += w;
	indx++;
}

static void
emit_space(int w)
{
	if (indx == YMAX)
		stop("The result has more than 10,000 symbols.\nPlease use print() or set tty = 1 and then repeat the computation.");

	if (chartab[indx].s) {
		free(chartab[indx].s);
		chartab[indx].s = 0;
	}

	chartab[indx].cmd = 32;
	chartab[indx].x = xpos;
	chartab[indx].y = 0;
	chartab[indx].h = 0;
	chartab[indx].w = w;

	xpos += w;

	indx++;
}

static void
emit_thick_space(void)
{
	int w;
	if (level == 0)
		w = text_metric[TIMES_FONT].width;
	else
		w = text_metric[SMALL_TIMES_FONT].width;
	emit_space(w);
}

static void
emit_thin_space(void)
{
	int w;
	if (level == 0)
		w = text_metric[TIMES_FONT].width / 2;
	else
		w = text_metric[SMALL_TIMES_FONT].width / 2;
	emit_space(w);
}

// if minus sign previously then emit thin space

static void
emit_minus_sign_space(void)
{
	if (indx && chartab[indx - 1].s && strcmp(chartab[indx - 1].s, "-") == 0)
		emit_thin_space();
}

// some chars need a little extra space before superscripts

static void
emit_thin_space_maybe(void)
{
	int c, cmd, len;

	if (indx == 0 || chartab[indx - 1].s == 0)
		return;

	len = (int) strlen(chartab[indx - 1].s);

	if (len == 0)
		return;

	c = chartab[indx - 1].s[len - 1];

	cmd = chartab[indx - 1].cmd;

	if (cmd == ITALIC_SYMBOL_FONT || cmd == SMALL_ITALIC_SYMBOL_FONT) {
		if (c == 108 || c == 119) // lambda or omega
			;
		else
			emit_thin_space();
		return;
	}

	if (cmd == SYMBOL_FONT || cmd == SMALL_SYMBOL_FONT) {
		if (c == 68 || c == 76) // Lambda or Delta
			;
		else
			emit_thin_space();
		return;
	}

	if (cmd == ITALIC_TIMES_FONT || cmd == SMALL_ITALIC_TIMES_FONT) {
#ifdef MAC
		emit_thin_space();
#else
		if (c == 'f')
			emit_thick_space();
#endif
		return;
	}

	if (cmd == TIMES_FONT || cmd == SMALL_TIMES_FONT) {
		if (c >= '0' || c <= '9')
			; // numerals are ok
		else
			; // placeholder
		return;
	}
}

// some chars need a little extra space before parens (and braces)

static void
emit_paren_space_maybe(void)
{
#ifndef MAC
	int c, cmd, len;

	if (indx == 0 || chartab[indx - 1].s == 0)
		return;

	len = (int) strlen(chartab[indx - 1].s);

	if (len == 0)
		return;

	c = chartab[indx - 1].s[len - 1];

	cmd = chartab[indx - 1].cmd;

	if (cmd == ITALIC_SYMBOL_FONT || cmd == SMALL_ITALIC_SYMBOL_FONT) {
		switch (c) {
		case 98:	// beta
		case 100:	// delta
		case 102:	// phi
		case 113:	// theta
		case 120:	// xi
		case 122:	// zeta
			emit_thin_space();
			break;
		default:
			break;
		}
		return;
	}

	if (cmd == SYMBOL_FONT || cmd == SMALL_SYMBOL_FONT) {
		return;
	}

	if (cmd == ITALIC_TIMES_FONT || cmd == SMALL_ITALIC_TIMES_FONT) {
		if (c == 'f')
			emit_thin_space();
		return;
	}

	if (cmd == TIMES_FONT || cmd == SMALL_TIMES_FONT) {
		return;
	}
#endif
}

static int
xheight(void)
{
	if (level == 0)
		return text_metric[TIMES_FONT].ascent / 3;
	else
		return text_metric[SMALL_TIMES_FONT].ascent / 3;

}

static int
subscript_dy(void)
{
	if (level == 0)
		return text_metric[TIMES_FONT].ascent / 3 + 1;
	else
		return text_metric[SMALL_TIMES_FONT].ascent / 3 + 1;
}

// math height

static int
mheight(void)
{
#ifdef MAC
	if (level == 0)
		return 7;
	else
		return 5;
#else
	if (level == 0)
		return 8;
	else
		return 6;
#endif
}

static int
shim(void)
{
	return 2;
}

static int
hrule_thickness(void)
{
	return 1;
}

static int
isgreek(char *s)
{
	int i;
	for (i = 0; i < NGREEK; i++)
		if (strncmp(greek[i].s, s, strlen(greek[i].s)) == 0)
			return i;
	return -1;
}

// for each pair of delimiters adjust the size if necessary

static void
do_groups(void)
{
	int i, j, n;
	for (i = 0; i < indx; i++) {
		if (chartab[i].s == NULL)
			continue;
		if (strcmp(chartab[i].s, "(") != 0)
			continue;
		n = 0;
		for (j = i + 1; j < indx; j++) {
			if (chartab[j].s == NULL)
				continue;
			if (strcmp(chartab[j].s, "(") == 0) {
				n++;
				continue;
			}
			if (strcmp(chartab[j].s, ")") != 0)
				continue;
			if (n) {
				n--;
				continue;
			}
			do_group(i, j);
			break;
		}
	}
}

// grow delimiters, maybe

static void
do_group(int i, int j)
{
	int h, w, y, ymin, ymax;

	get_size(i, j, &h, &w, &y);

	ymin = chartab[i].y - chartab[i].h / 2;
	ymax = chartab[i].y + chartab[i].h + chartab[i].h / 3;

	if (y < ymin || y + h > ymax) {
		free(chartab[i].s);
		free(chartab[j].s);
		chartab[i].s = NULL;
		chartab[j].s = NULL;
		chartab[i].cmd = DRAW_LEFT_DELIMETER;
		chartab[j].cmd = DRAW_RIGHT_DELIMETER;
		chartab[i].y = y;
		chartab[j].y = y;
		chartab[i].h = h;
		chartab[j].h = h;
	}
}

// scan chartab and normalize y so the minimum y is 0

static void
normy(void)
{
	int i, miny;

	miny = chartab[0].y;

	for (i = 1; i < indx; i++)
		if (chartab[i].y < miny)
			miny = chartab[i].y;

	for (i = 0; i < indx; i++)
		chartab[i].y -= miny;
}

static char *s[] = {

	"format=1",
	"",

	"((a,b),(c,d))",
	"a   b\n"
	"\n"
	"c   d",

	"1/sqrt(-15)",

	"       i\n"
	"- -----------\n"
	"    1/2  1/2\n"
	"   3    5",
};

void
test_cmdisplay(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
