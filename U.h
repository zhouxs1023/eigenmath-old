// Expressions are built by connecting U structs.
//
// For example, (a b + c) is built like this:
//
//           _______      _______                                _______
// U *p --->|CONS   |--->|CONS   |----------------------------->|CONS   |
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
//
// It's no coincidence that it looks like LISP.

typedef struct u {
	unsigned char k, tag;
	unsigned short pad;
	union {
		struct {
			struct u *car; // pointing down
			struct u *cdr; // pointing right
		} cons;
		struct {
			struct u *binding;	// symbol's value binding
			struct u *binding2;	// symbol's function binding
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
	IMAGINARYUNIT,
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
	NIL,
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

	AUTOEXPAND,
	E,
	EXPOMODE,
	FORMAT,
	IM,
	LAST,
	OVERLAY,
	PENCOLOR,
	PI,

	TRANGE,
	XRANGE,
	YRANGE,

	SYMBOL_BIG_D,
	SYMBOL_BIG_Q,

	SYMBOL_D,
	SYMBOL_R,
	SYMBOL_T,
	SYMBOL_X,
	SYMBOL_Y,
	SYMBOL_Z,
};

