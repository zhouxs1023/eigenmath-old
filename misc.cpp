#include "stdafx.h"
#include "defs.h"

extern void define_symbol(char *, int);
extern void define_variable(char *, int);

//U *minus_one_half;

U *varlist;
int symbol_level;

int
is_blank(char *s)
{
	while (*s)
		if (!isspace(*s++))
			return 0;
	return 1;
}

void
list(int n)
{
	int i;
	push(nil);
	for (i = 0; i < n; i++)
		cons();
}

void
new_string(char *s)
{
	save();
	p1 = alloc();
	p1->k = STR;
	p1->u.str = strdup(s);
	push(p1);
	restore();
}

#if 0

void
solve(void)
{
	save();

	p2 = pop();
	p1 = pop();

	push(p1);
	push(p2);
	coeff();

	p3 = pop();

	if (length(p3) == 4) {

		_c->u.sym.binding = car(cdr(p3));
		_b->u.sym.binding = car(cdr(cdr(p3)));
		_a->u.sym.binding = car(cdr(cdr(cdr(p3))));

//		push(_tensor);

		scan("(-_b+(_b^2-4*_a*_c)^(1/2))/(2*_a)");
		eval();

		scan("(-_b-(_b^2-4*_a*_c)^(1/2))/(2*_a)");
		eval();

		list(3);

		restore();
		return;
	}

	if (length(p3) == 5) {

		_d->u.sym.binding = car(cdr(p3));
		_c->u.sym.binding = car(cdr(cdr(p3)));
		_b->u.sym.binding = car(cdr(cdr(cdr(p3))));
		_a->u.sym.binding = car(cdr(cdr(cdr(cdr(p3)))));

		scan("_b/_a");
		eval();
		_r->u.sym.binding = pop();

		scan("_c/_a");
		eval();
		_s->u.sym.binding = pop();

		scan("_d/_a");
		eval();
		_t->u.sym.binding = pop();

		scan("_s-_r^2/3");
		eval();
		_p->u.sym.binding = pop();

		scan("2*_r^3/27-_r*_s/3+_t");
		eval();
		_q->u.sym.binding = pop();

		scan("(-_q/2+(_q^2/4+_p^3/27)^(1/2))^(1/3)");
		eval();
		_u->u.sym.binding = pop();

		scan("(-_q/2-(_q^2/4+_p^3/27)^(1/2))^(1/3)");
		eval();
		_v->u.sym.binding = pop();

//		push(_tensor);

		scan("(_u+_v)-_r/3");
		eval();

		scan("(-(_u+_v)/2+i*3^(1/2)*(_u-_v)/2)-_r/3");
		eval();

		scan("(-(_u+_v)/2-i*3^(1/2)*(_u-_v)/2)-_r/3");
		eval();

		list(4);

		restore();
		return;
	}

//	push(_tensor);
	restore();
}

#endif

int
isposint(U *p)
{
	if (isinteger(p) && MSIGN(p->u.q.a) == 1)
		return 1;
	else
		return 0;
}

int
ispoly(U *p, U *x)
{
	if (find(p, x))
		return ispoly_expr(p, x);
	else
		return 0;
}

int
ispoly_expr(U *p, U *x)
{
	if (isadd(p)) {
		p = cdr(p);
		while (iscons(p)) {
			if (!ispoly_term(car(p), x))
				return 0;
			p = cdr(p);
		}
		return 1;
	} else
		return ispoly_term(p, x);
}

int
ispoly_term(U *p, U *x)
{
	if (car(p) == symbol(MULTIPLY)) {
		p = cdr(p);
		while (iscons(p)) {
			if (!ispoly_factor(car(p), x))
				return 0;
			p = cdr(p);
		}
		return 1;
	} else
		return ispoly_factor(p, x);
}

int
ispoly_factor(U *p, U *x)
{
	if (equal(p, x))
		return 1;
	if (car(p) == symbol(POWER) && equal(cadr(p), x)) {
		if (isposint(caddr(p)))
			return 1;
		else
			return 0;
	}
	if (find(p, x))
		return 0;
	else
		return 1;
}

#if 0

int
is_unit_imaginary(U *p)
{
	if (equal(p, unit_imaginary))
		return 1;
	else
		return 0;
}

#endif

void
out_of_memory(void)
{
	stop("out of memory");
}

void
append(void)
{
	int h;

	save();

	p2 = pop();
	p1 = pop();

	h = tos;

	while (iscons(p1)) {
		push(car(p1));
		p1 = cdr(p1);
	}

	while (iscons(p2)) {
		push(car(p2));
		p2 = cdr(p2);
	}

	list(tos - h);

	restore();
}

void
push_zero_matrix(int i, int j)
{
	push(alloc_tensor(i * j));
	stack[tos - 1]->u.tensor->ndim = 2;
	stack[tos - 1]->u.tensor->dim[0] = i;
	stack[tos - 1]->u.tensor->dim[1] = j;
}

void
push_identity_matrix(int n)
{
	int i;
	push_zero_matrix(n, n);
	for (i = 0; i < n; i++)
		stack[tos - 1]->u.tensor->elem[i * n + i] = _one;
}

void
push_cars(U *p)
{
	while (iscons(p)) {
		push(car(p));
		p = cdr(p);
	}
}

void
peek(void)
{
	save();
	p1 = pop();
	push(p1);
	printline(p1);
	restore();
}

void
peek2(void)
{
	print_lisp(stack[tos - 2]);
	print_lisp(stack[tos - 1]);
}

void
cons(void)
{
	save();
	p3 = alloc();
	p2 = pop();
	p1 = pop();
	p3->k = CONS;
	p3->u.cons.car = p1;
	p3->u.cons.cdr = p2;
	push(p3);
	restore();
}

extern void run(char *);

#if 0
void
read_file(char *s)
{
	int n;
	FILE *f;
	f = fopen(s, "r");
	if (f == NULL) {
		printf("cannot open file %s\n", s);
		return;
	}
	n = (int) fread(program_buf, 1, MAX_PROGRAM_SIZE, f); // WARNING size_t
	fclose(f);
	if (n == MAX_PROGRAM_SIZE) {
		printstr("program file is too long");
		return;
	}
	program_buf[n] = 0;
	run(program_buf);
}
#endif

int
equal(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) == 0)
		return 1;
	else
		return 0;
}

int
lessp(U *p1, U *p2)
{
	if (cmp_expr(p1, p2) < 0)
		return 1;
	else
		return 0;
}

int
sign(int n)
{
	if (n < 0)
		return -1;
	else if (n > 0)
		return 1;
	else
		return 0;
}

int
cmp_expr(U *p1, U *p2)
{
	int n;

	if (p1 == p2)
		return 0;

	if (p1 == nil)
		return -1;

	if (p2 == nil)
		return 1;

	if (isnum(p1) && isnum(p2))
		return sign(compare_numbers(p1, p2));

	if (isnum(p1))
		return -1;

	if (isnum(p2))
		return 1;

	if (isstr(p1) && isstr(p2))
		return sign(strcmp(p1->u.str, p2->u.str));

	if (isstr(p1))
		return -1;

	if (isstr(p2))
		return 1;

	if (issym(p1) && issym(p2))
		return sign(strcmp(get_printname(p1), get_printname(p2)));

	if (issym(p1))
		return -1;

	if (issym(p2))
		return 1;

	if (istensor(p1) && istensor(p2))
		return compare_tensors(p1, p2);

	if (istensor(p1))
		return -1;

	if (istensor(p2))
		return 1;

	while (iscons(p1) && iscons(p2)) {
		n = cmp_expr(car(p1), car(p2));
		if (n != 0)
			return n;
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	if (iscons(p2))
		return -1;

	if (iscons(p1))
		return 1;

	return 0;
}

int
length(U *p)
{
	int n = 0;
	while (iscons(p)) {
		p = cdr(p);
		n++;
	}
	return n;
}

static void unique_f(U *);

U *
unique(U *p)
{
	save();
	p1 = nil;
	p2 = nil;
	unique_f(p);
	if (p2 != nil)
		p1 = nil;
	p = p1;
	restore();
	return p;
}

static void
unique_f(U *p)
{
	if (p->k == SYM) {
		if (p1 == nil)
			p1 = p;
		else if (p != p1)
			p2 = p;
		return;
	}
	while (iscons(p)) {
		unique_f(car(p));
		if (p2 != nil)
			return;
		p = cdr(p);
	}
}

#if 0

void
check_endianess(void)
{
	int tmp = 1;
	if (((char *) &tmp)[0] == 1 && Y_LITTLE_ENDIAN == 0) {
		printf("Please change Y_LITTLE_ENDIAN to 1 in defs.h and recompile.\n");
		Exit(1);
	}
	if (((char *) &tmp)[0] == 0 && Y_LITTLE_ENDIAN != 0) {
		printf("Please change Y_LITTLE_ENDIAN to 0 in defs.h and recompile.\n");
		Exit(1);
	}
}

#endif

void
ssqrt(void)
{
	push_rational(1, 2);
	power();
}

void
slog(void)
{
	double d;

	save();

	p1 = pop();

	if (p1->k == DOUBLE) {
		d = p1->u.d;
		if (d < 0.0) {
			d = log(-d);
			push_double(d);
			push(unit_imaginary);
			push_symbol(PI);
			multiply();
			add();
		} else {
			d = log(d);
			push_double(d);
		}
		restore();
		return;
	}

	if (equal(p1, _one)) {
		push_integer(0);
		restore();
		return;
	}

	if (p1 == symbol(E)) {
		push(_one);
		restore();
		return;
	}

	if (car(p1) == symbol(POWER)) {
		push(caddr(p1));
		push(cadr(p1));
		slog();
		multiply();
		restore();
		return;
	}

	push_symbol(LOG);
	push(p1);
	list(2);
	restore();
}

void
conjugate(void)
{
	int tmp;
	tmp = conjugating;
	conjugating = 1;
	eval();
	conjugating = tmp;
}

void
expand(void)
{
	int x;
	x = expanding;
	expanding = 1;
	eval();
	expanding = x;
}

void
exponential(void)
{
	push_symbol(E);
	swap();
	power();
}

void
square(void)
{
	push_integer(2);
	power();
}

// save and restore symbols for formal arg lists, prog vars, etc.

int
save_symbols(int n)
{
	int h, i;

	save();

	h = tos - n;

	for (i = 0; i < n; i++) {
		p1 = stack[h + i];
		if (p1->k != SYM)
			continue;
		push(p1);
		push(p1->u.sym.binding);
		push(p1->u.sym.binding2);
	}

	list(tos - h - n);

	push(varlist);

	cons();

	varlist = pop();

	tos = h;

	restore();

	symbol_level++;

	return symbol_level - 1;
}

void
restore_symbols_f(void)
{
	save();

	p1 = varlist;

	varlist = cdr(p1);

	p1 = car(p1);

	while (iscons(p1)) {
		p2 = car(p1);
		p2->u.sym.binding = cadr(p1);
		p2->u.sym.binding2 = caddr(p1);
		p1 = cdddr(p1);
	}

	restore();

	symbol_level--;
}

void
restore_symbols(int mark)
{
	while (symbol_level > mark)
		restore_symbols_f();
}

static int
__cmp(const void *p1, const void *p2)
{
	return cmp_expr(*((U **) p1), *((U **) p2));
}

void
sort_stack(int n)
{
	qsort(stack + tos - n, n, sizeof (U *), __cmp);
}

int
isnegativeterm(U *p)
{
	if (isnum(p))
		return isnegativenumber(p);
	else if (car(p) == symbol(MULTIPLY) && isnum(cadr(p)))
		return isnegativenumber(cadr(p));
	else
		return 0;
}

int
isimaginarynumber(U *p)
{
	if ((car(p) == symbol(MULTIPLY)
	&& length(p) == 3
	&& isnum(cadr(p))
	&& equal(caddr(p), unit_imaginary))
	|| equal(p, unit_imaginary))
		return 1;
	else 

		return 0;
}

int
iscomplexnumber(U *p)
{
	if ((car(p) == symbol(ADD)
	&& length(p) == 3
	&& isnum(cadr(p))
	&& isimaginarynumber(caddr(p)))
	|| isimaginarynumber(p))
		return 1;
	else
		return 0;
}

extern int nsym;
extern U symtab[];

void
clear_mem(void)
{
	int i;
	U *p;
	for (i = 0; i < nsym; i++) {
		p = symtab + i;
		if (p->k == SYM) {
			p->u.sym.binding = p;
			p->u.sym.binding2 = nil;
		}
	}
	defn();
}
