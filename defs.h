// Symbolic expressions are built by connecting U structs.
//
// For example, (a b + c) is built like this:
//
//           _______      _______                                _______
//          |CONS   |--->|CONS   |----------------------------->|CONS   |
//          |       |    |       |                              |       |
//          |_______|    |_______|                              |_______|
//              |            |                                      |
//           ___v___      ___v___      _______      _______      ___v___
//          |ADD    |    |CONS   |--->|CONS   |--->|CONS   |    |SYM c  |
//          |       |    |       |    |       |    |       |    |       |
//          |_______|    |_______|    |_______|    |_______|    |_______|
//                           |            |            |
//                        ___v___      ___v___      ___v___
//                       |MUL    |    |SYM a  |    |SYM b  |
//                       |       |    |       |    |       |
//                       |_______|    |_______|    |_______|

typedef struct U {
	unsigned char k, tag;
	unsigned short pad;
	union {
		struct {
			struct U *car;		// pointing down
			struct U *cdr;		// pointing right
		} cons;
		struct {
			struct U *binding;	// symbol's value binding
			struct U *binding2;	// symbol's function binding
		} sym;
		char *str;
		struct tensor *tensor;
		struct {
			unsigned int *a, *b;	// rational number a over b
		} q;
		double d;
	} u;
} U;

// Here's what goes in the k field.

enum {
	CONS,
	NUM,
	DOUBLE,
	STR,
	TENSOR,
	SYM,

	ABS,
	ADD,
	ADJ,
	ARCCOS,
	ARCCOSH,
	ARCSIN,
	ARCSINH,
	ARCTAN,
	ARCTANH,
	ATOMIZE,
	BINDING2,
	BINOMIAL,
	BREAK,
	CEILING,
	CHARPOLY,
	CHECK,
	CLEAR,
	CLS,
	COEFF,
	CONDENSE,
	CONJ,
	CONTRACT,
	COS,
	COSH,
	DEGREE,
	DENOMINATOR,
	DERIVATIVE,
	DET,
	DIM,
	DISPLAY,
	DIVISORS,
	DO,
	DOT,
	DRAW,
	DSOLVE,
	EIGEN,
	EIGENVAL,
	EIGENVEC,
	EVAL,
	EXP,
	EXPAND,
	EXPCOS,
	EXPSIN,
	FACTOR,
	FACTORIAL,
	FACTORPOLY,
	FILTER,
	FLOATF,
	FLOOR,
	FOR,
	GCD,
	HERMITE,
	HILBERT,
	INDEX,
	INNER,
	INTEGRAL,
	INV,
	INVG,
	ISINTEGER,
	ISPRIME,
	LAGUERRE,
	LCM,
	LEGENDRE,
	LOG,
	MOD,
	MULTIPLY,
	NUMERATOR,
	OPERATOR,
	OUTER,
	POWER,
	PRIME,
	PRINT,
	PRODUCT,
	PROG,
	QUOTE,
	RANK,
	RATIONALIZE,
	RETURN,
	ROOTS,
	SAMPLE,
	SETQ,
	SIMPLIFY,
	SIN,
	SINH,
	SQRT,
	STOP,
	SUBST,
	SUM,
	TAB,
	TAN,
	TANH,
	TAYLOR,
	TEST,
	TESTEQ,
	TESTGE,
	TESTGT,
	TESTLE,
	TESTLT,
	TRACE,
	TRANSPOSE,
	UNIT,
	WEDGE,
	ZERO,
};

#define TOS 1000000
#define BUF 10000

#define MAX_PROGRAM_SIZE 100001
#define MAXPRIMETAB 10000

#define _USE_MATH_DEFINES // for MS C++

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <setjmp.h>
#include <math.h>
#include <errno.h>

// for fast symbol access (see symbol.cpp)

enum {
	AUTOEXPAND = 200,
	E,
	EXPOMODE,
	IM,
	LAST,
	PI,
	SYMBOL_A,
	SYMBOL_B,
	SYMBOL_D,
	SYMBOL_R,
	SYMBOL_T,
	SYMBOL_X,
	SYMBOL_Y,
	SYMBOL_Z,
	TTY,
};

#define MAXDIM 24

typedef struct tensor {
	int ndim;
	int dim[MAXDIM];
	int nelem;
	U *elem[1];
} T;

struct display {
	int h, w, n;
	struct {
		int c, x, y;
	} a[1]; // a for array
};

extern U **frame;

#define iscons(p) ((p)->k == CONS)
#define isnum(p) ((p)->k == NUM || (p)->k == DOUBLE)
#define isstr(p) ((p)->k == STR)
#define istensor(p) ((p)->k == TENSOR)
#define isscalar(p) ((p)->k != TENSOR)
#define issym(p) ((p)->k >= SYM)
#define issymbol(p) ((p)->k == SYM)
#define iskeyword(p) ((p)->k > SYM)

#define car(p) (iscons(p) ? (p)->u.cons.car : nil)
#define cdr(p) (iscons(p) ? (p)->u.cons.cdr : nil)
#define caar(p) car(car(p))
#define cadr(p) car(cdr(p))
#define cdar(p) cdr(car(p))
#define cddr(p) cdr(cdr(p))
#define caadr(p) car(car(cdr(p)))
#define caddr(p) car(cdr(cdr(p)))
#define cadar(p) car(cdr(car(p)))
#define cdadr(p) cdr(car(cdr(p)))
#define cddar(p) cdr(cdr(car(p)))
#define cdddr(p) cdr(cdr(cdr(p)))
#define caaddr(p) car(car(cdr(cdr(p))))
#define cadadr(p) car(cdr(car(cdr(p))))
#define caddar(p) car(cdr(cdr(car(p))))
#define cdaddr(p) cdr(car(cdr(cdr(p))))
#define cadddr(p) car(cdr(cdr(cdr(p))))
#define cddddr(p) cdr(cdr(cdr(cdr(p))))
#define caddddr(p) car(cdr(cdr(cdr(cdr(p)))))
#define cadaddr(p) car(cdr(car(cdr(cdr(p)))))
#define cddaddr(p) cdr(cdr(car(cdr(cdr(p)))))
#define cdddaddr(p) cdr(cdr(cdr(car(cdr(cdr(p))))))
#define caddaddr(p) car(cdr(cdr(car(cdr(cdr(p))))))

//#define arg1(p) car(cdr(p))
//#define arg2(p) car(cdr(cdr(p)))
//#define arg3(p) car(cdr(cdr(cdr(p))))

#define isadd(p) (car(p) == symbol(ADD))
#define ispower(p) (car(p) == symbol(POWER))
#define isfactorial(p) (car(p) == symbol(FACTORIAL))

//#define numerator mp_numerator
void mp_numerator(void);

//#define denominator mp_denominator
void mp_denominator(void);

#define isfraction mp_isfraction
int mp_isfraction(U *);

#define scan_integer bignum_scan_integer
void bignum_scan_integer(char *);

#define scan_float bignum_scan_float
void bignum_scan_float(char *);

void bignum_power_number(int);

void bignum_truncate(void);

void mp_set_bit(unsigned int *, unsigned int);
void mp_clr_bit(unsigned int *, unsigned int);

extern U *pop();

extern int tos;
extern int expanding;
extern int expomode;
extern int conjugating;
extern int fmt_x;
extern int fmt_index;
extern int fmt_level;
extern int verbosing;
extern int floating;
extern char program_buf[];
extern int primetab[MAXPRIMETAB];
extern int esc_flag;
extern U *nil;
extern U *stack[];

extern U *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8;
extern U *formal_arg[6];
extern U *tmp;
extern U *last;
extern U *nil;
extern U *zero, *one, *imaginaryunit;
extern U *table_of_integrals, *meta_a, *meta_b, *meta_x;

extern U *alloc();
extern U *alloc_tensor();
extern U *inner_product(U *, U *);
extern U *number();
extern U *pop();

extern int cmp(U **, U**);

extern U *copy_number(U *);
double convert_rational_to_double(U *);

extern void run(char *);
extern void save(void);
extern void restore(void);
extern void d_tensor_tensor(void);
extern void d_tensor_scalar(void);
extern void d_scalar_tensor(void);
extern void d_scalar_scalar(void);
extern int equal(U *, U *);
extern int length(U *);
extern void push(U *);
extern void addk(int);
extern void multiply_all(int);
extern void divide(void);
extern void multiply(void);
extern void logarithm(void);
extern void add(void);
extern int lessp(U *, U *);
extern void list(int);
extern void sine(void);
extern void cosine(void);
extern void expcos(void);
extern void expsin(void);
extern void negate(void);
extern void negate_expand(void);
extern void negate_noexpand(void);
extern void init(void);
extern void defn(void);
extern void test(char *, char **, int);
extern void test_all(void);
extern void read_file(char *);
extern int scan(char *);
extern void print(U *);
extern void qfree(U *);
extern void init(void);
extern U *number(int);
extern void push_double(double);
extern void push_rational(int, int);
extern void out_of_memory(void);
extern void maxdim_error(void);
extern void cons(void);
extern int isfraction(U *);
extern void subtract(void);
extern int ispoly(U *, U *);
extern int ispoly_expr(U *, U *);
extern void subst(void);
extern int coeff(void);
extern void eval(void);
extern void eval_noexpand(void);
extern void evalat(void);
extern int find(U *, U *);
extern int ispoly_term(U *, U *);
extern int ispoly_factor(U *, U *);
extern int isposint(U *);
extern int compare_tensors(U *, U *);
extern void inv(void);
extern void inverse(void);
extern void term_plus_term(void);
extern void print_number(U *);
extern void tensor_plus_tensor(void);
extern void factor_times_factor(int *);
extern void push_number(int);
extern void push_integer(int);
extern int pop_integer(void);
extern void tensor_times_scalar(void);
extern void scalar_times_tensor(void);
extern void spow(void);
extern int is_numeric_factor(U *);
extern void multiply_rational_powers(int);
extern void qnumer(void);
extern void qdenom(void);
extern int qfit(U *);
extern void qinv(void);
extern void qtrunc(void);
extern void qpow(void);
extern void conj_tensor(void);
extern void build_tensor(int);
extern U *alloc_tensor(int);
extern void print_unsigned_term(U *);
extern void print_unsigned_factor(U *);
extern void print_factor(U *);
extern void print_tensor(U *);
extern void print_tensor_inner(U *, int, int *);
extern void print_str(char *);
extern void print_char(int);
extern void print_num(U *, int);
extern void print_double(U *, int flag);
extern void print_function_definition(U *);
extern void print_arg_list(U *);
extern void print_lisp(U *);
extern void print1(U *);
extern void eval(void);
extern void iterate(int);
extern void adj(void);
extern void arccos(void);
extern void arcsin(void);
extern void arctan(void);
extern void contract(void);
extern void scot(void);
extern void scsc(void);
extern void cross(void);
extern void curl(void);
extern void derivative(void);
extern int integer(void);
extern void det(void);
extern void inner(void);
extern void inv(void);
extern void outer(void);
extern void ssec(void);
extern void tangent(void);
extern void taylor(void);
extern void trace(void);
extern void transpose(void);
extern void conjugate(void);
extern void tensor_plus_tensor(void);
extern void determinant(int);
extern int is_square_matrix(U *);
extern void cofactor(U *, int, int, int);
extern void component(int);
extern void push_cars(U *);
extern void mp_test(void);
extern void push_int(int);
extern void integral(void);
extern int cmp_expr(U *, U *);
extern void dsolve(void);
extern void sexp(void);
extern void swap(void);
extern void distill(void);
extern void distill_sum(void);
extern void distill_product(void);
extern void scanlisp(char *);
extern int distilly();
extern void simplifyfactorials(void);
extern void peek(void);
extern void peek2(void);
extern void expand(void);
extern void expand1(void);
extern void mul2(void);
extern void factorial(void);
extern void hilbert(void);
extern void swap(void);
extern void guess(void);
extern int isnegativeterm(U *);
char *get_name(U *);
int new_name(char *);
void init_alloc(void);
void detg(void);
void invg(void);
void eval_tensor(void);
void test_a(void);
void test_b(void);
void test_c(void);
void test_d(void);
void test_h(void);
void test_i(void);
void test_k(void);
void testp(void);
void testp_prime(void);
void testq(void);
void testq_prime(void);
void gc(void);
void push_frame(int);
void pop_frame(int);
void binomial(void);
void charpoly(void);
void init_stack(void);
void push_zero_matrix(int, int);
void push_identity_matrix(int);
void push_symbol(int);
U *get_symbol(char *);
char *get_printname(U *);
U *symbol(int);
void power(void);
void exponential(void);
void invert_matrix(void);
void power_tensor(void);
void init_integers(void);
void stop(char *);
void top_eval(void);
void hermite(void);
void prime(void);
int iszero(U *);
int isplusone(U *);
int isminusone(U *);
int isinteger(U *);
int isintegerfactor(U *);
int isnonnegativeinteger(U *);
int iseveninteger(U *);
void ssinh(void);
void scosh(void);
void gcd(void);
void laguerre(void);
void lcm(void);
void legendre(void);
void test_binomial(void);
void test_hermite(void);
void test_laguerre(void);
void test_legendre(void);
void square(void);
void rationalize(void);
int isnegativenumber(U *);
int isnegative(U *);
void test_gcd(void);
void factor(void);
void test_factor(void);
void fmt_char(int);
void fmt_fraction(int, int, int);
void fmt_test(U *);
void scan2(char *);
void test_is_prime(void);
void test_quickfactor(void);
void factor_number(void);
void factor_poly(void);
void degree(void);
void divisors(void);
void divisors_onstack(void);
void factor_small_number(void);
int sign(int);
void factorpoly(void);
void multiply_all_noexpand(int);
void vectorize(int);
void quickfactor(void);
void alloc_qnum(void);
void restore_frame(U **);
void multiply_noexpand(void);
void absval(void);
void variables(void);
void coeff_cooked(void);
void display(U *);
void print_out(void);
void prog(void);
void prog_return(void);
void loop(void);
void loop_break(void);
void __print(void);
double pop_double(void);
void bignum_float(void);
void new_string(char *);
void printline(U *);
void tokenize(char *);
int scan_next(void);
void wedge2(void);
void wedge3(void);
int save_symbols(int);
void restore_symbols(int);
void printstr(char *);
void printchar(int);
void printchar_nowrap(int);
void condense(void);
extern void ysinh(void);
extern void ycosh(void);
extern char logbuf[];
extern void logout(char *);
extern void errout(void);
extern void clear(void);
extern void clear_symbols(void);
extern void clear_term(void);
extern void numerator(void);
extern void denominator(void);
extern void reciprocate(void);

#define MSIGN(p) (((int *) (p))[-2])
#define MLENGTH(p) (((int *) (p))[-1])

#define MZERO(p) (MLENGTH(p) == 1 && (p)[0] == 0)
#define MEQUAL(p, n) (MLENGTH(p) == 1 && (long long) MSIGN(p) * (p)[0] == (n))

extern unsigned int *mnew(int);
extern void mfree(unsigned int *);
extern unsigned int *mint(int);
extern unsigned int *mcopy(unsigned int *);
extern int mcmp(unsigned int *, unsigned int *);
extern int mcmpint(unsigned int *, int);
extern unsigned int *madd(unsigned int *, unsigned int *);
extern unsigned int *msub(unsigned int *, unsigned int *);
extern unsigned int *mmul(unsigned int *, unsigned int *);
extern unsigned int *mdiv(unsigned int *, unsigned int *);
extern unsigned int *mmod(unsigned int *, unsigned int *);
void mdivrem(unsigned int **, unsigned int **, unsigned int *, unsigned int *);
extern unsigned int *mpow(unsigned int *, unsigned int);
extern unsigned int *mroot(unsigned int *, unsigned int);
extern unsigned int *msqrt(unsigned int *);
extern unsigned int *mgcd(unsigned int *, unsigned int *);
extern unsigned int *mmodpow(unsigned int *, unsigned int *, unsigned int *);
extern void mshiftright(unsigned int *);
extern unsigned int *mscan(char *);
extern char *mstr(unsigned int *);
extern int mprime(unsigned int *);
extern int mtotal;
extern int issymbolic(U *);

extern void qadd(void);
extern void qsub(void);
extern void qmul(void);
extern void qdiv(void);
extern void qpow(void);

// these functions handle mixed-mode arithmetic (rationals and doubles)

extern int compare_numbers(U *, U *);
extern void add_numbers(void);
extern void subtract_numbers(void);
extern void multiply_numbers(void);
extern void divide_numbers(void);
extern void power_numbers(void);
extern void negate_number(void);
extern void invert_number(void);
extern void gcd_numbers(void);
