#include "stdafx.h"



#include "defs.h"

extern int powermode;

static int check_args(void);
static void charpoly_n(void);
static void charpoly_s(void);

#define A p1
#define B p2
#define LAMBDA p3
#define C p4
#define POLY p5

void
charpoly(void)
{
	int i, n;
	U **a;
	save();
	LAMBDA = pop();
	A = pop();
	if (check_args() == 0) {
		push_symbol(CHARPOLY);
		push(A);
		push(LAMBDA);
		list(3);
		restore();
		return;
	} else {
		n = A->u.tensor->nelem;
		a = A->u.tensor->elem;
		for (i = 0; i < n; i++)
			if (!isnum(a[i]))
				break;
		if (i == n)
			charpoly_n();
		else
			charpoly_s();
	}

	// multiply constant terms by LAMBDA ^ 0

	push_symbol(POWER);
	push(LAMBDA);
	push(_zero);
	list(3);
	multiply();

	restore();
}

static int
check_args(void)
{
	if (!istensor(A))
		return 0;
	else if (A->u.tensor->ndim != 2)
		return 0;
	else if (A->u.tensor->dim[0] != A->u.tensor->dim[1])
		return 0;
	else if (!issym(LAMBDA))
		return 0;
	else
		return 1;
}

static void
charpoly_n(void)
{
	int i, j, n;

	n = A->u.tensor->dim[0];

	// copy A

	push(A);
	eval();
	B = pop();

	// C = trace(A)

	push(A);
	trace();
	C = pop();

	// POLY = LAMBDA ^ n - C * LAMBDA ^ (n - 1)

	push(LAMBDA);
	push_integer(n);
	power();
	push(C);
	push(LAMBDA);
	push_integer(n - 1);
	power();
	multiply();
	subtract();
	POLY = pop();

	for (i = 1; i < n; i++) {

		// B = B - C * I

		for (j = 0; j < n; j++) {
			push(B->u.tensor->elem[j * n + j]);
			push(C);
			subtract();
			B->u.tensor->elem[j * n + j] = pop();
		}

		// B = A . B 

		push(A);
		push(B);
		inner();
		B = pop();

		// C = trace(B) / (i + 1)

		push(B);
		trace();
		push_integer(i + 1);
		divide();
		C = pop();

		// POLY = POLY - C * LAMBDA ^ (n - i - 1)

		push(POLY);
		push(C);
		push(LAMBDA);
		push_integer(n - i - 1);
		power();
		multiply();
		subtract();
		POLY = pop();
	}

	push(POLY);

	if (n % 2)
		negate();
}

static void
charpoly_s(void)
{
	int n;
	n = A->u.tensor->dim[0];
	push(A);
	push(LAMBDA);
	push_identity_matrix(n);
	multiply();
	subtract();
	det();
}

static char *s[] = {

	"A=((1,2),(3,4))",
	"",

	"f(x)=eval(charpoly(A))",
	"",

	"f",
	"-5*x-2*x^0+x^2",

	"f(A)",
	"0",

	"A=((a,b),(c,d))",
	"",

	"f(lambda)=eval(charpoly(A,lambda))",
	"",

	"f",
	"a*d*lambda^0-a*lambda-b*c*lambda^0-d*lambda+lambda^2",

	"f(A)",
	"0",

	"A=quote(A)",
	"",

	"f()=eval(quote(f))",
	"",

	"charpoly(A)",
	"charpoly(A,x)",
};

void
test_charpoly(void)
{
	test(__FILE__, s, sizeof s / sizeof (char *));
}
