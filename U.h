// Symbolic expressions are built by connecting U structs.
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
