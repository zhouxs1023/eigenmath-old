/* 9-20-00 add "if" function */

#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MEM 3000000 /* (3,000,000 nodes) * (12 bytes/node) = 36 megabytes */
#define TOS 100000
#define BUF 1000

#define STRBUF 10000

#define CONS		0
#define NUM		1
#define STR		2
#define SYM		3
#define ZAND		4
#define ZAPPEND		5
#define ZARGLIST	6
#define ZATOM		7
#define ZCAR		8
#define ZCDR		9
#define ZCOMPONENT	10
#define ZCOND		11
#define ZCONS		12
#define ZCONTRACT	13
#define ZDEFINE		14
#define ZDERIVATIVE	15
#define ZDOT		16
#define ZEQUAL		17
#define ZEVAL		18
#define ZEXIT		19
#define ZGC		20
#define ZGOTO		21
#define ZGREATERP	22
#define ZIF		23
#define ZINTEGERP	24
#define ZINTEGRAL	25
#define ZLENGTH		26
#define ZLESSP		27
#define ZLIST		28
#define ZLOAD		29
#define ZNOT		30
#define ZNULL		31
#define ZNUMBERP	32
#define ZOR		33
#define ZPOWER		34
#define ZPRINT		35
#define ZPRINTCARS	36
#define ZPRODUCT	37
#define ZPROG		38
#define ZQUOTE		39
#define ZRETURN		40
#define ZSETQ		41
#define ZSUM		42
#define ZSUBST		43
#define ZTENSOR		44
#define ZTRANSPOSE	45

#define iscons(p) ((p)->k == CONS)
#define isnum(p) ((p)->k == NUM)
#define isstr(p) ((p)->k == STR)
#define issym(p) ((p)->k >= SYM)

#define car(p) (iscons(p) ? (p)->u.cons.car : nil)
#define cdr(p) (iscons(p) ? (p)->u.cons.cdr : nil)
#define caar(p) car(car(p))
#define cadr(p) car(cdr(p))
#define cdar(p) cdr(car(p))
#define cddr(p) cdr(cdr(p))
#define caddr(p) car(cdr(cdr(p)))
#define cadar(p) car(cdr(car(p)))
#define cddar(p) cdr(cdr(car(p)))
#define cdddr(p) cdr(cdr(cdr(p)))
#define caaddr(p) car(car(cdr(cdr(p))))
#define cdaddr(p) cdr(car(cdr(cdr(p))))
#define cadaddr(p) car(cdr(car(cdr(cdr(p)))))
#define cddaddr(p) cdr(cdr(car(cdr(cdr(p)))))
#define cdddaddr(p) cdr(cdr(cdr(car(cdr(cdr(p))))))
#define caddaddr(p) car(cdr(cdr(car(cdr(cdr(p))))))

#define arg1(p) car(cdr(p))
#define arg2(p) car(cdr(cdr(p)))
#define arg3(p) car(cdr(cdr(cdr(p))))

#define A u.num.a
#define B u.num.b

typedef struct node {
	char k, tag, gamma, pad;
	union {
		struct { struct node *car; struct node *cdr; } cons;
		struct { struct node *binding; char *printname; } sym;
		struct { int a, b; } num;
		char *str;
	} u;
} U;

U _nil, mem[MEM];

typedef struct {
	U *p;
	int a, b;
} ST;

ST stack[TOS];

U *_arg, *_arg1, *_arg2, *_arg3, *_arg4, *_arg5, *_arg6, *_arglist;
U *_cos;
U *derivative;
U *e;
U *eof;
U *freelist;
U *gamma[17];
U *indexlist;
U *_integral;
U *nil;
U *nothing;
U *_power;
U *_product;
U *quote;
U *read1();
U *read_expr();
U *scann();
U *_sin;
U *sum;
U *symtbl[64];
U *t;
U *_tensor;
U *_integral_list;
U *_meta_a;
U *_meta_b;
U *_meta_f;
U *_meta_n;
U *_meta_x;

FILE *infile;

int tos;
int tensor_op;
int index_i;
int index_j;

char buf[BUF];

int k;
char strbuf[STRBUF + 1];

/* gamma product matrix */

int gp[17][17] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,1,-6,-7,-8,-3,-4,-5,13,14,15,-16,9,10,11,-12,
	0,0,6,-1,-11,10,-2,-15,14,12,-5,4,-9,16,-8,7,-13,
	0,0,7,11,-1,-9,15,-2,-13,5,12,-3,-10,8,16,-6,-14,
	0,0,8,-10,9,-1,-14,13,-2,-4,3,12,-11,-7,6,16,-15,
	0,0,3,2,15,-14,1,11,-10,16,-8,7,13,12,-5,4,9,
	0,0,4,-15,2,13,-11,1,9,8,16,-6,14,5,12,-3,10,
	0,0,5,14,-13,2,10,-9,1,-7,6,16,15,-4,3,12,11,
	0,0,13,12,-5,4,16,-8,7,-1,-11,10,-3,-2,-15,14,-6,
	0,0,14,5,12,-3,8,16,-6,11,-1,-9,-4,15,-2,-13,-7,
	0,0,15,-4,3,12,-7,6,16,-10,9,-1,-5,-14,13,-2,-8,
	0,0,16,-9,-10,-11,-13,-14,-15,-3,-4,-5,1,-6,-7,-8,2,
	0,0,9,-16,8,-7,-12,5,-4,-2,-15,14,6,-1,-11,10,3,
	0,0,10,-8,-16,6,-5,-12,3,15,-2,-13,7,11,-1,-9,4,
	0,0,11,7,-6,-16,4,-3,-12,-14,13,-2,8,-10,9,-1,5,
	0,0,12,13,14,15,9,10,11,-6,-7,-8,-2,-3,-4,-5,-1,
};

U *ccons();
U *num();
U *str();
U *sym();
U *zand();
U *zappend();
U *zatom();
U *zcar();
U *zcdr();
U *zcomponent();
U *zcond();
U *zcons();
U *zcontract();
U *zdefine();
U *zderivative();
U *zdot();
U *zequal();
U *zeval();
U *zexit();
U *zfixp();
U *zgc();
U *zgoto();
U *zgreaterp();
U *zif();
U *zintegral();
U *zlessp();
U *zlist();
U *zload();
U *zlength();
U *znot();
U *znull();
U *znumberp();
U *zor();
U *zpower();
U *zprint();
U *zprintcars();
U *zproduct();
U *zprog();
U *zquote();
U *zreturn();
U *zsetq();
U *zsum();
U *zsubst();
U *ztensor();
U *ztranspose();

U *alloc();
U *append();
U *cons();
U *contract();
U *d();
U *dcos();
U *dd();
U *dfunction();
U *dpower();
U *dproduct();
U *dsin();
U *dsum();
U *eval();
U *eval_user_function();
U *eval_args();
U *expand(int, int);
U *inner_expand();
U *inner_product(U *, U *);
U *product(int, int);
U *ksum();
U *number();
U *pop();
U *power();
void push_factor(U *);
void push_term(U *);
U *subst();
U *symbol();
U *transpose();

void print(U *);
void print1(U *);
void untag(U *);
void add(int *, int *, int, int);
void mult(int *, int *, int, int);
void push(U *);
void pushvars(U *);
void popvars(U *);
void init(void);
void load(char *);
void stop(char *);
char *sdup(char *);

main(argc, argv)
int argc;
char *argv[];
{
	int i;
	U *p;
	infile = stdin;
	init();
	load("startup");
	for (i = 1; i < argc; i++)
		load(argv[i]);
	for (;;) {
		printf("> ");
		p = read1();
		push(p); /* save from gc */
		p = eval(p);
		pop();
		if (p != nothing)
			print(p);
		if (tos) {
			tos = 0;
			printf("stack error\n");
		}
	}
}

U *
eval(p)
U *p;
{
	if (p->k == SYM)
		return p->u.sym.binding; /* symbol's binding */

	if (p->k != CONS)
		return p; /* numbers, strings, etc. */

	switch (p->u.cons.car->k) {
	case SYM:		return eval_user_function(p);
	case ZAND:		return zand(p);
	case ZAPPEND:		return zappend(p);
	case ZATOM:		return zatom(p);
	case ZCAR:		return zcar(p);
	case ZCDR:		return zcdr(p);
	case ZCOMPONENT:	return zcomponent(p);
	case ZCOND:		return zcond(p);
	case ZCONS:		return zcons(p);
	case ZCONTRACT:		return zcontract(p);
	case ZDEFINE:		return zdefine(p);
	case ZDERIVATIVE:	return zderivative(p);
	case ZDOT:		return zdot(p);
	case ZEQUAL:		return zequal(p);
	case ZEVAL:		return zeval(p);
	case ZEXIT:		return zexit(p);
	case ZGC:		return zgc(p);
	case ZGOTO:		return zgoto(p);
	case ZGREATERP:		return zgreaterp(p);
	case ZIF:		return zif(p);
	case ZINTEGERP:		return zfixp(p);
	case ZINTEGRAL:		return zintegral(p);
	case ZLENGTH:           return zlength(p);
	case ZLESSP:		return zlessp(p);
	case ZLIST:		return zlist(p);
	case ZLOAD:		return zload(p);
	case ZNOT:		return znot(p);
	case ZNULL:		return znull(p);
	case ZNUMBERP:		return znumberp(p);
	case ZOR:		return zor(p);
	case ZPOWER:		return zpower(p);
	case ZPRINT:		return zprint(p);
	case ZPRINTCARS:	return zprintcars(p);
	case ZPRODUCT:		return zproduct(p);
	case ZPROG:		return zprog(p);
	case ZQUOTE:		return zquote(p);
	case ZRETURN:		return zreturn(p);
	case ZSETQ:		return zsetq(p);
	case ZSUM:		return zsum(p);
	case ZSUBST:		return zsubst(p);
	case ZTENSOR:		return ztensor(p);
	case ZTRANSPOSE:	return ztranspose(p);
	default:		return p; /* a list with ? head */
	}
}

U *
eval_user_function(p)
U *p;
{
	U *q;

	q = eval_args(cdr(p));

	push(_arg->u.sym.binding);
	push(_arg1->u.sym.binding);
	push(_arg2->u.sym.binding);
	push(_arg3->u.sym.binding);
	push(_arg4->u.sym.binding);
	push(_arg5->u.sym.binding);
	push(_arg6->u.sym.binding);
	push(_arglist->u.sym.binding);

	_arglist->u.sym.binding = q;

	_arg->u.sym.binding = car(q);
	_arg1->u.sym.binding = car(q);

	q = cdr(q);
	_arg2->u.sym.binding = car(q);

	q = cdr(q);
	_arg3->u.sym.binding = car(q);

	q = cdr(q);
	_arg4->u.sym.binding = car(q);

	q = cdr(q);
	_arg5->u.sym.binding = car(q);

	q = cdr(q);
	_arg6->u.sym.binding = car(q);

	p = car(p);
	if (p->u.sym.binding->k == CONS)
		/* normal function */
		p = eval(p->u.sym.binding);
	else
		/* undefined function */
		p = cons(p->u.sym.binding, _arglist->u.sym.binding);

	_arglist->u.sym.binding = pop();
	_arg6->u.sym.binding = pop();
	_arg5->u.sym.binding = pop();
	_arg4->u.sym.binding = pop();
	_arg3->u.sym.binding = pop();
	_arg2->u.sym.binding = pop();
	_arg1->u.sym.binding = pop();
	_arg->u.sym.binding = pop();

	return p;
}

U *
eval_args(p)
U *p;
{
	int i, n = 0;
	while (iscons(p)) {
		push(eval(car(p)));
		n++;
		p = cdr(p);
	}
	p = nil;
	for (i = 0; i < n; i++)
		p = cons(stack[tos - i - 1].p, p);
	tos -= n;
	return p;
}

U *
zand(p)
U *p;
{
	p = cdr(p);
	while (iscons(p)) {
		if (eval(car(p)) == nil)
			return nil;
		p = cdr(p);
	}
	return t;
}

U *
zappend(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	pop();
	p = append(p1, p2);
	return p;
}

U *
append(p1, p2)
U *p1, *p2;
{
	int i, n = 0;
	while (iscons(p1)) {
		push(car(p1));
		n++;
		p1 = cdr(p1);
	}
	for (i = 0; i < n; i++)
		p2 = cons(stack[tos - i - 1].p, p2);
	tos -= n;
	return p2;
}

U *
zatom(p)
U *p;
{
	p = eval(arg1(p));
	if (iscons(p))
		return nil;
	else
		return t;
}

U *
zcar(p)
U *p;
{
	return car(eval(arg1(p)));
}

U *
zcdr(p)
U *p;
{
	return cdr(eval(arg1(p)));
}

U *
zcomponent(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval_args(cddr(p));
	push(p2);
	indexlist = p2;
	tensor_op = 1;
	p = eval(p1);
	tensor_op = 0;
	pop();
	pop();
	return p;
}

U *
zcond(p)
U *p;
{
	p = cdr(p);
	while (iscons(p)) {
		if (eval(caar(p)) != nil)
			return eval(cadar(p));
		p = cdr(p);
	}
	return nil;
}

U *
zcons(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	pop();
	return cons(p1, p2);
}

U *
cons(p1, p2)
U *p1, *p2;
{
	U *p;
	if (freelist == nil) {
		push(p1);
		push(p2);
		p = alloc();
		pop();
		pop();
	} else {
		p = freelist;
		freelist = freelist->u.cons.cdr;
	}
	p->k = CONS;
	p->u.cons.car = p1;
	p->u.cons.cdr = p2;
	return p;
}

U *
zcontract(p)
U *p;
{
	int i, j;
	i = tindex(eval(arg1(p)));
	j = tindex(eval(arg2(p)));
	p = eval(arg3(p));
	if (i == 0 || j == 0 || i == j)
		return p;
	index_i = i;
	index_j = j;
	tensor_op = 2;
	push(p);
	p = eval(p);
	pop();
	tensor_op = 0;
	return p;
}

tindex(p)
U *p;
{
	if (isnum(p) && p->A > 0 && p->B == 1)
		return p->A;
	else
		return 0;
}

/* just like setq except arg2 is not evaluated */

U *
zdefine(p)
U *p;
{
	U *p1, *p2;
	p1 = arg1(p);
	p2 = arg2(p);
	if (issym(p1))
		p1->u.sym.binding = p2;
	return nothing;
}

U *
zderivative(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	push(p2);
	p = d(p1, p2);
	tos -= 2;
	return p;
}

U *
zdot(p)
U *p;
{
	int h = tos;
	p = cdr(p);
	while (iscons(p)) {
		push_factor(eval(car(p)));
		p = cdr(p);
	}
	return product(1, tos - h);
}

U *
zequal(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	pop();
	if (equal(p1, p2))
		return t;
	else
		return nil;
}

equal(p1, p2)
U *p1, *p2;
{
	if (p1 == p2)
		return 1;

	if (p1->k != p2->k)
		return 0;

	switch (p1->k) {
	case CONS:
		while (iscons(p1) && iscons(p2)) {
			if (!equal(car(p1), car(p2)))
				return 0;
			p1 = cdr(p1);
			p2 = cdr(p2);
		}
		return equal(p1, p2);
	case NUM:
		if (p1->A == p2->A && p1->B == p2->B)
			return 1;
		else
			return 0;
	case STR:
		if (strcmp(p1->u.str, p2->u.str) == 0)
			return 1;
		else
			return 0;
	default:
		return 0; /* symbols would have matched p1 == p2 */
	}
}

U *
zeval(p)
U *p;
{
	p = eval(arg1(p));
	push(p);
	p = eval(p);
	pop();
	return p;
}

U *
zexit(p)
U *p;
{
	(void) p;
	exit(1);
	return nil;
}

U *
zfixp(p)
U *p;
{
	p = eval(arg1(p));
	if (p->k == NUM && p->u.num.b == 1)
		return t;
	else
		return nil;
}

U *
zgc()
{
	return number(gc(), 1);
}

gc()
{
	int i, n = 0;

	/* tag everything */

	for (i = 0; i < MEM; i++)
		mem[i].tag = 1;

	/* untag what's used */

	for (i = 0; i < 64; i++)
		untag(symtbl[i]);

	for (i = 0; i < tos; i++)
		untag(stack[i].p);

	/* collect everything that's still tagged */

	freelist = nil;
	for (i = 0; i < MEM; i++)
		if (mem[i].tag) {
			mem[i].u.cons.cdr = freelist;
			freelist = mem + i;
			n++;
		}

	return n;
}

void
untag(p)
U *p;
{
	while (iscons(p) && p->tag) {
		p->tag = 0;
		untag(p->u.cons.car);
		p = p->u.cons.cdr;
	}
	if (p->tag) {
		p->tag = 0;
		if (issym(p))
			untag(p->u.sym.binding);
	}
}

U *
zgoto(p)
U *p;
{
	return p;
}

U *
zgreaterp(p)
U *p;
{

	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	pop();
	if (lessp(p2, p1))
		return t;
	else
		return nil;
}

U *
zif(p)
U *p;
{
	if (eval(arg1(p)) == nil)
		return eval(arg3(p));
	else
		return eval(arg2(p));
}

/* example: p = (integral (power x 2) x) */

U *
zintegral(p)
U *p;
{
	push(eval(arg1(p)));
	push(eval(arg2(p)));
	integral();
	return pop();
}

U *
zlessp(p)
U *p;
{
	U *p1, *p2;
	p1 = eval(arg1(p));
	push(p1);
	p2 = eval(arg2(p));
	pop();
	if (lessp(p1, p2))
		return t;
	else
		return nil;
}

lessp(p1, p2)
U *p1, *p2;
{
	if (p1 == p2)
		return 0;

	if (p1 == nil)
		return 1;

	if (p2 == nil)
		return 0;

	if (isnum(p1) && isnum(p2))
		if (p1->A * p2->B < p2->A * p1->B)
			return 1;
		else
			return 0;

	if (isnum(p1))
		return 1;

	if (isnum(p2))
		return 0;

	if (isstr(p1) && isstr(p2))
		if (strcmp(p1->u.str, p2->u.str) < 0)
			return 1;
		else
			return 0;

	if (isstr(p1))
		return 1;

	if (isstr(p2))
		return 0;

	if (issym(p1) && issym(p2))
		if (strcmp(p1->u.sym.printname, p2->u.sym.printname) < 0)
			return 1;
		else
			return 0;

	if (issym(p1))
		return 1;

	if (issym(p2))
		return 0;

	while (iscons(p1) && iscons(p2) && equal(car(p1), car(p2))) {
		p1 = cdr(p1);
		p2 = cdr(p2);
	}

	if (iscons(p1) && iscons(p2))
		return lessp(car(p1), car(p2));
	else
		return lessp(p1, p2);
}

U *
zlist(p)
U *p;
{
	int i, n = 0;
	p = cdr(p);
	while (iscons(p)) {
		push(eval(car(p)));
		n++;
		p = cdr(p);
	}
	p = nil;
	for (i = 0; i < n; i++)
		p = cons(stack[tos - i - 1].p, p);
	tos -= n;
	return p;
}

U *
zload(p)
U *p;
{
	p = eval(arg1(p));
	if (isstr(p))
		load(p->u.str);
	else
		printf("bad file name, string expected\n");
	return nothing;
}

void
load(s)
char *s;
{
	U *p;
	FILE *f = infile;
	infile = fopen(s, "r");
	if (infile == NULL) {
		printf("cannot open file %s\n", s);
		infile = f;
		return;
	}
	for (;;) {
		p = read1();
		if (p == eof)
			break;
		push(p);
		p = eval(p);
		if (p != nothing)
			print(p);
		pop();
	}
	fclose(infile);
	infile = f;
}

U *
zlength(p)
U *p;
{
	int n = 0;
	p = eval(arg1(p));
	while (iscons(p)) {
		n++;
		p = cdr(p);
	}
	return number(n, 1);
}

U *
znot(p)
U *p;
{
	if (eval(arg1(p)) == nil)
		return t;
	else
		return nil;
}

U *
znull(p)
U *p;
{
	if (eval(arg1(p)) == nil)
		return t;
	else
		return nil;
}

U *
znumberp(p)
U *p;
{
	p = eval(arg1(p));
	if (isnum(p))
		return t;
	else
		return nil;
}

U *
zor(p)
U *p;
{
	p = cdr(p);
	while (iscons(p)) {
		if (eval(car(p)) != nil)
			return t;
		p = cdr(p);
	}
	return nil;
}

U *
zprintcars(p)
U *p;
{
	p = eval(arg1(p));
	while (iscons(p)) {
		print(car(p));
		p = cdr(p);
	}
	return nothing;
}

void
add(pa, pb, a, b)
int *pa, *pb, a, b;
{
	int k;
	a = *pa * b + *pb * a;
	b = *pb * b;
	k = gcd(a, b);
	a /= k;
	b /= k;
	*pa = a;
	*pb = b;
}

U *
zpower(p)
U *p;
{
	push(eval(arg1(p)));
	push(eval(arg2(p)));
	return power();
}

U *
zprint(p)
U *p;
{
	p = cdr(p);
	while (iscons(p)) {
		print1(eval(car(p)));
		p = cdr(p);
	}
	printf("\n");
	return nothing;
}

void
print(p)
U *p;
{
	print1(p);
	printf("\n");
}

void
print1(p)
U *p;
{
	static char buf[100];
	switch (p->k) {
	case CONS:
		printf("(");
		print1(car(p));
		p = cdr(p);
		while (iscons(p)) {
			printf(" ");
			print1(car(p));
			p = cdr(p);
		}
		if (p != nil) {
			printf(" . ");
			print1(p);
		}
		printf(")");
		break;
	case STR:
		printf("%s", p->u.str);
		break;
	case NUM:
		if (p->B == 1)
			printf("%d", p->A);
		else
			printf("%d/%d", p->A, p->B);
		break;
	default:
		printf("%s", p->u.sym.printname);
		break;
	}
}

U *
zproduct(p)
U *p;
{
	int h = tos;
	p = cdr(p);
	while (iscons(p)) {
		push_factor(eval(car(p)));
		p = cdr(p);
	}
	return product(0, tos - h);
}

U *
zprog(p)
U *p;
{
	U *p1, *p2;

	pushvars(cadr(p));

	p1 = cddr(p);

	while (iscons(p1)) {
		p2 = eval(car(p1));
		if (car(p2)->k == ZRETURN) {
			push(p2);
			p2 = eval(cadr(p2));
			pop();
			popvars(cadr(p));
			return p2;
			break;
		}
		if (car(p2)->k == ZGOTO) {
			p1 = cddr(p);
			p2 = cadr(p2);
			while (iscons(p1) && car(p1) != p2)
				p1 = cdr(p1);
		}
		p1 = cdr(p1);
	}

	popvars(cadr(p));

	return nothing;
}

void
pushvars(p)
U *p;
{
	while (iscons(p)) {
		if (issym(car(p)))
			push(car(p)->u.sym.binding);
		p = cdr(p);
	}
}

void
popvars(p)
U *p;
{
	if (iscons(p)) {
		popvars(cdr(p));
		if (issym(car(p)))
			car(p)->u.sym.binding = pop();
	}
}

U *
zquote(p)
U *p;
{
	return arg1(p);
}

U *
zreturn(p)
U *p;
{
	return p;
}

U *
zsetq(p)
U *p;
{
	U *p1, *p2;
	p1 = arg1(p);
	p2 = eval(arg2(p));
	if (issym(p1))
		p1->u.sym.binding = p2;
	return nothing;
}

U *
zsubst(p)
U *p;
{
	U *p1, *p2, *p3;

	p1 = eval(arg1(p));
	push(p1);

	p2 = eval(arg2(p));
	push(p2);

	p3 = eval(arg3(p));
	push(p3);

	p = subst(p1, p2, p3);

	pop();
	pop();
	pop();

	return p;
}

/* substitute p1 for p2 in p3 */

U *
subst(p1, p2, p3)
U *p1, *p2, *p3;
{
	U *p4, *p5;
	if (equal(p2, p3))
		return p1;
	else if (iscons(p3)) {
		p4 = subst(p1, p2, car(p3));
		push(p4);
		p5 = subst(p1, p2, cdr(p3));
		pop();
		return cons(p4, p5);
	} else
		return p3;
}

U *
zsum(p)
U *p;
{
	int h = tos;
	p = cdr(p);
	while (iscons(p)) {
		push_term(eval(car(p)));
		p = cdr(p);
	}
	return ksum(tos - h);
}

U *
ztensor(p)
U *p;
{
	switch (tensor_op) {
	default:
		return cons(_tensor, eval_args(cdr(p)));
	case 1:
		if (equal(cdr(p), indexlist))
			return number(1, 1);
		else
			return number(0, 1);
	case 2:
		return contract(p, index_i, index_j);
	case 3:
		return transpose(p, index_i, index_j);
	}
}

U *
contract(p, i, j)
U *p;
int i, j;
{
	U *p1;
	int k, n;
	ST *s;
	p1 = p;
	n = 0;
	s = stack + tos;
	while (iscons(p1)) {
		push(car(p1));
		n++;
		p1 = cdr(p1);
	}
	if (i >= n || j >= n) {
		tos -= n;
		return p;
	}
	if (!equal(s[i].p, s[j].p)) {
		tos -= n;
		return number(0, 1);
	}
	if (n == 3) {
		tos -= n;
		return number(1, 1);
	}
	p = nil;
	for (k = n - 1; k >= 0; k--)
		if (k != i && k != j)
			p = cons(s[k].p, p);
	tos -= n;
	return p;
}

U *
transpose(p, i, j)
U *p;
int i, j;
{
	U *p1;
	int k, n;
	ST *s;
	p1 = p;
	n = 0;
	s = stack + tos;
	while (iscons(p1)) {
		push(car(p1));
		n++;
		p1 = cdr(p1);
	}
	if (i >= n || j >= n) {
		tos -= n;
		return p;
	}
	p = nil;
	for (k = n - 1; k >= 0; k--)
		if (k == i)
			p = cons(s[j].p, p);
		else if (k == j)
			p = cons(s[i].p, p);
		else
			p = cons(s[k].p, p);
	tos -= n;
	return p;
}

void
mult(pa, pb, a, b)
int *pa, *pb, a, b;
{
	int k;
	a *= *pa;
	b *= *pb;
	k = gcd(a, b);
	a /= k;
	b /= k;
	*pa = a;
	*pb = b;
}

U *
ztranspose(p)
U *p;
{
	int i, j;
	i = tindex(eval(arg1(p)));
	j = tindex(eval(arg2(p)));
	p = eval(arg3(p));
	if (i == 0 || j == 0 || i == j)
		return p;
	index_i = i;
	index_j = j;
	tensor_op = 3;
	push(p);
	p = eval(p);
	pop();
	tensor_op = 0;
	return p;
}

void
init()
{
	int i;

	nil = &_nil;
	nil->k = SYM;
	nil->u.sym.binding = nil;
	nil->u.sym.printname = "nil";

	for (i = 0; i < 64; i++)
		symtbl[i] = nil;

	freelist = nil;
	for (i = 0; i < MEM; i++) {
		/* mem[i].gamma = 0; */
		mem[i].u.cons.cdr = freelist;
		freelist = mem + i;
	}

	symbol("and")		->k = ZAND;
	symbol("append")	->k = ZAPPEND;
	symbol("atom")		->k = ZATOM;
	symbol("car")		->k = ZCAR;
	symbol("cdr")		->k = ZCDR;
	symbol("component")	->k = ZCOMPONENT;
	symbol("cond")		->k = ZCOND;
	symbol("cons")		->k = ZCONS;
	symbol("contract")	->k = ZCONTRACT;
	symbol("define")	->k = ZDEFINE;
	symbol("derivative")	->k = ZDERIVATIVE;
	symbol("equal")		->k = ZEQUAL;
	symbol("eval")		->k = ZEVAL;
	symbol("exit")		->k = ZEXIT;
	symbol("gc")		->k = ZGC;
	symbol("goto")		->k = ZGOTO;
	symbol("greaterp")	->k = ZGREATERP;
	symbol("if")		->k = ZIF;
	symbol("integerp")	->k = ZINTEGERP;
	symbol("integral")	->k = ZINTEGRAL;
	symbol("lessp")		->k = ZLESSP;
	symbol("list")		->k = ZLIST;
	symbol("load")		->k = ZLOAD;
	symbol("run")		->k = ZLOAD;
	symbol("length")	->k = ZLENGTH;
	symbol("not")		->k = ZNOT;
	symbol("null")		->k = ZNULL;
	symbol("numberp")	->k = ZNUMBERP;
	symbol("or")		->k = ZOR;
	symbol("power")		->k = ZPOWER;
	symbol("print")		->k = ZPRINT;
	symbol("printcars")	->k = ZPRINTCARS;
	symbol("product")	->k = ZPRODUCT;
	symbol("prog")		->k = ZPROG;
	symbol("quote")		->k = ZQUOTE;
	symbol("return")	->k = ZRETURN;
	symbol("setq")		->k = ZSETQ;
	symbol("sum")		->k = ZSUM;
	symbol("subst")		->k = ZSUBST;
	symbol("tensor")	->k = ZTENSOR;
	symbol("transpose")	->k = ZTRANSPOSE;
	symbol("dot")		->k = ZDOT;

	gamma[2]  = symbol("gamma0");
	gamma[3]  = symbol("gamma1");
	gamma[4]  = symbol("gamma2");
	gamma[5]  = symbol("gamma3");
	gamma[6]  = symbol("sigma1");  /* (product gamma1 gamma0) */
	gamma[7]  = symbol("sigma2");  /* (product gamma2 gamma0) */
	gamma[8]  = symbol("sigma3");  /* (product gamma3 gamma0) */
	gamma[9]  = symbol("isigma1"); /* (product gamma3 gamma2) */
	gamma[10] = symbol("isigma2"); /* (product gamma1 gamma3) */
	gamma[11] = symbol("isigma3"); /* (product gamma2 gamma1) */
	gamma[12] = symbol("igamma0"); /* (product gamma3 gamma2 gamma1) */
	gamma[13] = symbol("igamma1"); /* (product gamma0 gamma3 gamma2) */
	gamma[14] = symbol("igamma2"); /* (product gamma0 gamma1 gamma3) */
	gamma[15] = symbol("igamma3"); /* (product gamma0 gamma2 gamma1) */
	gamma[16] = symbol("i"); /* (product gamma0 gamma1 gamma2 gamma3) */

	gamma[2]->gamma = 2;
	gamma[3]->gamma = 3;
	gamma[4]->gamma = 4;
	gamma[5]->gamma = 5;
	gamma[6]->gamma = 6;
	gamma[7]->gamma = 7;
	gamma[8]->gamma = 8;
	gamma[9]->gamma = 9;
	gamma[10]->gamma = 10;
	gamma[11]->gamma = 11;
	gamma[12]->gamma = 12;
	gamma[13]->gamma = 13;
	gamma[14]->gamma = 14;
	gamma[15]->gamma = 15;
	gamma[16]->gamma = 16;

	_arg		= symbol("arg");
	_arg1		= symbol("arg1");
	_arg2		= symbol("arg2");
	_arg3		= symbol("arg3");
	_arg4		= symbol("arg4");
	_arg5		= symbol("arg5");
	_arg6		= symbol("arg6");
	_arglist	= symbol("arglist");
	_cos		= symbol("cos");
	derivative	= symbol("derivative");
	e		= symbol("e");
	eof		= symbol("eof");
	_integral	= symbol("integral");
	_integral_list	= symbol("integral-list");
	_meta_a		= symbol("meta-a");
	_meta_b		= symbol("meta-b");
	_meta_f		= symbol("meta-f");
	_meta_n		= symbol("meta-n");
	_meta_x		= symbol("meta-x");
	nothing		= symbol("nothing");
	quote		= symbol("quote");
	_power		= symbol("power");
	_product	= symbol("product");
	_sin		= symbol("sin");
	sum		= symbol("sum");
	t		= symbol("t");
	_tensor		= symbol("tensor");
}

void
stop(char *s)
{
	printf("\nERROR %s\n", s);
	exit(1);
}

/* the temp stack keeps intermediate results from garbage collection */

void
push(p)
U *p;
{
	if (tos == TOS)
		stop("stack overflow");
	stack[tos++].p = p;
}

U *
pop()
{
	if (tos == 0)
		stop("stack underflow");
	return stack[--tos].p;
}

void
push_term(p)
U *p;
{
	if (car(p) == sum) {
		p = cdr(p);
		while (iscons(p)) {
			push(car(p));
			p = cdr(p);
		}
	} else
		push(p);
}

void
push_factor(p)
U *p;
{
	if (car(p) == _product) {
		p = cdr(p);
		while (iscons(p)) {
			push(car(p));
			p = cdr(p);
		}
	} else
		push(p);
}

U *
alloc()
{
	U *p;
	if (freelist == nil) {
		gc();
		if (freelist == nil)
			stop("out of memory");
	}
	p = freelist;
	freelist = freelist->u.cons.cdr;
	return p;
}

U *
number(a, b)
int a, b;
{
	int k;
	U *p;
	k = gcd(a, b);
	a /= k;
	b /= k;
	p = alloc();
	p->k = NUM;
	p->A = a;
	p->B = b;
	return p;
}

gcd(a, b)
int a, b;
{
	int k, sign;
	if (b == 0)
		stop("divide by zero");
	if (a == 0)
		return b;
	if (a < 0)
		a = -a;
	if (b < 0) {
		b = -b;
		sign = -1;
	} else
		sign = 1;
	while (b) {
		k = a % b;
		a = b;
		b = k;
	}
	return sign * a;
}

char *
sdup(char *s)
{
	int j = k;
	while (*s) {
		if (k >= STRBUF)
			stop("string buffer full");
		strbuf[k++] = *s++;
	}
	strbuf[k++] = 0;
	return strbuf + j;
}

U *
symbol(s)
char *s;
{
	U *p;
	int x;
	if (strcmp(s, "nil") == 0)
		return nil;
	x = *s & 63;
	p = symtbl[x];
	while (iscons(p)) {
		if (strcmp(car(p)->u.sym.printname, s) == 0)
			return car(p);
		p = cdr(p);
	}
	p = alloc();
	p->k = SYM;
	p->u.sym.printname = sdup(s);
	p->u.sym.binding = p;
	p = cons(p, symtbl[x]);
	symtbl[x] = p;
	return car(p);
}

U *
read1()
{
	return read_expr(read_token());
}

U *
read_expr(c)
int c;
{
	int i, n;
	U *p;

	while (c == ')' || c == '.') {
		printf("discarding unexpected %c\n", c);
		c = read_token();
	}

	switch (c) {

	case EOF:
		return eof;

	case '(':
		n = 0;
		c = read_token();
		while (c != EOF && c != ')' && c != '.') {
			p = read_expr(c);
			push(p);
			n++;
			c = read_token();
		}
		if (c == '.') {
			c = read_token();
			p = read_expr(c);
			c = read_token();
		} else
			p = nil;
		if (c != ')')
			printf("missing )\n");
		for (i = 0; i < n; i++)
			p = cons(stack[tos - i - 1].p, p);
		tos -= n;
		return p;

	case '\'':
		c = read_token();
		p = read_expr(c);
		p = cons(p, nil);
		p = cons(quote, p);
		return p;

	case 'n':
		return scann(buf);

	case 'q':
		p = alloc();
		p->k = STR;
		p->u.str = sdup(buf);
		return p;

	case 's':
		return symbol(buf);
		
	default:
		stop("bug in read_expr()");
		return nil;
	}
}

read_token()
{
	int c, i;

	/* skip spaces and comments */

	for (;;) {
		c = fgetc(infile);
		if (c == ';')
			while (c != EOF && c != '\n')
				c = fgetc(infile);
		if (c == EOF || !isspace(c))
			break;
	}

	switch (c) {

	case EOF:
		return EOF;

	case '(':
	case ')':
	case '.':
	case '\'':
		return c;

	case '\"':
		for (i = 0; i < BUF; i++) {
			c = fgetc(infile);
			if (c == EOF) {
				printf("unexpected end of file\n");
				break;
			}
			if (c == '\"')
				break;
			buf[i] = c;
		}
		if (i == BUF)
			stop("input buffer overflow");
		buf[i] = 0;
		return 'q'; /* quoted string */

	default:
		buf[0] = c;
		for (i = 1; i < BUF; i++) {
			c = fgetc(infile);
			if (c == EOF)
				break;
			if (isspace(c) || c == '(' || c == ')'
			|| c == ';' || c == '.' || c == '\'') {
				ungetc(c, infile);
				break;
			}
			buf[i] = c;
		}
		if (i == BUF)
			stop("input buffer overflow");
		buf[i] = 0;
		if (*buf == '+' || *buf == '-' || isdigit(*buf))
			return 'n'; /* number */
		else
			return 's'; /* symbol */
	}
}

U *
scann(s)
char *s;
{
	int a, b, sgn;

	a = 0;
	b = 1;
	sgn = 1;

	if (*s == '+')
		s++;
	else if (*s == '-') {
		s++;
		sgn = -1;
	}

	if (!isdigit(*s)) {
		printf("syntax error in number\n");
		return nil;
	}

	while (isdigit(*s)) {
		a = 10 * a + *s - '0';
		s++;
	}

	if (*s == '/') {
		s++;
		b = 0;
		while (isdigit(*s)) {
			b = 10 * b + *s - '0';
			s++;
		}
	}

	if (*s) {
		printf("syntax error in number\n");
		return nil;
	}

	return number(sgn * a, b);
}

cmp(a, b)
ST *a, *b;
{
	if (equal(a->p, b->p))
		return 0;
	else if (lessp(a->p, b->p))
		return -1;
	else
		return 1;
}

U *
ksum(n)
int n;
{
	int a, b, i, j;
	U *p, *q;
	ST *s;

	s = stack + tos - n;

	/* add numbers */

	a = 0;
	b = 1;

	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (isnum(p)) {
			add(&a, &b, p->u.num.a, p->u.num.b);
			s[i].p = nil;
		}
	}

	/* remove numeric coefficients */

	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (car(p) == _product && isnum(cadr(p))) {
			s[i].a = cadr(p)->u.num.a;
			s[i].b = cadr(p)->u.num.b;
			if (iscons(cdddr(p)))
				p = cons(_product, cddr(p));
			else
				p = caddr(p);
			s[i].p = p;
		} else {
			s[i].a = 1;
			s[i].b = 1;
		}
	}

	/* sort terms */

	qsort(s, n, sizeof (ST), cmp);

	/* combine terms */

	for (i = 0; i < n - 1; i++) {
		if (s[i].p == nil)
			continue;
		for (j = i + 1; j < n; j++) {
			if (!equal(s[i].p, s[j].p))
				break;
			add(&s[i].a, &s[i].b, s[j].a, s[j].b);
			s[j].p = nil;
			if (s[i].a == 0) {
				s[i].p = nil;
				break;
			}
		}
	}

	/* put the coefficients back */

	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (p == nil)
			continue;
		if (s[i].a == 1 && s[i].b == 1)
			continue;
		q = number(s[i].a, s[i].b);
		if (car(p) == _product)
			p = cdr(p);
		else {
			push(q); /* save q from gc */
			p = cons(p, nil);
			pop();
		}
		p = cons(q, p);
		p = cons(_product, p);
		s[i].p = p;
	}

	/* add number */

	if (a != 0) {
		for (i = 0; i < n; i++)
			if (s[i].p == nil)
				break;
		if (i == n)
			stop("bug in ksum()");
		s[i].p = number(a, b);
	}

	/* remove nils */

	j = 0;
	for (i = 0; i < n; i++)
		if (s[i].p != nil)
			s[j++].p = s[i].p;

	/* sort terms */

	qsort(s, j, sizeof (ST), cmp);

	/* result */

	switch (j) {
	case 0:
		p = number(0, 1);
		break;
	case 1:
		p = s[0].p;
		break;
	default:
		p = nil;
		for (i = j - 1; i >= 0; i--)
			p = cons(s[i].p, p);
		p = cons(sum, p);
		break;
	}

	tos -= n;

	return p;
}

U *
product(inner, n)
int inner, n;
{
	int a, b, flag, g, i, j, x;
	U *p, *q;
	ST *s;

	s = stack + tos - n;

	/* check for product of sum or sums */

	for (i = 0; i < n; i++)
		if (car(s[i].p) == sum)
			return expand(inner, n);

	/* multiply numbers, tensors and gammas */

	a = 1;
	b = 1;
	g = -1;

	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (isnum(p)) {
			if (p->A == 0) {
				tos -= n;
				return number(0, 1);
			}
			mult(&a, &b, p->A, p->B);
			s[i].p = nil;
		} else if (p->gamma) {
			if (g == -1)
				g = i;
			else {
				x = gp[s[g].p->gamma][p->gamma];
				if (x < 0) {
					x = -x;
					a *= -1;
				}
				if (x == 1) {
					s[g].p = nil;
					s[i].p = nil;
					g = -1;
				} else {
					s[g].p = gamma[x];
					s[i].p = nil;
				}
			}
		}
	}

	/* multiply tensors */

	for (i = 0; i < n; i++) {
	    if (car(s[i].p) == _tensor) {
	        for (j = i + 1; j < n; j++) {
	            if (car(s[j].p) == _tensor) {
	                if (inner) {
	                    s[i].p = inner_product(s[i].p, s[j].p);
	                    s[j].p = nil;
	                    if (isnum(s[i].p)) {
	                        if (s[i].p->A == 0) {
	                            tos -= n;
	                            return number(0, 1);
	                        } else {
	                            s[i].p = nil;
	                            i = j;
	                            break;
                                }
	                    }
	                } else {
	                    s[i].p = append(s[i].p, cdr(s[j].p));
	                    s[j].p = nil;
	                }
	            }
	        }
	    }
	}

	prep_exponents(n);

	/* sort factors */

	qsort(s, n, sizeof (ST), cmp);

	/* combine factors */

	for (i = 0; i < n - 1; i++) {
		if (s[i].p == nil)
			continue;
		for (j = i + 1; j < n; j++) {
			if (!equal(s[i].p, s[j].p))
				break;
			add(&s[i].a, &s[i].b, s[j].a, s[j].b);
			s[j].p = nil;
			if (s[i].a == 0) {
				s[i].p = nil;
				break;
			}
/* quick bug fix while working on general solution... */
/* result is a number with exponent 1 */
			if (isnum(s[i].p) && s[i].a == 1 && s[i].b == 1) {
				mult(&a, &b, s[i].p->A, s[i].p->B);
				s[i].p = nil;
				break;
			}
/* result is a number with exponent -1 */
			if (isnum(s[i].p) && s[i].a == -1 && s[i].b == 1) {
				mult(&a, &b, s[i].p->B, s[i].p->A);
				s[i].p = nil;
				break;
			}
		}
	}

	/* restore exponents */

	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (p == nil)
			continue;
		if (s[i].a == 1 && s[i].b == 1)
			continue;
		q = number(s[i].a, s[i].b);
		if (car(p) == _power) {
			if (caaddr(p) == _product)
				q = cons(q, cdaddr(p));
			else {
				push(q); /* save from gc */
				q = cons(q, cons(caddr(p), nil));
				pop();
			}
			q = cons(_product, q);
			q = cons(q, nil);
			q = cons(cadr(p), q);
		} else
			q = cons(p, cons(q, nil));
		q = cons(_power, q);
		if (cadr(q) == sum && isnum(caddr(q)))
			q = eval(q);
		s[i].p = q;
	}

	/* restore coefficient */

	if (a != 1 || b != 1) {
		for (i = 0; i < n; i++)
			if (s[i].p == nil)
				break;
		if (i == n)
			stop("bug in product()");
		s[i].p = number(a, b);
	}

	/* remove nils */

	j = 0;
	flag = 0;
	for (i = 0; i < n; i++) {
		p = s[i].p;
		if (p != nil) {
			s[j++].p = p;
			if (car(p) == sum)
				flag = 1;
		}
	}

	if (flag) {
		tos = tos - n + j;
		return expand(inner, j);
	}

	/* sort factors */

	qsort(s, j, sizeof (ST), cmp);

	/* result */

	switch (j) {
	case 0:
		p = number(1, 1);
		break;
	case 1:
		p = s[0].p;
		break;
	default:
		p = nil;
		for (i = j - 1; i >= 0; i--)
			p = cons(s[i].p, p);
		p = cons(_product, p);
		break;
	}

	tos -= n;

	return p;
}

/* Here are the cases that must be handled:

input				output

s[i].p				s[i].a	s[i].b	s[i].p
------				------	------	------

a				1	1	a

(power a b)			1	1	(power a b)

(power a 1/2)			1	2	a

(power a (product 1/2 b))	1	2	(power a b)

(power a (product 1/2 b c))	1	2	(power a (product b c))


Example for navigating cars and cdrs:

x = (power a (product 3 b c))

(car x)		->	power
(cdr x)		->	(a (product 3 b c))

(cadr x)	->	a
(cddr x)	->	((product 3 b c))

(caddr x)	->	(product 3 b c)

(caaddr x)	->	product
(cdaddr x)	->	(3 b c)

(cadaddr x)	->	3
(cddaddr x)	->	(b c)

(caddaddr x)	->	b
(cdddaddr x)	->	(c)

*/

prep_exponents(n)
{
	int i;
	U *p, *q;
	for (i = tos - n; i < tos; i++) {
		stack[i].a = 1;
		stack[i].b = 1;
		p = stack[i].p;
		if (car(p) != _power)
			continue;
		if (isnum(caddr(p))) {
			stack[i].p = cadr(p);
			stack[i].a = caddr(p)->A;
			stack[i].b = caddr(p)->B;
		} else if (caaddr(p) == _product && isnum(cadaddr(p))) {
			if (cdddaddr(p) == nil)
				q = caddaddr(p);
			else
				q = cons(_product, cddaddr(p));
			q = cons(q, nil);
			q = cons(cadr(p), q);
			q = cons(_power, q);
			stack[i].p = q;
			stack[i].a = cadaddr(p)->A;
			stack[i].b = cadaddr(p)->B;
		}
	}
}

U *
expand(inner, n)
int inner, n;
{
	U *p;
	int h, h1, h2, i, j, k, m;

	h = tos - n;

	/* stack[h + i].a = stack index of factor i */

	/* stack[h + i].b = number of terms in factor i */

	/* m = number of permutations */

	m = 1;
	for (i = 0; i < n; i++) {
		p = stack[h + i].p;
		if (car(p) == sum) {
			stack[h + i].a = tos;
			j = 0;
			p = cdr(p);
			while (iscons(p)) {
				push(car(p));
				p = cdr(p);
				j++;
			}
		} else {
			stack[h + i].a = h + i;
			j = 1;
		}
		stack[h + i].b = j;
		m = j * m;
	}

	h1 = tos;

	for (i = 0; i < m; i++) {
		k = i;
		h2 = tos;
		for (j = 0; j < n; j++) {
			p = stack[stack[h + j].a + k % stack[h + j].b].p;
			k = k / stack[h + j].b;
			push_factor(p);
		}
		push_term(product(inner, tos - h2));
	}

	p = ksum(tos - h1);

	tos = h;

	return p;
}

U *
power()
{
	int a, b, h, i, n;
	U *p1, *p2;

	p1 = stack[tos - 2].p;
	p2 = stack[tos - 1].p;

	/* (power ? 0) -> 1 */

	if (isnum(p2) && p2->A == 0) {
		tos -= 2;
		return number(1, 1);
	}

	/* (power ? 1) -> ? */

	if (isnum(p2) && p2->A == 1 && p2->B == 1) {
		tos -= 2;
		return p1;
	}

	/* (power 1 ?) -> 1 */

	if (isnum(p1) && p1->A == 1 && p1->B == 1) {
		tos -= 2;
		return p1;
	}

	/* (power a (sum b c)) -> (product (power a b) (power a c)) */

	if (car(p2) == sum) {
		h = tos;
		p2 = cdr(p2);
		while (iscons(p2)) {
			push(p1);
			push(car(p2));
			push_factor(power());
			p2 = cdr(p2);
		}
		p1 = product(0, tos - h);
		tos -= 2;
		return p1;
	}

	/* (power (product a b) c) -> (product (power a c) (power b c)) */

	if (car(p1) == _product) {
		h = tos;
		p1 = cdr(p1);
		while (iscons(p1)) {
			push(car(p1));
			push(p2);
			push_factor(power());
			p1 = cdr(p1);
		}
		p1 = product(0, tos - h);
		tos -= 2;
		return p1;
	}

	/* (power (sum a b) 2) -> (sum (power a 2) (power b 2) (product 2 a b)) */

	if (car(p1) == sum && isnum(p2) && p2->A > 1 && p2->B == 1) {
		push(p1);
		for (i = 1; i < p2->A; i++) {
			push(p1);
			push(expand(0, 2));
		}
		p1 = pop();
		tos -= 2;
		return p1;
	}

	/* (power (power a b) c) -> (power a (product b c)) */

	if (car(p1) == _power) {
		push(cadr(p1));
		h = tos;
		push_factor(caddr(p1));
		push_factor(p2);
		push(product(0, tos - h));
		p1 = power();
		tos -= 2;
		return p1;
	}

	if (isnum(p1) && isnum(p2) && p2->B == 1) {
		a = p1->A;
		b = p1->B;
		if (p2->A > 0)
			n = p2->A;
		else
			n = -p2->A;
		for (i = 1; i < n; i++)
			mult(&a, &b, p1->A, p2->B);
		if (p2->A > 0)
			p1 = number(a, b);
		else
			p1 = number(b, a);
		tos -= 2;
		return p1;
	}

	p1 = cons(_power, cons(p1, cons(p2, nil)));
	tos -= 2;
	return p1;
}

U *
d(p, dx)
U *p, *dx;
{
	// for example, (derivative x x) is 1

	if (equal(p, dx))
		return number(1, 1);

	// for example, (derivative a x) is 0

	if (!iscons(p) || car(p) == _tensor)
		return number(0, 1);

	if (car(p) == sum)
		return dsum(p, dx);

	if (car(p) == _product)
		return dproduct(p, dx);

	if (car(p) == _power)
		return dpower(p, dx);

	if (car(p) == derivative)
		return dd(p, dx);

	if (car(p) == _sin)
		return dsin(p, dx);

	if (car(p) == _cos)
		return dcos(p, dx);

	return dfunction(p, dx);
}

U *
dsum(p, dx)
U *p, *dx;
{
	int h = tos;
	p = cdr(p);
	while (iscons(p)) {
		push_term(d(car(p), dx)); /* result may be a sum */
		p = cdr(p);
	}
	return ksum(tos - h);
}

U *
dproduct(p, dx)
U *p, *dx;
{
	int h1, h2, i, j, n = 0;
	U *p1;
	p1 = cdr(p);
	while (iscons(p1)) {
		n++;
		p1 = cdr(p1);
	}
	h1 = tos;
	for (i = 0; i < n; i++) {
		h2 = tos;
		p1 = cdr(p);
		for (j = 0; j < n; j++) {
			if (i == j)
				push_factor(d(car(p1), dx));
			else
				push(car(p1));
			p1 = cdr(p1);
		}
		push_term(product(0, tos - h2));
	}
	return ksum(tos - h1);
}

U *
dpower(p, dx)
U *p, *dx;
{
	int h = tos;
	U *base, *exponent;
	base = cadr(p);
	exponent = caddr(p);
	if (base == e) {
		/* d exponent * e ^ exponent */
		push_factor(d(exponent, dx)); /* result may be a product */
		push(p);
		return product(0, tos - h);
	} else {
		/* exponent * base ^ (exponent - 1) * d base */
		push_factor(exponent); /* exponent may be a product */
		push(base);
		push(exponent); /* exponent is never a sum */
		push(number(-1, 1));
		push(ksum(2));
		push(power());
		push_factor(d(base, dx));
		return product(0, tos - h);
	}
}

/* derivative of derivative */

U *
dd(p, dx2)
U *p, *dx2;
{
	U *dx1;
	dx1 = caddr(p);
	p = d(cadr(p), dx2);
	push(p);
	if (car(p) == derivative) {
		dx2 = caddr(p);
		p = cadr(p);
		if (lessp(dx1, dx2)) {
			p = cons(derivative, cons(p, cons(dx1, nil)));
			pop();
			push(p);
			p = cons(derivative, cons(p, cons(dx2, nil)));
		} else {
			p = cons(derivative, cons(p, cons(dx2, nil)));
			pop();
			push(p);
			p = cons(derivative, cons(p, cons(dx1, nil)));
		}
	} else
		p = d(p, dx1);
	pop();
	return p;
}

/* derivative of a generic function */

U *
dfunction(p, dx)
U *p, *dx;
{
	U *t;

	t = cdr(p);

	/* no arg list? */

	if (t == nil)
		return cons(derivative, cons(p, cons(dx, nil)));

	/* check arg list */

	while (iscons(t)) {
		if (equal(car(t), dx))
			return cons(derivative, cons(p, cons(dx, nil)));
		t = cdr(t);
	}

	return number(0, 1);
}

U *
dsin(p, dx)
U *p, *dx;
{
	int h = tos;
	push_factor(d(cadr(p), dx));
	push(cons(_cos, cons(cadr(p), nil)));
	return product(0, tos - h);
}

U *
dcos(p, dx)
U *p, *dx;
{
	int h = tos;
	push(number(-1, 1));
	push_factor(d(cadr(p), dx));
	push(cons(_sin, cons(cadr(p), nil)));
	return product(0, tos - h);
}

/*	returns the inner product of two tensor args

	p1		p2		return

	(tensor 1)	(tensor 1)	1

	(tensor 1)	(tensor 2)	0

	(tensor 1 2)	(tensor 1)	0

	(tensor 1 2)	(tensor 2)	(tensor 1)

	(tensor 1 2)	(tensor 2 1)	(tensor 1 1)

*/

U *
inner_product(p1, p2)
U *p1, *p2;
{
	int i, n = 0;
	while (iscons(p1)) {
		push(car(p1));
		n++;
		p1 = cdr(p1);
	}
	if (equal(stack[tos - 1].p, cadr(p2))) {
		p2 = cddr(p2);
		for (i = 1; i < n; i++)
			p2 = cons(stack[tos - i - 1].p, p2);
		if (cdr(p2) == nil)
			p2 = number(1, 1);
	} else
		p2 = number(0, 1);
	tos -= n;
	return p2;
}

#define f (_meta_f->u.sym.binding)
#define x (_meta_x->u.sym.binding)
#define a (_meta_a->u.sym.binding)
#define n (_meta_n->u.sym.binding)

integral()
{
	U *tmp1, *tmp2;

	tmp2 = pop();
	tmp1 = pop();

	push(f);
	push(x);
	push(a);
	push(n);

	f = tmp1;
	x = tmp2;

	if (does_not_depend_on_x(f))
		integral_of_constant();
	else if (car(f) == sum)
		integral_of_sum();
	else if (car(f) == _product)
		integral_of_product();
	else
		integral_of_nub();

	tmp1 = pop();

	n = pop();
	a = pop();
	x = pop();
	f = pop();

	push(tmp1);
}

integral_of_constant()
{
	push(f);
	push(x);
	push(product(0, 2));
}

integral_of_sum()
{
	int h;
	U *p;

	h = tos;

	p = cdr(f);
	while (iscons(p)) {
		push(car(p));
		push(x);
		integral();
		push_term(pop());
		p = cdr(p);
	}

	p = ksum(tos - h);

	push(p);
}

integral_of_product()
{
	int h1, h2;
	U *p;

	h1 = tos;

	p = cdr(f);
	while (iscons(p)) {
		if (does_not_depend_on_x(car(p)))
			push(car(p));
		p = cdr(p);
	}

	if (tos == h1) {
		integral_of_nub();
		return;
	}

	h2 = tos;

	p = cdr(f);
	while (iscons(p)) {
		if (depends_on_x(car(p)))
			push(car(p));
		p = cdr(p);
	}

	push(product(0, tos - h2));

	push(x);

	integral();

	push_factor(pop());

	push(product(0, tos - h1));
}

integral_of_nub()
{
	int h;
	U *p;

	h = tos;

	push(number(1, 1)); /* so meta variables can try the value 1 */

	deconstruct(f);

	p = _integral_list->u.sym.binding;

	while (iscons(p)) {
		if (match(caar(p), cddar(p), h)) {
			p = eval(cadar(p));
			tos = h;
			push(p);
			return;
		}
		p = cdr(p);
	}

	p = cons(_integral, cons(f, cons(x, nil)));

	tos = h;

	push(p);
}

depends_on_x(p)
U *p;
{
	if (findx(p) || findf(p))
		return 1;
	else
		return 0;
}

does_not_depend_on_x(p)
U *p;
{
	if (findx(p) || findf(p))
		return 0;
	else
		return 1;
}

/* yes if dx somewhere in p */

findx(p)
U *p;
{
	if (p == x)
		return 1;
	while (iscons(p)) {
		if (findx(car(p)))
			return 1;
		p = cdr(p);
	}
	return 0;
}

/* yes if anonymous function somewhere in p, i.e. p = (sum (f) 1) */

findf(p)
U *p;
{
	if (iscons(p) && car(p)->k == SYM && cdar(p) == nil)
		return 1;
	while (iscons(p)) {
		if (findf(car(p)))
			return 1;
		p = cdr(p);
	}
	return 0;
}

/* push constant expressions */

deconstruct(p)
U *p;
{
	int h;
	U *q;

	if (does_not_depend_on_x(p)) {
		push(p);
		return;
	}

	if (car(p) != _product) {
		p = cdr(p);
		while (iscons(p)) {
			deconstruct(car(p));
			p = cdr(p);
		}
		return;
	}

	q = cdr(p);
	while (iscons(q)) {
		if (depends_on_x(car(q)))
			deconstruct(car(q));
		q = cdr(q);
	}

	h = tos;

	p = cdr(p);
	while (iscons(p)) {
		if (does_not_depend_on_x(car(p)))
			push(car(p));
		p = cdr(p);
	}

	if (tos - h)
		push(product(0, tos - h));
}

/*	p	form

	l	list of constraints

	h	Where constant expressions start on the stack

	Juggle the constant expressions to try and match f and p.
*/

match(p, l, h)
U *p, *l;
int h;
{
	int j1, j2;
	U *q;

	for (j1 = h; j1 < tos; j1++)
	for (j2 = h; j2 < tos; j2++)
	{
		a = stack[j1].p;
		n = stack[j2].p;

		/* check constraints */

		q = l;
		while (iscons(q)) {
			if (eval(car(q)) == nil)
				break;
			q = cdr(q);
		}

		if (iscons(q))
			continue; /* constraint failed */

		if (equal(f, eval(p)))
			return 1;
	}

	return 0;
}
