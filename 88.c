/* An experimental CAS that works by rewriting character strings. */

int debug=0;

#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <math.h>

#include <string.h>

char *input, *output;

int flag;

int nalloc, valloc;

char *expr[1000];

int nexpr;

main(argc, argv)

int argc;

char *argv[];

{

	int i, n;

	input = malloc(1000);

	output = malloc(30000);

	if (input == NULL || output == NULL)

		exit(1);

	output[29999] = -1;

	if (argc > 1)

		for (i = 1; i < argc; i++)

			process(argv[i]);

	else

		while (fgets(input, 1000, stdin))

			process(input);

}

process(s)

char *s;

{

	yyparse(s);

	replace(output);

	eval(output);

	format(output);

	printf("\n");

	if (nalloc - valloc)

		printf("error: nalloc-valloc=%d\n", nalloc - valloc);

	if (output[29999] != -1) {

		printf("buffer overflow\n");

		exit(1);

	}

}

/*---------------------------------------------------------------------------*/

/* this is the parser */

int yn;

char *ys, *yy;

yyparse(s)

char *s;

{

	if (debug)

		printf("debug: yyparse(\"%s\")\n", s);

	yn = 0;

	yy = output;

	ys = s;

	yscan(0);

	yexpr();

	if (*ys)

		yerr(9);	/* syntax error */

	youtc('\0');

}

yexpr()

{

	y32();

}

/* expr = expr */

/* expr := expr */

/* expr =. */

y32()

{

	int i, k, n;

	n = yn;

	y24();

	if (ys[0] == '=' && ys[1] != '.') {

		k = 0;

		while (ys[0] == '=' && ys[1] != '.') {

			k++;

			yscan(1);

			youts(n, "{Set ");

			n = yn;

			y24();

		}

		for (i = 0; i < k; i++)

			youts(yn, "} ");

	} else if (ys[0] == ':' && ys[1] == '=') {

		yscan(2);

		youts(n, "{SetDelayed ");

		y24();

		youts(yn, "} ");

	} else if (ys[0] == '=' && ys[1] == '.') {

		yscan(2);

		youts(n, "{UnSet ");

		youts(yn, "} ");

	}

}

/* expr || expr || expr */

y24()

{

	int n = yn;

	y23();

	if (ys[0] == '|' && ys[1] == '|') {

		youts(n, "{Or ");

		do {

			yscan(2);

			y23();

		} while (ys[0] == '|' && ys[1] == '|');

		youts(yn, "} ");

	}

}

/* expr && expr && expr */

y23()

{

	int n = yn;

	y22();

	if (ys[0] == '&' && ys[1] == '&') {

		youts(n, "{And ");

		do {

			yscan(2);

			y22();

		} while (ys[0] == '&' && ys[1] == '&');

		youts(yn, "} ");

	}

}

/* ! expr */

y22()

{

	if (*ys == '!') {

		youts(yn, "{Not ");

		yscan(1);

		y22();

		youts(yn, "} ");

	} else

		y21();

}

/* evaluate left to right */

/* expr == expr */

/* expr != expr */

/* expr > expr */

/* expr >= expr */

/* expr < expr */

/* expr <= expr */

y21()

{

	int n = yn;

	y20();

	for (;;)

		if (ys[0] == '=' && ys[1] == '=') {

			youts(n, "{Equal ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '=');

			youts(yn, "} ");

		} else if (ys[0] == '!' && ys[1] == '=') {

			youts(n, "{Unequal ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '!');

			youts(yn, "} ");

		} else if (ys[0] == '>' && ys[1] != '=' && ys[1] != '>') {

			youts(n, "{Greater ");

			do {

				yscan(1);

				y20();

			} while (ys[0] == '>' && ys[1] != '=' && ys[1] != '>');

			youts(yn, "} ");

		} else if (ys[0] == '>' && ys[1] == '=') {

			youts(n, "{GreaterEqual ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '>' && ys[1] == '=');

			youts(yn, "} ");

		} else if (ys[0] == '<' && ys[1] != '=') {

			youts(n, "{Less ");

			do {

				yscan(1);

				y20();

			} while (ys[0] == '<' && ys[1] != '=');

			youts(yn, "} ");

		} else if (ys[0] == '<' && ys[1] == '=') {

			youts(n, "{LessEqual ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '<' && ys[1] == '=');

			youts(yn, "} ");

		} else

			break;

}

/* expr + expr + expr */

y20()

{

	int n = yn;

	yy20();

	if (ys[0] == '+' && ys[1] != '+' && ys[1] != '=') {

		youts(n, "{Plus ");

		do {

			yscan(1);

			yy20();

		} while (ys[0] == '+' && ys[1] != '+' && ys[1] != '=');

		youts(yn, "} ");

	}

}

/* expr - expr */

/* ( expr - expr ) - expr */

yy20()

{

	int n = yn;

	y19();

	while (ys[0] == '-' && ys[1] != '-' && ys[1] != '=') {

		yscan(1);

		youts(n, "{Plus ");

		youts(yn, "{Times -1 ");

		y19();

		youts(yn, "} } ");

	}

}

/* expr expr expr */

/* expr * expr * expr */

y19()

{

	int n = yn;

	y18();

	if (yisfactor()) {

		youts(n, "{Times ");

		do {

			if (*ys == '*')

				yscan(1);

			y18();

		} while (yisfactor());

		youts(yn, "} ");

	}

}

yisfactor()

{

	if (

	*ys == '(' ||

	*ys == '{' ||

	*ys == '$' ||

	ys[0] == '*' && ys[1] != '*' ||

	ys[0] == '.' && isdigit(ys[1]) ||

	ys[0] == '+' && ys[1] == '+' ||

	ys[0] == '-' && ys[1] == '-' ||

	isalnum(*ys)

	)

		return 1;

	else

		return 0;

}

/* expr / expr */

/* ( expr / expr ) / expr */

y18()

{

	int n = yn;

	y17();

	while (ys[0] == '/' && ys[1] != '/') {

		yscan(1);

		youts(n, "{Times ");

		youts(yn, "{Power ");

		y17();

		youts(yn, "-1 } } ");

	}

}

/* + expr */

/* - expr */

y17()

{

	if (ys[0] == '+' && ys[1] != '+') {

		yscan(1);

		y16();

	} else if (ys[0] == '-' && ys[1] != '-') {

		yscan(1);

		youts(yn, "{Times -1 ");

		y16();

		youts(yn, "} ");

	} else

		y16();

}

/* expr ^ expr */

/* expr ^ ( expr ^ expr ) */

y16()

{

	int n = yn;

	y15();

	if (ys[0] == '^' && ys[1] != '^') {

		youts(n, "{Power ");

		yscan(1);

		y16();

		youts(yn, "} ");

	}

}

/* expr . expr . expr */

y15()

{

	int n = yn;

	y14();

	if (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1])) {

		youts(n, "{Dot ");

		do {

			yscan(1);

			y14();

		} while (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1]));

		youts(yn, "} ");

	}

}

yisdot()

{

	if (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1]))

		return 1;

	else

		return 0;

}

/* expr ** expr ** expr */

y14()

{

	int n = yn;

	y13();

	if (ys[0] == '*' && ys[1] == '*') {

		youts(n, "{NonCommutativeMultiply ");

		do {

			yscan(2);

			y13();

		} while (ys[0] == '*' && ys[1] == '*');

		youts(yn, "} ");

	}

}

/* expr ' */

y13()

{

	y12();

}

/* expr ! */

y12()

{

	int n = yn;

	y11();

	if (ys[0] == '!' && ys[1] != '!') {

		yscan(1);

		youts(n, "{Factorial ");

		youts(yn, "} ");

	} else if (ys[0] == '!' && ys[1] == '!') {

		yscan(2);

		youts(n, "{Factorial2 ");

		youts(yn, "} ");

	}

}

/* expr /@ expr */

/* expr //@ expr */

/* expr @@ expr */

y11()

{

	int n = yn;

	y10();

	if (ys[0] == '/' && ys[1] == '@') {

		yscan(2);

		youts(n, "{Map ");

		y11();

		youts(yn, "} ");

	} else if (ys[0] == '/' && ys[1] == '/' && ys[2] == '@') {

		yscan(3);

		youts(n, "{MapAll ");

		y11();

		youts(yn, "} ");

	} else if (ys[0] == '@' && ys[1] == '@') {

		yscan(2);

		youts(n, "{Apply ");

		y11();

		youts(yn, "} ");

	}

}

/* expr ~ expr ~ expr */

y10()

{

	y9();

}

/* expr @ expr */

/* expr @ ( expr @ expr ) */

y9()

{

	y8();

	if (ys[0] == '@' && ys[1] != '@') {

		yscan(1);

		youtc('[');

		y9();

		youts(yn, "} ");

	}

}

/* expr ++ */

/* expr -- */

/* ++ expr */

/* -- expr */

y8()

{

	int n = yn;

	if (ys[0] == '+' && ys[1] == '+') {

		yscan(2);

		youts(yn, "{PreIncrement ");

		y7();

		youts(yn, "} ");

	} else if (ys[0] == '-' && ys[1] == '-') {

		yscan(2);

		youts(yn, "{PreDecrement ");

		y7();

		youts(yn, "} ");

	} else {

		y7();

		if (ys[0] == '+' && ys[1] == '+') {

			yscan(2);

			youts(n, "{Increment ");

			youts(yn, "} ");

		} else if (ys[0] == '-' && ys[1] == '-') {

			yscan(2);

			youts(n, "{Decrement ");

			youts(yn, "} ");

		}

	}

}

/* expr [ expr , expr , ... ] */

/* expr [[ expr , expr, ... ]] */

y7()

{

	int n = yn;

	y6();

	for (;;)

		if (ys[0] == '[' && ys[1] != '[') {

			youts(n, "{");

			yscan(1);

			yexpr();

			while (*ys == ',') {

				yscan(1);

				yexpr();

			}

			if (ys[0] == ']')

				yscan(1);

			else

				yerr(3);	/* ] expected */

			youts(yn, "} ");

		} else if (ys[0] == '[' && ys[1] == '[') {

			youts(n, "{Part ");

			yscan(2);

			yexpr();

			while (*ys == ',') {

				yscan(1);

				yexpr();

			}

			if (ys[0] == ']' && ys[1] == ']')

				yscan(2);

			else

				yerr(4);	/* ]] expected */

			youts(yn, "} ");

		} else

			break;

}

/* expr ? expr */

y6()

{
	int n = yn;

	y5();

	if (*ys == '?' ) {

		yscan(1);

		youts(n, "{PatternTest ");

		y5();

		youts(yn, "} ");

	}

}

y5()

{

	y4();

}

y4()

{

	y3();

}

y3()

{

	y2();

}

/* expr :: string */

y2()

{

	y1();

}

/* symbol */

/* digits . digits */

/* -digits . digits */

/* ( expr ) */

/* { expr , expr , ... } */

y1()

{

	int n = yn;

	if (*ys == '$' || isalpha(*ys)) {

		do

			youtc(*ys++);

		while (*ys == '$' || isalnum(*ys));

		youtc(' ');

		if (*ys == '_') {

			ys++;

			youts(n, "{Pattern ");

			youts(yn, "{Blank ");

			if (*ys == '(' || isalpha(*ys))

				y1();

			else if (*ys == '.') {

				ys++;

				youts(n, "{Optional ");

				youts(yn, "} ");

			}

			youts(yn, "} } ");

		}

		yscan(0);

	} else if (isdigit(*ys) || *ys == '-' && isdigit(ys[1])) {

		youtc(*ys++);

		while (isdigit(*ys))

			youtc(*ys++);

		if (*ys == '.') {

			youtc(*ys++);

			if (isdigit(*ys))

				while (isdigit(*ys))

					youtc(*ys++);
			else

				youtc('0');

		}

		youtc(' ');

		yscan(0);

	} else if (ys[0] == '.' && isdigit(ys[1])) {

		ys++;

		youts(yn, "0.");

		while (isdigit(*ys))

			youtc(*ys++);

		youtc(' ');

		yscan(0);

	} else if (*ys == '(') {

		yscan(1);

		yexpr();

		if (*ys == ')')

			yscan(1);

		else

			yerr(5);	/* ) expected */

	} else if (*ys == '{') {

		yscan(1);

		youts(yn, "{List ");

		yexpr();

		while (*ys == ',') {

			yscan(1);

			yexpr();

		}

		if (*ys == '}')

			yscan(1);

		else

			yerr(6);	/* } expected */

		youts(yn, "} ");

	} else {

		printf("ys=\"%s\"\n", ys);

		yerr(2);		/* syntax error */

	}

}

/* skip n chars then skip spaces */

yscan(n)

int n;

{

	ys += n;

	while (isspace(*ys))

		ys++;

}

/* insert string s at index n */

youts(n, s)

int n;

char *s;

{

	int i, len;

	len = strlen(s);

	for (i = yn - 1; i >= n; i--)

		yy[i + len] = yy[i];

	for (i = 0; i < len; i++)

		yy[n + i] = s[i];

	yn += len;

}

youtc(c)

int c;

{

	yy[yn++] = c;

}

/*	1	out of memory */

/*	2	syntax error */

/*	3	] expected */

/*	4	]] expected */

/*	5	) expected */

/*	6	} expected */

yerr(n)

int n;

{

	printf("error %d\n", n);

	exit(1);

}

/*---------------------------------------------------------------------------*/

char *arg1(s)

char *s;

{

	return s + 1 + arglen(s + 1);

}


char *arg2(s)

char *s;

{

	s = arg1(s);

	return s + arglen(s);

}

char *copyarg(s)

char *s;

{

	int i, n;

	char *t;

	n = arglen(s);

	nalloc += n;

	t = malloc(n + 1);

	for (i = 0; i < n; i++)

		t[i] = s[i];

	t[i] = 0;

	return t;

}

freearg(s)

char *s;

{

	nalloc -= strlen(s);

	free(s);

}

delarg(s)

char *s;

{

	char *t;

	flag = 1;

	t = s + arglen(s);

	while (*t)

		*s++ = *t++;

	*s = 0;

}

insarg(s, t)

char *s, *t;

{

	int i, len, n;

	if (t == NULL || *t == 0)

		return;

	flag = 1;

	n = arglen(t);

/*	n = strlen(t); */

	len = strlen(s);

	for (i = len; i >= 0; i--)

		s[i + n] = s[i];

	for (i = 0; i < n; i++)

		s[i] = t[i];

}

isnumeric(s)

char *s;

{

	if (*s == '+' || *s == '-')

		s++;

	if (*s >= '0' && *s <= '9')

		return 1;

	else

		return 0;

}

/*

s points to the argument

s = "(times,a,b)" returns 11

s = "times,a,b)" returns 5

*/

arglen(s)

char *s;

{

	int k, n;

	if (*s == '{') {

		n = 3;

		s++;

		while (*s != '}') {

			k = arglen(s);

			n += k;

			s += k;

		}

	} else {

		n = 0;

		while (*s && *s != ' ') {

			n++;

			s++;

		}

		if (*s == ' ')

			n++;

	}

	return n;

}

/*

s points to the expression

s = "(times,a,b)" returns 3

s = "times,a,b)" returns 1

*/

argcount(s)

char *s;

{

	int n = 1;

	if (*s == '{') {

		s++;

		for (;;) {

			s += arglen(s);

			if (*s == '}')

				break;

			n++;

		}

	}

	return n;

}

mulnum(s)

char *s;

{

	int i, k, n;

	char *t, u[20];

	double d, x;

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		t = s + 7;

		k = 0;

		for (i = 0; i < n; i++) {

			if (isnumeric(t))

				k++;

			t += arglen(t);

		}

		if (k > 1) {

			x = 1.0;

			t = s + 7;

			for (i = 0; i < n; i++)

				if (isnumeric(t)) {

					sscanf(t, "%lg", &d);

					x *= d;

					delarg(t);

				} else

					t += arglen(t);

			if (x == 0.0)

				x = 0.0;	/* eliminate -0 */

			sprintf(u, "%g ", x);

			if (argcount(s) == 1) {

				delarg(s);

				insarg(s, u);

			} else

				insarg(s + 7, u);

		}

	}

}

pownum(s)

char *s;

{

	char u[20];

	double x, y;

	if (strncmp(s, "{Power ", 7) == 0 && isnumeric(s + 7) && isnumeric(s + 7 + arglen(s + 7))) {

		sscanf(s + 7, "%lg%lg", &x, &y);

		sprintf(u, "%g ", pow(x, y));

		delarg(s);

		insarg(s, u);

	}

}

/* compare expressions */

isequal(s, t)

char *s, *t;

{

	int i, n;

	/* exact compare */

	n = arglen(s);

	if (n == arglen(t)) {

		for (i = 0; i < n; i++)

			if (s[i] == t[i])

				;

			else

				return 0;

		return 1;

	} else

		return 0;

}

outofmemory()

{

	printf("out of memory\n");

	exit(1);

}

sort(s)

char *s;

{

	int i, l, lp, lq, n, nn;

	char *p, *q, *copyarg();

	if (strncmp(s, "{Plus ", 6) == 0 || strncmp(s, "{Times ", 7) == 0) {

		nn = argcount(s) - 2;

		while (nn) {

			n = nn;

			nn = 0;

			p = s + 1 + arglen(s + 1);

			lp = arglen(p);			/* length of p */

			for (i = 0; i < n; i++) {

				q = p + lp;

				lq = arglen(q);		/* length of q */

				if (lp < lq)

					l = lp;

				else

					l = lq;

				if (strncmp(p, q, l) > 0) {

					nn = i;

					q = copyarg(q);

					delarg(p + lp);

					insarg(p, q);

					freearg(q);

					p += lq;

				} else {

					p = q;

					lp = lq;

				}

			}

		}

	}

}

/*

(Power (Times x y ) 2 ) = (Times (Power x 2 ) (Power y 2 ) )

*/

expandnumericalpowers(s)

char *s;

{

	char *a, *b, *k, *copyarg();

	if (strncmp(s, "{Power {Times ", 14) == 0 && isnumeric(s + 7 + arglen(s + 7))) {

		k = copyarg(s + 7 + arglen(s + 7));

		a = copyarg(s + 14);

		if (argcount(s + 7) == 3)

			b = copyarg(s + 14 + arglen(s + 14));

		else {

			delarg(s + 14);

			b = copyarg(s + 7);

		}

		delarg(s);

		insarg(s, "{Times {Power } {Power } } ");

		insarg(s + 23, k);

		insarg(s + 23, b);

		insarg(s + 14, k);

		insarg(s + 14, a);

		freearg(a);

		freearg(b);

		freearg(k);

	}

}

/*

(Times 3 (Plus x y ) ) = (Plus (Times 3 x ) (Times 3 y ) )

*/

expandnumericalproducts(s)

char *s;

{

	char *a, *b, *k, *t, *copyarg();

	if (strncmp(s, "{Times ", 7) == 0 && isnumeric(s + 7) && strncmp(s + 7 + arglen(s + 7), "{Plus ", 6) == 0 && argcount(s) == 3) {

		k = copyarg(s + 7);

		t = s + 7 + arglen(s + 7);

		a = copyarg(t + 6);

		if (argcount(t) == 3)

			b = copyarg(t + 6 + arglen(t + 6));

		else {

			delarg(t + 6);

			b = copyarg(t);

		}

		delarg(s);

		insarg(s, "{Plus {Times } {Times } } ");

		insarg(s + 22, b);

		insarg(s + 22, k);

		insarg(s + 13, a);

		insarg(s + 13, k);

		freearg(a);

		freearg(b);

		freearg(k);

	}

}

/*---------------------------------------------------------------------------*/

replace(s)

char *s;

{

	int i, n;

	char *t, *copyarg();

	if (debug)

		printf("debug: replace(\"%s\")\n", s);

	if (strncmp(s, "{Set ", 5) == 0) {

		replace(s + 5 + arglen(s + 5));

		return;

	}

	for (;;) {

		if (*s == '{') {

			n = argcount(s);

			t = s + 1;

			for (i = 0; i < n; i++) {

				replace(t);

				t += arglen(t);

			}

		}

		for (i = nexpr - 1; i > -1; i--)

			if (isequal(s, expr[i] + 5))

				break;

		if (i > -1) {

			delarg(s);

			insarg(s, expr[i] + 5 + arglen(expr[i] + 5));

		} else

			break;

	}

}

eval(s)

char *s;

{

	int i, n;

	char *t;

	if (strncmp(s, "{Set ", 5) == 0) {

		eval(s + 5 + arglen(s + 5));

		Set(s);

		return;

	}

	while (*s == '{') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			eval(t);

			t += arglen(t);

		}

		flag = 0;

		rule1(s);	/* {Plus a {Plus b c } } = {Plus a b c } */

		rule2(s);	/* {Times a {Times b c } } = {Times a b c } */

		rule3(s);	/* {Plus 2 3 } = 5 */

		pownum(s);

		mulnum(s);

		rule4(s);

		rule5(s);

		rule6(s);

		rule7(s);

		rule8(s);

		rule9(s);

		rule10(s);

		rule11(s);	/* {Plus x x } = {Times 2 x } */

		rule12(s);	/* {Times x x } = {Power x 2 } */

		rule13(s);	/* {Power {Power x 2 } 3 } = {Power x 6 } */

		rule14(s);	/* add tensors */

		expandnumericalpowers(s);

		expandnumericalproducts(s);

		sort(s);

		Contract(s);

		D(s);

		Dot(s);

		Gradient(s);

		Part(s);

		TensorProduct(s);

		Outer(s);

		Transpose(s);

		Tensor(s);

		WedgeProduct(s);

		/* reevaluate entire expression tree if any changes */

		if (flag == 0)

			break;

	}

}

/* {Plus a {Plus b c } } = {Plus a b c } */

rule1(s)

char *s;

{

	int i, n;

	char *a, *b, *copyarg();

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		s += 6;

		for (i = 0; i < n; i++) {

			if (strncmp(s, "{Plus ", 6) == 0)

				break;

			s += arglen(s);

		}

		if (i < n) {

			n = argcount(s) - 1;

			a = copyarg(s);

			delarg(s);

			b = a + 6;

			for (i = 0; i < n; i++) {

				insarg(s, b);

				s += arglen(s);

				b += arglen(b);

			}

			freearg(a);

		}

	}

}

/* {Times a {Times b c } } = {Times a b c } */

rule2(s)

char *s;

{

	int i, n;

	char *a, *b, *copyarg();

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		s += 7;

		for (i = 0; i < n; i++) {

			if (strncmp(s, "{Times ", 7) == 0)

				break;

			s += arglen(s);

		}

		if (i < n) {

			n = argcount(s) - 1;

			a = copyarg(s);

			delarg(s);

			b = a + 7;

			for (i = 0; i < n; i++) {

				insarg(s, b);

				s += arglen(s);

				b += arglen(b);

			}

			freearg(a);

		}

	}

}

/* {Plus 2 3 } = 5 */

rule3(s)

char *s;

{

	int i, k, n;

	char *t, u[20];

	double d, x;

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		t = s + 6;

		k = 0;

		for (i = 0; i < n; i++) {

			if (isnumeric(t))

				k++;

			t += arglen(t);

		}

		if (k > 1) {

			x = 0.0;

			t = s + 6;

			for (i = 0; i < n; i++)

				if (isnumeric(t)) {

					sscanf(t, "%lg", &d);

					x += d;

					delarg(t);

				} else

					t += arglen(t);

			if (x == 0.0)

				x = 0.0;	/* eliminate -0 */

			sprintf(u, "%g ", x);

			if (argcount(s) == 1) {

				delarg(s);

				insarg(s, u);

			} else

				insarg(s + 6, u);

		}

	}

}

/*

{Plus x 0 y } = {Plus x y }

*/

rule4(s)

char *s;

{

	int i, n;

	char *t, *copyarg();

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		t = s + 6;

		for (i = 0; i < n; i++)

			if (strncmp(t, "0 ", 2) == 0)

				delarg(t);

			else

				t += arglen(t);

		switch (argcount(s)) {

		case 1:

			delarg(s);

			insarg(s, "0 ");

			break;

		case 2:

			t = copyarg(s + 6);

			delarg(s);

			insarg(s, t);

			freearg(t);

			break;

		default:

			break;

		}

	}

}

/*

{Times x 0 y } = 0

*/

rule5(s)

char *s;

{

	int i, n;

	char *t;

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		t = s + 7;

		for (i = 0; i < n; i++)

			if (strncmp(t, "0 ", 2) == 0) {

				delarg(s);

				insarg(s, "0 ");

				break;

			} else

				t += arglen(t);

	}

}

/*

{Times x 1 y } = {Times x y }

*/

rule6(s)

char *s;

{

	int i, n;

	char *t, *copyarg();

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		t = s + 7;

		for (i = 0; i < n; i++)

			if (strncmp(t, "1 ", 2) == 0)

				delarg(t);

			else

				t += arglen(t);

		switch (argcount(s)) {

		case 1:

			delarg(s);

			insarg(s, "1 ");

			break;

		case 2:

			t = copyarg(s + 7);

			delarg(s);

			insarg(s, t);

			freearg(t);

			break;

		default:

			break;

		}

	}

}

/*

{Power e 0 } = 1

*/

rule7(s)

char *s;

{

	if (strncmp(s, "{Power ", 7) == 0 && strncmp(s + 7 + arglen(s + 7), "0 ", 2) == 0) {

		delarg(s);

		insarg(s, "1 ");

	}

}

/*

{Power e 1 } = e

*/

rule8(s)

char *s;

{

	char *t, *copyarg();

	if (strncmp(s, "{Power ", 7) == 0 && strncmp(s + 7 + arglen(s + 7), "1 ", 2) == 0) {

		t = copyarg(s + 7);

		delarg(s);

		insarg(s, t);

		freearg(t);

	}

}

/*

{Power 1 e } = 1

*/

rule9(s)

char *s;

{

	if (strncmp(s, "{Power 1 ", 9) == 0) {

		delarg(s);

		insarg(s, "1 ");

	}

}

/*

{Times a {List b c } } = {List {Times a b } {Times a c } }

*/

rule10(s)

char *s;

{

	int i, n;

	char *t, *u, *copyarg();

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		t = s + 7;

		for (i = 0; i < n; i++)

			if (strncmp(t, "{List ", 6) == 0)

				break;

			else

				t += arglen(t);

		if (i < n) {

			u = copyarg(t);

			delarg(t);

			if (n == 2)

				t = copyarg(s + 7);

			else

				t = copyarg(s);

			delarg(s);

			insarg(s, u);

			freearg(u);

			n = argcount(s) - 1;

			s += 6;

			for (i = 0; i < n; i++) {

				u = copyarg(s);

				delarg(s);

				insarg(s, "{Times } ");

				insarg(s + 7, u);

				insarg(s + 7, t);

				freearg(u);

				s += arglen(s);

			}

			freearg(t);

		}

	}

}

/*

{Plus x x } = {Times 2 x }

{Plus x {Times 2 x } } = {Times 3 x }

{Plus {Times 2 x } {Times 3 x } } = {Times 5 x }

*/

rule11(s)

char *s;

{

	int i, j, n;

	char *a, *b;

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		if (n < 2)

			return;

		a = s + 6;

		for (i = 0; i < n - 1; i++) {

			b = a + arglen(a);

			for (j = i + 1; j < n; j++) {

				if (rule11compare(a, b)) {

					rule11merge(s, a, b);

					return;

				}

				b += arglen(b);

			}

			a = a + arglen(a);

		}

	}

}

rule11compare(a, b)

char *a, *b;

{

	int i, na = 1, nb = 1;

	if (strncmp(a, "{Times ", 7) == 0) {

		na = argcount(a) - 1;

		a += 7;

		if (isnumeric(a)) {

			a += arglen(a);

			na--;

		}

	}

	if (strncmp(b, "{Times ", 7) == 0) {

		nb = argcount(b) - 1;

		b += 7;

		if (isnumeric(b)) {

			b += arglen(b);

			nb--;

		}

	}

	if (na != nb)

		return 0;

	for (i = 0; i < na; i++) {

		if (!isequal(a, b))

			return 0;

		a += arglen(a);

		b += arglen(b);

	}

	return 1;

}

rule11merge(s, a, b)

char *s, *a, *b;

{

	char *m, *n, *copyarg();

	if (strncmp(b, "{Times ", 7) == 0 && isnumeric(b + 7))

		n = copyarg(b + 7);

	else

		n = copyarg("1 ");

	delarg(b);

	if (strncmp(a, "{Times ", 7) == 0 && isnumeric(a + 7)) {

		m = copyarg(a + 7);

		delarg(a + 7);

		insarg(a + 7, "1 ");

	} else

		m = copyarg("1 ");

	if (argcount(s) - 1 == 1) {

		a = copyarg(a);

		delarg(s);

	} else {

		s = a;

		a = copyarg(a);

		delarg(s);

	}

	insarg(s, "{Times {Plus } } ");

	insarg(s + 15, a);

	insarg(s + 13, n);

	insarg(s + 13, m);

	freearg(a);

	freearg(m);

	freearg(n);

}

/*

{Times x x } = {Power x 2 }

{Times x {Power x 2 } } = {Power x 3 }

{Times {Power x 2 } {Power x 3 } } = {Power x 5 }

*/

rule12(s)

char *s;

{

	int i, j, n;

	char *a, *b;

	if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		if (n < 2)

			return;

		a = s + 7;

		for (i = 0; i < n - 1; i++) {

			b = a + arglen(a);

			for (j = i + 1; j < n; j++) {

				if (rule12compare(a, b)) {

					rule12merge(s, a, b);

					return;

				}

				b += arglen(b);

			}

			a = a + arglen(a);

		}

	}

}

rule12compare(a, b)

char *a, *b;

{

	int i;

	if (strncmp(a, "{Power ", 7) == 0 && isnumeric(a + 7 + arglen(a + 7)))

		a += 7;

	if (strncmp(b, "{Power ", 7) == 0 && isnumeric(b + 7 + arglen(b + 7)))

		b += 7;

	return isequal(a, b);

}

rule12merge(s, a, b)

char *s, *a, *b;

{

	char *m, *n, *t, *copyarg();

	if (strncmp(a, "{Power ", 7) == 0 && isnumeric(a + 7 + arglen(a + 7))) {

		t = copyarg(a + 7);

		m = copyarg(a + 7 + arglen(a + 7));

	} else {

		t = copyarg(a);

		m = copyarg("1 ");

	}

	if (strncmp(b, "{Power ", 7) == 0 && isnumeric(b + 7 + arglen(b + 7)))

		n = copyarg(b + 7 + arglen(b + 7));

	else

		n = copyarg("1 ");


	if (arglen(s) - 1 == 2)

		delarg(s);

	else {

		s = a;

		delarg(b);

		delarg(a);

	}

	insarg(s, "{Power {Plus } } ");

	insarg(s + 13, n);

	insarg(s + 13, m);

	insarg(s + 7, t);

	freearg(t);

	freearg(m);

	freearg(n);

}

/*

{Power {Power x y } z } = {Power x {Times y z } }

*/

rule13(s)

char *s;

{

	char *x, *y, *z;

	if (strncmp(s, "{Power {Power ", 14) == 0) {

		x = copyarg(s + 14);

		y = copyarg(s + 14 + arglen(s + 14));

		z = copyarg(s + 7 + arglen(s + 7));

		delarg(s);

		insarg(s, "{Power {Times } } ");

		insarg(s + 14, z);

		insarg(s + 14, y);

		insarg(s + 7, x);

		freearg(x);

		freearg(y);

		freearg(z);

	}

}

/* add tensors */

rule14(s)

char *s;

{

	int i, j, m, n;

	char *a, *b, *bb;

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		if (n < 2)

			return;

		a = s + 6;

		for (i = 0; i < n - 1; i++) {

			if (strncmp(a, "{List ", 6) == 0) {

				m = argcount(a) - 1;

				b = a + arglen(a);

				for (j = i + 1; j < n; j++) {

					if (strncmp(b, "{List ", 6) == 0 && argcount(b) - 1 == m)

						break;

					b += arglen(b);

				}

				if (j < n)

					break;

			}

			a += arglen(a);

		}

		if (i < n - 1) {

			bb = copyarg(b);

			delarg(b);

			b = bb + 6;

			if (n == 2) {

				a = copyarg(a);

				delarg(s);

				insarg(s, a);

				freearg(a);

			} else

				s = a;

			s += 6;

			for (i = 0; i < m; i++) {

				a = copyarg(s);

				delarg(s);

				insarg(s, "{Plus } ");

				insarg(s + 6, b);

				insarg(s + 6, a);

				freearg(a);

				b += arglen(b);

				s += arglen(s);

			}

			freearg(bb);

		}

	}

}

D(s)

char *s;

{

	char *t;

	if (strncmp(s, "{D ", 3) == 0) {

		t = s + 3;

		if (isnumeric(t))

			dnumerical(s);

		else if (isalpha(*t))

			dvariable(s);

		else if (strncmp(t, "{Plus ", 6) == 0)

			dsum(s);

		else if (strncmp(t, "{Times ", 7) == 0)

			dproduct(s);

		else if (strncmp(t, "{Power ", 7) == 0)

			dpower(s);

		else if (strncmp(t, "{Sin ", 5) == 0)

			dsin(s);

		else if (strncmp(t, "{Cos ", 5) == 0)

			dcos(s);

		else if (strncmp(t, "{Arctan ", 8) == 0)

			darctan(s);

	}

}

dnumerical(s)

char *s;

{

	delarg(s);

	insarg(s, "0 ");

}

dvariable(s)

char *s;

{

	if (isequal(s + 3, s + 3 + arglen(s + 3))) {

		delarg(s);

		insarg(s, "1 ");

	} else {

		delarg(s);

		insarg(s, "0 ");

	}

}

dsum(s)

char *s;

{

	char *a, *b, *d, *copyarg();

	d = copyarg(s + 3 + arglen(s + 3));

	if (argcount(s + 3) == 3) {

		/* (D (Plus x y ) d ) */

		a = copyarg(s + 9);

		b = copyarg(s + 9 + arglen(a));

	} else {

		/* (D (Plus x y z ) d ) */

		a = copyarg(s + 9);

		delarg(s + 9);

		b = copyarg(s + 3);

	}

	delarg(s);

	insarg(s, "{Plus {D } {D } } ");

	insarg(s + 14, d);

	insarg(s + 14, b);

	insarg(s + 9, d);

	insarg(s + 9, a);

	freearg(a);

	freearg(b);

	freearg(d);

}

dproduct(s)

char *s;

{

	char *a, *b, *d, *copyarg();

	d = copyarg(s + 3 + arglen(s + 3));

	if (argcount(s + 3) == 3) {

		/* (D (Times x y ) d ) */

		a = copyarg(s + 10);

		b = copyarg(s + 10 + arglen(a));

	} else {

		/* (D (Times x y z ) d ) */

		a = copyarg(s + 10);

		delarg(s + 10);

		b = copyarg(s + 3);

	}

	delarg(s);

	insarg(s, "{Plus {Times {D } } {Times {D } } } ");

	insarg(s + 30, d);

	insarg(s + 30, b);

	insarg(s + 27, a);

	insarg(s + 18, b);

	insarg(s + 16, d);

	insarg(s + 16, a);

	freearg(a);

	freearg(b);

	freearg(d);

}

dpower(s)

char *s;

{

	char *u, *v, *d, *copyarg();

	/* example {D {Power y 3 } x } */

	d = copyarg(s + 3 + arglen(s + 3));		/* x */

	u = copyarg(s + 10);				/* y */

	v = copyarg(s + 10 + arglen(s + 10));		/* 3 */

	delarg(s);

	insarg(s, "{Plus {Times {Power {Plus -1 } } {D } } {Times {Log } {Power } {D } } } ");

	insarg(s + 66, d);

	insarg(s + 66, v);

	insarg(s + 61, v);

	insarg(s + 61, u);

	insarg(s + 52, u);

	insarg(s + 36, d);

	insarg(s + 36, u);

	insarg(s + 29, v);

	insarg(s + 20, u);

	insarg(s + 13, v);

	freearg(d);

	freearg(u);

	freearg(v);

}

dsin(s)

char *s;

{

	char *a, *d, *copyarg();

	a = copyarg(s + 8);

	d = copyarg(s + 3 + arglen(s + 3));

	delarg(s);

	insarg(s, "{Times {Cos } {D } } ");

	insarg(s + 17, d);

	insarg(s + 17, a);

	insarg(s + 12, a);

	freearg(a);

	freearg(d);

}

dcos(s)

char *s;

{

	char *a, *d, *copyarg();

	a = copyarg(s + 8);

	d = copyarg(s + 3 + arglen(s + 3));

	delarg(s);

	insarg(s, "{Times -1 {Sin } {D } } ");

	insarg(s + 20, d);

	insarg(s + 20, a);

	insarg(s + 15, a);

	freearg(a);

	freearg(d);

}

darctan(s)

char *s;

{

	char *d, *u;

	u = copyarg(arg1(arg1(s)));

	d = copyarg(arg2(s));

	delarg(s);

	/*                   1         2         3         4		*/

	/*         01234567890123456789012345678901234567890123456	*/

	insarg(s, "{Times {Power {Plus 1 {Power 2 } } -1 } {D } } ");

	insarg(s + 43, d);

	insarg(s + 43, u);

	insarg(s + 29, u);

	freearg(u);

	freearg(d);

}

TensorProduct(s)

char *s;

{

	if (strncmp(s, "{TensorProduct ", 15) == 0) {

		delarg(s + 1);

		insarg(s + 1, "Times ");

		insarg(s + 1, "Outer ");

	}

}

Transpose(s)

char *s;

{

	int a[10], i, k, n, nn;

	char *arg1, *arg2, *t, *copyarg();

	if (strncmp(s, "{Transpose ", 11) == 0) {

		n = argcount(s);

		if (n > 1)

			arg1 = s + 11;

		if (n > 2)

			arg2 = arg1 + arglen(arg1);

		if (n == 3 && strncmp(arg2, "{List ", 6) == 0) {

			n = argcount(arg2) - 1;

			if (n > 10)

				return;

			for (i = 0; i < n; i++)

				a[i] = 0;

			t = arg2 + 6;

			for (i = 0; i < n; i++) {

				k = 0;

				sscanf(t, "%d", &k);

				if (k < 1 || k > n || a[k - 1])

					return;

				a[k - 1] = i + 1;

				t += arglen(t);

			}

			/* bubble sort */

			n = n - 1;

			while (n) {

				nn = n;

				n = 0;

				for (i = 0; i < nn; i++)

					if (a[i] > a[i + 1]) {

						swap(arg1, 0, i);

						k = a[i];

						a[i] = a[i + 1];

						a[i + 1] = k;

						n = i;

					}

			}

			t = copyarg(arg1);

			delarg(s);

			insarg(s, t);

			freearg(t);

		} else if (n == 2) {

			swap(arg1, 0, 0);

			t = copyarg(arg1);

			delarg(s);

			insarg(s, t);

			freearg(t);

		}

	}

}

/* {List {List a b c } {List d e f } } = {List {List a d } {List b e } {List c f } } */

/* s=tensor  k=current depth  d=desired depth */

swap(s, k, d)

char *s;

int k, d;

{

	int i, j, m, n;

	char *t, *u, *copyarg(), *elem();

	if (strncmp(s, "{List {List ", 12) == 0)

		if (k < d) {

			n = argcount(s) - 1;

			s += 6;

			for (i = 0; i < n; i++) {

				swap(s, k + 1, d);

				s += arglen(s);

			}

		} else {

			m = argcount(s) - 1;

			n = argcount(s + 6) - 1;

			t = copyarg(s);

			delarg(s);

			insarg(s, "{List } ");

			s += 6;

			for (i = 0; i < n; i++) {

				insarg(s, "{List } ");

				s += 6;

				for (j = 0; j < m; j++) {

					u = elem(t, j, i);

					if (u) {

						insarg(s, u);

						s += arglen(s);

					}

				}

				s += 2;

			}

			freearg(t);

		}

}

/* s[i][j] */

char *elem(s, i, j)

char *s;

int i, j;

{

	int k;

	s += 6;

	for (k = 0; k < i; k++)

		s += arglen(s);

	if (strncmp(s, "{List ", 6) == 0 && j < argcount(s) - 1) {

		s += 6;

		for (k = 0; k < j; k++)

			s += arglen(s);

		return s;

	} else

		return NULL;

}

Tensor(s)

char *s;

{

	int i, n;

	char *t, u[20];

	if (strncmp(s, "{Tensor ", 8) == 0) {

		n = argcount(s);

		if (n == 1)

			delarg(s);

		else if (n == 2) {

			t = copyarg(s + 8);

			delarg(s);

			insarg(s, t);

			freearg(t);

		} else {

			n = 0;

			sscanf(s + 8 + arglen(s + 8), "%d", &n);

			delarg(s + 8 + arglen(s + 8));

			t = copyarg(s);

			delarg(s);

			insarg(s, "{List } ");

			s += 6;

			for (i = 0; i < n; i++) {

				insarg(s, t);

				sprintf(u, "%d", i + 1);

				insarg(s + 8 + arglen(s + 8) - 1, u);

				s += arglen(s);

			}

			freearg(t);

		}

	}

}

Contract(s)

char *s;

{

	int d, i, m, n;

	char *a1, *a2, *t, *copyarg(), *elem();

	if (strncmp(s, "{Contract ", 10) == 0 && argcount(s) == 4) {

		t = s + 10;

		a1 = t + arglen(t);

		a2 = a1 + arglen(a1);

		m = 0;

		n = 0;

		sscanf(a1, "%d", &m);

		sscanf(a2, "%d", &n);

		if (m == 0 || n == 0 || m == n)

			return;

		if (n < m) {

			i = m;

			m = n;

			n = i;

		}

		while (m > 1) {

			m--;

			swap(t, 1, m);

		}

		while (n > 2) {

			n--;

			swap(t, 1, n);

		}

		d = argcount(t) - 1;

		t = copyarg(t);

		delarg(s);

		insarg(s, "{Plus } ");

		s += 6;

		for (i = 0; i < d; i++) {

			insarg(s, elem(t, i, i));

			s += arglen(s);

		}

		freearg(t);

	}

}

WedgeProduct(s)

char *s;

{

	int n;

	char *a, *b;

	if (strncmp(s, "{WedgeProduct ", 14) == 0) {

		n = argcount(s) - 1;

		if (n < 2)

			return;

		a = copyarg(s + 14);

		b = copyarg(s + 14 + arglen(s + 14));

		if (n == 2)

			delarg(s);

		else {

			s += 14;

			delarg(s);

			delarg(s);

		}

		insarg(s, "{Plus {TensorProduct } {Times -1 {TensorProduct } } } ");

		insarg(s + 48, a);

		insarg(s + 48, b);

		insarg(s + 21, b);

		insarg(s + 21, a);

		freearg(a);

		freearg(b);

	}

}

Gradient(s)

char *s;

{

	if (strncmp(s, "{Gradient ", 10) == 0 && argcount(s) == 3) {

		delarg(s + 1);

		insarg(s + 1, "D ");

		insarg(s + 1, "Outer ");

	}

}

Part(s)

char *s;

{

	int i, n;

	char *t, *copyarg();

	if (strncmp(s, "{Part ", 6) == 0 && isnumeric(s + 6 + arglen(s + 6))) {

		n = 0;

		sscanf(s + 6 + arglen(s + 6), "%d", &n);

		if (0 < n && n < argcount(s + 6)) {

			t = s + 7;

			for (i = 0; i < n; i++)

				t += arglen(t);

			t = copyarg(t);

			delarg(s);

			insarg(s, t);

			freearg(t);

		}

	}

}

Outer(s)

char *s;

{

	int i, n;

	char *a, *b, *f, *copyarg();

	if (strncmp(s, "{Outer ", 7) == 0) {

		n = argcount(s);

		if (n < 4)

			return;

		f = copyarg(s + 7);

		a = copyarg(s + 7 + arglen(s + 7));

		delarg(s + 7 + arglen(s + 7));

		if (n == 4)

			b = copyarg(s + 7 + arglen(s+7));

		else

			b = copyarg(s);

		delarg(s);

		if (strncmp(a, "{List ", 6) == 0) {

			insarg(s, a);

			freearg(a);

			n = argcount(s) - 1;

			s += 6;

			for (i = 0; i < n; i++) {

				a = copyarg(s);

				delarg(s);

				insarg(s, "{Outer } ");

				insarg(s + 7, b);

				insarg(s + 7, a);

				insarg(s + 7, f);

				freearg(a);

				s += arglen(s);

			}

			freearg(b);

		} else if (strncmp(b, "{List ", 6) == 0) {

			insarg(s, b);

			freearg(b);

			n = argcount(s) - 1;

			s += 6;

			for (i = 0; i < n; i++) {

				b = copyarg(s);

				delarg(s);

				insarg(s, "{Outer } ");

				insarg(s + 7, b);

				insarg(s + 7, a);

				insarg(s + 7, f);

				freearg(b);

				s += arglen(s);

			}

			freearg(a);

		} else {

			insarg(s, "{} ");

			insarg(s + 1, b);

			insarg(s + 1, a);

			insarg(s + 1, f);

			freearg(a);

			freearg(b);

		}

		freearg(f);

	}

}

Dot(s)

char *s;

{

	int n, r;

	char *a, *b, u[24];

	if (strncmp(s, "{Dot ", 5) == 0) {

		n = argcount(s) - 1;

		if (n < 2)

			return;

		if (strncmp(s + 5, "{List ", 6))

			return;

		if (strncmp(s + 5 + arglen(s + 5), "{List ", 6))

			return;

		/* determine rank of first argument */

		a = s + 5;

		r = 0;

		while (strncmp(a, "{List ", 6) == 0) {

			r++;

			a += 6;

		}

		a = copyarg(s + 5);

		b = copyarg(s + 5 + arglen(s + 5));

		if (n == 2)

			/* replace entire dot function */

			delarg(s);

		else {

			/* replace first two arguments */

			s += 5;

			delarg(s);

			delarg(s);

		}

		insarg(s, "{Contract {TensorProduct } } ");

		sprintf(u, "%d ", r + 1);

		insarg(s + 27, u);

		sprintf(u, "%d ", r);

		insarg(s + 27, u);

		insarg(s + 25, b);

		insarg(s + 25, a);

		freearg(a);

		freearg(b);

	}

}

Set(s)

char *s;

{

	char *t, *copyarg();

	if (strncmp(s, "{Set ", 5) == 0) {

		expr[nexpr] = copyarg(s);

		valloc += strlen(expr[nexpr]);

		nexpr++;

		t = copyarg(s + 5 + arglen(s + 5));

		delarg(s);

		insarg(s, t);

		freearg(t);

	}

}

format(s)

char *s;

{

	int i, n;

	if (strncmp(s, "{Plus ", 6) == 0) {

		n = argcount(s) - 1;

		s += 6;

		for (i = 0; i < n; i++) {

			format_term(s, i);

			s += arglen(s);

		}

	} else if (strncmp(s, "{Times ", 7) == 0) {

		n = argcount(s) - 1;

		s += 7;

		if (strncmp(s, "-1 ", 3) == 0) {

			printf("-");

			s += 3;

			n--;

		}

		for (i = 0; i < n; i++) {

			format_factor(s, i);

			s += arglen(s);

		}

	} else if (strncmp(s, "{Power ", 7) == 0) {

		s += 7;

		if (*s == '-' || strncmp(s, "{Plus ", 6) == 0 || strncmp(s, "{Times ", 7) == 0) {

			printf("(");

			format(s);

			printf(")");

		} else

			format(s);

		printf("^");

		s += arglen(s);

		if (strncmp(s, "{Plus ", 6) == 0 || strncmp(s, "{Times ", 7) == 0) {

			printf("(");

			format(s);

			printf(")");

		} else

			format(s);

	} else if (strncmp(s, "{List ", 6) == 0) {

		n = argcount(s) - 1;

		s += 6;

		printf("{");

		for (i = 0; i < n; i++) {

			format_component(s, i);

			s += arglen(s);

		}

		printf("}");

	} else if (*s == '{') {

		n = argcount(s) - 1;

		s++;

		format(s);

		s += arglen(s);

		printf("[");

		for (i = 0; i < n; i++) {

			format_arg(s, i);

			s += arglen(s);

		}

		printf("]");

	} else {

		n = arglen(s) - 1;

		for (i = 0; i < n; i++)

			printf("%c", s[i]);

	}

}

format_term(s, n)

char *s;

int n;

{

	if (strncmp(s, "{Times -", 8) == 0)

		format(s);

	else {

		if (n > 0)

			printf("+");

		format(s);

	}

}

format_factor(s, n)

char *s;

int n;

{

	if (n > 0)

		printf("*");

	if (strncmp(s, "{Plus ", 6) == 0) {

		printf("(");

		format(s);

		printf(")");

	} else

		format(s);

}

format_arg(s, n)

char *s;

int n;

{

	if (n > 0)

		printf(", ");

	format(s);

}

format_component(s, n)

char *s;

int n;

{

	if (n > 0)

		printf(", ");

	format(s);

}
