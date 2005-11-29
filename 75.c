/* Another experimental parser.
This one simplifies expressions. */

#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>

#include <math.h>

#include <string.h>

#define MAX 1000

char str[MAX];

char s1[MAX];

char s2[MAX];

char argbuf[MAX];

char a[20];

main()

{
	int n = 1;

/*	test1(); */

	for (;;) {

		printf("\nIn[%d]: ", n);

		gets(s2);

		inputformtofullform(s1, s2);

		printf("%s\n", s1);

		norm(s1);

		printf("Out[%d]: %s\n", n, s1);

		n++;

	}

}

/*---------------------------------------------------------------------------*/

int yn;

char *ys, *yy;

inputformtofullform(s1, s2)

char *s1, *s2;

{

	yn = 0;

	yy = s1;

	ys = s2;

	yscan(0);

	yexpr();

	if (*ys)

		yerr(2);	/* syntax error */

	youtc('\0');

}

yexpr()

{

	y32();

}

/* expr := expr */

y32()

{

	int n = yn;

	y24();

	if (ys[0] == ':' && ys[1] == '=') {

		yscan(2);

		youts(n, "(SetDelayed ");

		y24();

		youts(yn, ") ");

	}

}

/* expr || expr || expr */

y24()

{

	int n = yn;

	y23();

	if (ys[0] == '|' && ys[1] == '|') {

		youts(n, "(Or ");

		do {

			yscan(2);

			y23();

		} while (ys[0] == '|' && ys[1] == '|');

		youts(yn, ") ");

	}

}

/* expr && expr && expr */

y23()

{

	int n = yn;

	y22();

	if (ys[0] == '&' && ys[1] == '&') {

		youts(n, "(And ");

		do {

			yscan(2);

			y22();

		} while (ys[0] == '&' && ys[1] == '&');

		youts(yn, ") ");

	}

}

/* ! expr */

y22()

{

	if (*ys == '!') {

		youts(yn, "(Not ");

		yscan(1);

		y22();

		youts(yn, ") ");

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

			youts(n, "(Equal ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '=');

			youts(yn, ") ");

		} else if (ys[0] == '!' && ys[1] == '=') {

			youts(n, "(Unequal ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '!');

			youts(yn, ") ");

		} else if (ys[0] == '>' && ys[1] != '=' && ys[1] != '>') {

			youts(n, "(Greater ");

			do {

				yscan(1);

				y20();

			} while (ys[0] == '>' && ys[1] != '=' && ys[1] != '>');

			youts(yn, ") ");

		} else if (ys[0] == '>' && ys[1] == '=') {

			youts(n, "(GreaterEqual ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '>' && ys[1] == '=');

			youts(yn, ") ");

		} else if (ys[0] == '<' && ys[1] != '=') {

			youts(n, "(Less ");

			do {

				yscan(1);

				y20();

			} while (ys[0] == '<' && ys[1] != '=');

			youts(yn, ") ");

		} else if (ys[0] == '<' && ys[1] == '=') {

			youts(n, "(LessEqual ");

			do {

				yscan(2);

				y20();

			} while (ys[0] == '<' && ys[1] == '=');

			youts(yn, ") ");

		} else

			break;

}

/* expr + expr + expr */

y20()

{

	int n = yn;

	yy20();

	if (ys[0] == '+' && ys[1] != '+' && ys[1] != '=') {

		youts(n, "(Plus ");

		do {

			yscan(1);

			yy20();

		} while (ys[0] == '+' && ys[1] != '+' && ys[1] != '=');

		youts(yn, ") ");

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

		youts(n, "(Plus ");

		youts(yn, "(Times -1 ");

		y19();

		youts(yn, ") ) ");

	}

}

/* expr expr expr */

/* expr * expr * expr */

y19()

{

	int n = yn;

	y18();

	if (yisfactor()) {

		youts(n, "(Times ");

		do {

			if (*ys == '*')

				yscan(1);

			y18();

		} while (yisfactor());

		youts(yn, ") ");

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

		youts(n, "(Times ");

		youts(yn, "(Power ");

		y17();

		youts(yn, "-1 ) ) ");

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

		youts(yn, "(Times -1 ");

		y16();

		youts(yn, ") ");

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

		youts(n, "(Power ");

		yscan(1);

		y16();

		youts(yn, ") ");

	}

}

/* expr . expr . expr */

y15()

{

	int n = yn;

	y14();

	if (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1])) {

		youts(n, "(Dot ");

		do {

			yscan(1);

			y14();

		} while (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1]));

		youts(yn, ") ");

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

		youts(n, "(NonCommutativeMultiply ");

		do {

			yscan(2);

			y13();

		} while (ys[0] == '*' && ys[1] == '*');

		youts(yn, ") ");

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

		youts(n, "(Factorial ");

		youts(yn, ") ");

	} else if (ys[0] == '!' && ys[1] == '!') {

		yscan(2);

		youts(n, "(Factorial2 ");

		youts(yn, ") ");

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

		youts(n, "(Map ");

		y11();

		youts(yn, ") ");

	} else if (ys[0] == '/' && ys[1] == '/' && ys[2] == '@') {

		yscan(3);

		youts(n, "(MapAll ");

		y11();

		youts(yn, ") ");

	} else if (ys[0] == '@' && ys[1] == '@') {

		yscan(2);

		youts(n, "(Apply ");

		y11();

		youts(yn, ") ");

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

		youts(yn, ") ");

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

		youts(yn, "(PreIncrement ");

		y7();

		youts(yn, ") ");

	} else if (ys[0] == '-' && ys[1] == '-') {

		yscan(2);

		youts(yn, "(PreDecrement ");

		y7();

		youts(yn, ") ");

	} else {

		y7();

		if (ys[0] == '+' && ys[1] == '+') {

			yscan(2);

			youts(n, "(Increment ");

			youts(yn, ") ");

		} else if (ys[0] == '-' && ys[1] == '-') {

			yscan(2);

			youts(n, "(Decrement ");

			youts(yn, ") ");

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

			youts(n, "(");

			yscan(1);

			yexpr();

			while (*ys == ',') {

				yscan(1);

				yexpr();

			}

			if (ys[0] == ']' && ys[1] != ']')

				yscan(1);

			else

				yerr(3);	/* ] expected */

			youts(yn, ") ");

		} else if (ys[0] == '[' && ys[1] == '[') {

			youts(n, "(Part ");

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

			youts(yn, ") ");

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

		youts(n, "(PatternTest ");

		y5();

		youts(yn, ") ");

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

			youts(n, "(Pattern ");

			youts(yn, "(Blank ");

			if (*ys == '(' || isalpha(*ys))

				y1();

			else if (*ys == '.') {

				ys++;

				youts(n, "(Optional ");

				youts(yn, ") ");

			}

			youts(yn, ") ) ");

		}

		yscan(0);

	} else if (isdigit(*ys)) {

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

		youts(yn, "(List ");

		yexpr();

		while (*ys == ',') {

			yscan(1);

			yexpr();

		}

		if (*ys == '}')

			yscan(1);

		else

			yerr(6);	/* } expected */

		youts(yn, ") ");

	} else

		yerr(2);		/* syntax error */

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

	if (yn + len > MAX)	/* if yn + len == max then yn - 1 + len < max */

		yerr(1);	/* out of memory */

	else {

		for (i = yn - 1; i >= n; i--)

			yy[i + len] = yy[i];

		for (i = 0; i < len; i++)

			yy[n + i] = s[i];

		yn += len;

	}

}

youtc(c)

int c;

{

	if (yn == MAX)

		yerr(1);	/* out of memory */

	else

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

flatten(s, head)

char *s, *head;

{

	int i, j, k, n;

	char *t;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			flatten(t, head);

			t += arglen(t);

		}

		if (equalhead(s + 1, head)) {

			t = s + arglen(s + 1) + 1;

			for (i = 0; i < n - 1; i++) {

				if (*t == '(' && equalhead(t + 1, head)) {

					k = argcount(t);

					delhead(t);

					for (j = 0; j < k - 1; j++)

						t += arglen(t);

				} else

					t += arglen(t);

			}

		}

	}

}

delarg(s)

char *s;

{

	char *t;

	t = s + arglen(s);

	while (*t)

		*s++ = *t++;

	*s = 0;

}

insarg(s, t)

char *s, *t;

{

	int i, len, n;

	n = strlen(t) + 1;

	len = strlen(s);

	for (i = len; i >= 0; i--)

		s[i + n] = s[i];

	for (i = 0; i < n - 1; i++)

		s[i] = t[i];

	s[i] = ' ';

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

equalhead(s, head)

char *s, *head;

{

	while (*head)

		if (*s++ != *head++)

			return 0;

	if (*s == ' ')

		return 1;

	else

		return 0;

}

eqarg(s, arg)

char *s, *arg;

{

	while (*arg)

		if (*s++ == *arg++)

			;

		else

			return 0;

	if (*s == ' ')

		return 1;

	else

		return 0;

}

/* convert "(head expr expr ) " to "expr expr " */

delhead(s)

char *s;

{

	int i, j, k, n;

	char *t;

	n = argcount(s);

	t = s + arglen(s + 1) + 1;

	for (i = 0; i < n - 1; i++) {

		k = arglen(t);

		for (j = 0; j < k; j++)

			*s++ = *t++;

	}

	t += 2;		/* skip closing parenthesis */

	while (*t)

		*s++ = *t++;

	*s = 0;

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

	if (*s == '(') {

		n = 3;

		s++;

		while (*s != ')') {

			k = arglen(s);

			n += k;

			s += k;

		}

	} else {

		n = 1;

		while (*s != ' ') {

			n++;

			s++;

		}

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

	if (*s == '(') {

		s++;

		for (;;) {

			s += arglen(s);

			if (*s == ')')

				break;

			n++;

		}

	}

	return n;

}

/* s points to the first argument */

/* k is the number of arguments following the first one */

/* example:  */

/* s = a,b,c) */

/* k = 2 */

/* returns s = b,c,a) */

rotate(k, s)

int k;

char *s;

{

	int i, j, len, n;

	char *t;

	len = arglen(s);

	for (i = 0; i < len; i++)

		argbuf[i] = s[i];

	t = s + len;

	for (i = 0; i < k; i++) {

		n = arglen(t);

		for (j = 0; j < n; j++)

			*s++ = *t++;

	}

	for (i = 0; i < len; i++)

		s[i] = argbuf[i];

}

addnum(s)

char *s;

{

	int i, n;

	char *t, u[20];

	double d, x;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			addnum(t);

			t += arglen(t);

		}

		if (equalhead(s + 1, "Plus")) {

			x = 0.0;

			t = s + arglen(s + 1) + 1;

			for (i = 0; i < n - 1; i++)

				if (isnumeric(t)) {

					sscanf(t, "%lg", &d);

					x += d;

					delarg(t);

				} else

					t += arglen(t);

			if (argcount(s) == 1) {

				delarg(s);

				sprintf(u, "%g", x);

				insarg(s, u);

			} else if (x != 0.0) {

				sprintf(u, "%g", x);

				t = s + arglen(s + 1) + 1;

				insarg(t, u);

			} else if (argcount(s) == 2)

				delhead(s);

		}

	}

}

mulnum(s)

char *s;

{

	int i, n;

	char *t, u[20];

	double d, x;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			mulnum(t);

			t += arglen(t);

		}

		if (equalhead(s + 1, "Times")) {

			x = 1.0;

			t = s + arglen(s + 1) + 1;

			for (i = 0; i < n - 1; i++)

				if (isnumeric(t)) {

					sscanf(t, "%lg", &d);

					x *= d;

					delarg(t);

				} else

					t += arglen(t);

			if (x == 0.0) {

				delarg(s);

				insarg(s, "0");

			} else if (argcount(s) == 1) {

				delarg(s);

				sprintf(u, "%g", x);

				insarg(s, u);

			} else if (x != 1.0) {

				sprintf(u, "%g", x);

				t = s + arglen(s + 1) + 1;

				insarg(t, u);

			} else if (argcount(s) == 2)

				delhead(s);

		}

	}

}

pownum(s)

char *s;

{

	int i, n;

	char *t, u[20];

	double x, y;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			pownum(t);

			t += arglen(t);

		}

		if (equalhead(s + 1, "Power")) {

			t = s + arglen(s + 1) + 1;

			if (isnumeric(t) && isnumeric(t + arglen(t))) {

				sscanf(t, "%lg%lg", &x, &y);

				sprintf(u, "%g", pow(x, y));

				delarg(s);

				insarg(s, u);

			}

		}

	}

}

/* compare expressions */

isequal(s, t)

char *s, *t;

{

	int n;

	if (*s == '(' && *t == '(') {

		if (equalhead(s + 1, "Plus") && equalhead(t + 1, "Plus")) {

			n = argcount(s);

			if (n == argcount(t))

				return isequalarglist(n - 1, s + arglen(s + 1) + 1, t + arglen(t + 1) + 1);

			else

				return 0;

		}

		if (equalhead(s + 1, "Times") && equalhead(t + 1, "Times")) {

			n = argcount(s);

			if (n == argcount(t))

				return isequalarglist(n - 1, s + arglen(s + 1) + 1, t + arglen(t + 1) + 1);

			else

				return 0;

		}

	}

	while (*s == *t) {

		if (*s == ' ')

			return 1;

		s++;

		t++;

	}

	return 0;

}

isequalarglist(n, s, t)

int n;

char *s, *t;

{

	int i;

	if (n == 1)

		return isequal(s, t);

	else

		for (i = 0; i < n; i++) {

			if (isequal(s, t))

				return isequalarglist(n - 1, s + arglen(s), t + arglen(t));

			else

				rotate(n - 1, s);

		}

	return 0;

}

test1()

{

	gets(str);

	inputformtofullform(s1, str);

	gets(str);

	inputformtofullform(s2, str);

	printf("%s\n", s1);

	printf("%s\n", s2);

	if (isequal(s1, s2))

		printf("equal\n");

	else

		printf("not equal\n");

	printf("%s\n", s1);

	printf("%s\n", s2);

	exit(1);

}

norm(s)

char *s;

{

	char *t;

	t = malloc(MAX);

	if (t == NULL)

		outofmemory();

	for (;;) {

		strcpy(t, s);

		flatten(s, "Plus");

		flatten(s, "Times");

		pownum(s);

		mulnum(s);

		addnum(s);

		expandnumericalpowers(s);

		expandnumericalproducts(s);

		simplifysums(s);

		simplifyproducts(s);

		simplifypowers(s);

		if (strcmp(t, s) == 0)

			break;

	}

	free(t);

}

outofmemory()

{

	printf("out of memory\n");

	exit(1);

}

simplifyproducts(s)

char *s;

{

	int i, j, k, n;

	char *t, *u;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			simplifyproducts(t);

			t += arglen(t);

		}

		if (equalhead(s + 1, "Times")) {

			t = s + 1;

			for (i = 1; i < n - 1; i++) {

				t += arglen(t);

				u = t + arglen(t);

				for (j = i + 1; j < n; j++)

					if (similarfactors(t, u)) {

						foldfactors(t, u);

						n--;

						u = t + arglen(t);

						for (k = i + 1; k < j; k++)

							u += arglen(u);

					} else

						u += arglen(u);

			}

			if (argcount(s) == 2)

				delhead(s);

		}

	}

}

similarfactors(s, t)

char *s, *t;

{

	if (*s == '(' && equalhead(s + 1, "Power")) {

		s += arglen(s + 1) + 1;

		if (isnumeric(s + arglen(s)))

			;

		else

			return 0;

	}

	if (*t == '(' && equalhead(t + 1, "Power")) {

		t += arglen(t + 1) + 1;

		if (isnumeric(t + arglen(t)))

			;

		else

			return 0;

	}

	return isequal(s, t);

}

foldfactors(s, t)

char *s, *t;

{

	char *u;

	double x, y;

	if (*t == '(' && equalhead(t + 1, "Power")) {

		u = t + arglen(t + 1) + 1;

		u += arglen(u);

		sscanf(u, "%lg", &y);

	} else

		y = 1.0;

	delarg(t);

	if (*s == '(' && equalhead(s + 1, "Power")) {

		u = s + arglen(s + 1) + 1;

		u += arglen(u);

		sscanf(u, "%lg", &x);

		sprintf(a, "%g", x + y);

		delarg(u);

		insarg(u, a);

	} else {

		sprintf(a, "%g", 1.0 + y);

		insarg(s, "(Power");

		s++;

		s += arglen(s);

		s += arglen(s);

		insarg(s, a);

		s += arglen(s);

		insarg(s, ")");

	}

}

simplifypowers(s)

char *s;

{

	int i, n;

	char *t;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			simplifypowers(t);

			t += arglen(t);

		}

		if (eqarg(s + 1, "Power")) {

			t = s + 1;

			t += arglen(t);

			t += arglen(t);

			if (eqarg(t, "0")) {

				delarg(s);

				insarg(s, "1");

			} else if (eqarg(t, "1")) {

				delarg(t);

				delhead(s);

			}

		}

	}

}

simplifysums(s)

char *s;

{

	int i, j, k, n;

	char *t, *u;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			simplifysums(t);

			t += arglen(t);

		}

		if (eqarg(s + 1, "Plus")) {

			t = s + 1;

			for (i = 1; i < n - 1; i++) {

				t += arglen(t);

				u = t + arglen(t);

				for (j = i + 1; j < n; j++)

					if (similarterms(t, u)) {

						foldterms(t, u);

						n--;

						u = t + arglen(t);

						for (k = i + 1; k < j; k++)

							u += arglen(u);

					} else

						u += arglen(u);

			}

			if (argcount(s) == 2)

				delhead(s);

		}

	}

}

similarterms(s, t)

char *s, *t;

{

	int ns, nt;

	if (*s == '(' && eqarg(s + 1, "Times")) {

		ns = argcount(s) - 1;

		s++;

		s += arglen(s);

		if (isnumeric(s)) {

			s += arglen(s);

			ns--;

		}

	} else

		ns = 1;

	if (*t == '(' && eqarg(t + 1, "Times")) {

		nt = argcount(t) - 1;

		t++;

		t += arglen(t);

		if (isnumeric(t)) {

			t += arglen(t);

			nt--;

		}

	} else

		nt = 1;

	if (ns == nt)

		return isequalarglist(ns, s, t);

	else

		return 0;

}

foldterms(s, t)

char *s, *t;

{

	char *u;

	double sn, tn;

	tn = 1.0;

	if (*t == '(' && eqarg(t + 1, "Times")) {

		u = t + 1;

		u += arglen(u);

		if (isnumeric(u))

			sscanf(u, "%lg", &tn);

	}

	delarg(t);

	if (*s == '(' && equalhead(s + 1, "Times")) {

		u = s + 1;

		u += arglen(u);

		if (isnumeric(u)) {

			sscanf(u, "%lg", &sn);

			sprintf(a, "%g", sn + tn);

			delarg(u);

			insarg(u, a);

		} else {

			sprintf(a, "%g", 1.0 + tn);

			insarg(u, a);

		}

	} else {

		sprintf(a, "%g", 1.0 + tn);

		insarg(s, "(Times");

		s++;

		s += arglen(s);		/* skip times */

		insarg(s, a);		/* put in number */

		s += arglen(s);		/* skip number */

		s += arglen(s);		/* skip original arg */

		insarg(s, ")");

	}

}

expandnumericalpowers(s)

char *s;

{

	int i, n;

	char *a, *t, *u;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			expandnumericalpowers(t);

			t += arglen(t);

		}

		t = s + arglen(s + 1) + 1;

		u = t + arglen(t);

		if (eqarg(s, "(Power") && eqarg(t, "(Times") && isnumeric(u)) {

			n = arglen(u) - 1;

			a = malloc(n + 1);

			if (a == NULL)

				outofmemory();

			for (i = 0; i < n; i++)

				a[i] = u[i];

			a[n] = 0;

			delarg(u);

			delhead(s);

			n = argcount(s) - 1;

			s += 7;		/* skip "(Times " */

			for (i = 0; i < n; i++) {

				insarg(s, "(Power");

				s += 7;			/* skip "(Power " */

				s += arglen(s);

				insarg(s, a);

				s += arglen(s);

				insarg(s, ")");

				s += 2;

			}

			free(a);

		}

	}

}

expandnumericalproducts(s)

char *s;

{

	int i, n;

	char *k, *t, *u;

	if (*s == '(') {

		n = argcount(s);

		t = s + 1;

		for (i = 0; i < n; i++) {

			expandnumericalproducts(t);

			t += arglen(t);

		}

		t = s + arglen(s + 1) + 1;

		u = t + arglen(t);

		if (n == 3 && eqarg(s, "(Times") && isnumeric(t) && eqarg(u, "(Plus")) {

			n = arglen(t) - 1;

			k = malloc(n + 1);

			if (k == NULL)

				outofmemory();

			for (i = 0; i < n; i++)

				k[i] = t[i];

			k[n] = 0;

			delarg(t);

			delhead(s);

			n = argcount(s) - 1;

			s += 6;				/* skip "(Plus " */

			for (i = 0; i < n; i++) {

				insarg(s, "(Times");

				s += 7;			/* skip "(Times " */

				insarg(s, k);

				s += arglen(s);		/* skip k */

				s += arglen(s);		/* skip arg */

				insarg(s, ")");

				s += 2;

			}

			free(k);

		}

	}

}
