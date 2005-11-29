/* Another experimental Mathematica parser.  This one folds constants. */

#include <stdio.h>

#include <ctype.h>

char s1[1000], s2[1000];

main()

{
	int n = 1;

	for (;;) {

		printf("\nIn[%d]: ", n);

		gets(s2);

		inputformtofullform(s1, s2);

		printf("%s\n", s1);

		flatten(s1, "Plus");

		flatten(s1, "Times");

		for (;;) {

			strcpy(s2, s1);

			addnum(s1);

			mulnum(s1);

			deladd(s1);

			delmul(s1);

			if (strcmp(s1, s2) == 0)

				break;

		}

		printf("Out[%d]: %s\n", n, s1);

/*		rotate(s1);
		printf("Out[%d]: %s\n", n, s1);

		rotate(s1);
		printf("Out[%d]: %s\n", n, s1);

		rotate(s1);
		printf("Out[%d]: %s\n", n, s1);
*/
		n++;

	}

}

flatten(s, head)

char *s, *head;

{

	char *t;

	if (*s == '(') {

		t = s;

		do {

			t++;

			flatten(t, head);

			t += arglen(t);

		} while (*t == ',');

		if (equalhead(s + 1, head)) {

			t = s + arglen(s + 1);

			do {

				t++;

				if (*t == '(' && equalhead(t + 1, head))

					delhead(t);

				else

					t += arglen(t);

			} while (*t == ',');

		}

	}

}

/*---------------------------------------------------------------------------*/

delarg(s)
char *s;
{
	char *t;

	t = s + arglen(s);

	if (*t == 0) {
		*s = 0;
		return;
	}

	s--;

	while (*t)
		*s++ = *t++;

	*s = 0;
}

insarg(s, t)

char *s, *t;

{

	int i, len, n;

	len = strlen(s);

	if (len == 0) {

		strcpy(s, t);

		return;

	}

	n = strlen(t);

	for (i = len + 1; i >= 0; i--)

		s[i + n + 1] = s[i];	/* n + 1 to make room for comma */

	*s++ = ',';

	for (i = 0; i < n; i++)

		s[i] = t[i];

}

is_numeric(s)
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

	if (*s == ',' || *s == ')' || *s == 0)

		return 1;

	else

		return 0;

}

/*

convert ( head, expr, expr ) to expr, expr

example

	s = "(Times,a,b),c)"

	l = 14, the length of "(Times,a,b),c)"

	m = 11, the length of "(Times,a,b)"

	n = 5, the length of "Times"

	total number of char removed is length of "Times" plus left paren plus

	comma plus right paren

	n + 3 = 8 

step 1

	move the arguments

	"(Times,a,b),c)" becomes

	"a,bmes,a,b),c)"

	the number of char to move is the length of "(Times,a,b)" minus the

	length of "Times" minus left paren minus comma minus right paren

	m - n - 3 = 11 - 5 - 3 = 3

step 2

	move the remainder of the string

	",a,bmes,a,b),c)" becomes

	",a,b,c)"

	the number of char to move is the length of "(Times,a,b),c)" minus the

	length of "(Times,a,b)" plus one for end of string

	l - m + 1 = 14 - 11 + 1 = 4

*/

delhead(s)

char *s;

{
	int i, l, m, n;

	l = strlen(s);

	m = arglen(s);

	n = arglen(s + 1);

	for (i = 0; i < m - n - 3; i++)

		s[i] = s[i + n + 2];

	s += m - n - 3;

	for (i = 0; i < l - m + 1; i++)

		s[i] = s[i + n + 3];

}

/*

s points to the argument

s = "(times,a,b)" returns 11

s = "times,a,b)" returns 5

*/

arglen(s)

char *s;

{

	int len = 0, n = 0;

	for (;;) {

		switch (*s++) {

		case 0:

			return len;

		case ',':

			if (n == 0)

				return len;

			break;

		case '(':

			n++;

			break;

		case ')':

			if (n == 0)

				return len;

			if (n == 1)

				return len + 1;

			n--;

			break;

		default:

			break;

		}

		len++;

	}

}

/*

s points to the expression

s = "(times,a,b)" returns 3

s = "times,a,b)" returns 1

*/

argcount(s)

char *s;

{

	int n;

	if (*s == 0)

		return 0;

	n = 1;

	if (*s == '(')

		for (;;) {

			s++;

			s += arglen(s);

			if (*s == ',')

				n++;

			else

				break;

		}

	return n;

}

char argbuf[1000];

rotate(s)

char *s;

{

	int i, l, m, n, x;

	l = arglen(s);

	m = arglen(s + 1);

	n = arglen(s + m + 2);

	s += m + 2;

	for (i = 0; i < n; i++)

		argbuf[i] = s[i];

	x = l - m - n - 4;

	for (i = 0; i < x; i++)

		s[i] = s[i + n + 1];

	s += x;

	*s++ = ',';

	for (i = 0; i < n; i++)

		s[i] = argbuf[i];

}

#define MAX 1000

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

	y24();

}

/* expr || expr || expr */

y24()

{

	int n = yn;

	y23();

	if (ys[0] == '|' && ys[1] == '|') {

		youts(n, "(Or,");

		do {

			youtc(',');

			yscan(2);

			y23();

		} while (ys[0] == '|' && ys[1] == '|');

		youtc(')');

	}

}

/* expr && expr && expr */

y23()

{

	int n = yn;

	y22();

	if (ys[0] == '&' && ys[1] == '&') {

		youts(n, "(And,");

		do {

			youtc(',');

			yscan(2);

			y22();

		} while (ys[0] == '&' && ys[1] == '&');

		youtc(')');

	}

}

/* ! expr */

y22()

{

	if (*ys == '!') {

		youts(yn, "(Not,");

		yscan(1);

		y22();

		youtc(')');

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

			youts(n, "(Equal,");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '=');

			youtc(')');

		} else if (ys[0] == '!' && ys[1] == '=') {

			youts(n, "(Unequal,");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '!');

			youtc(')');

		} else if (ys[0] == '>' && ys[1] != '=' && ys[1] != '>') {

			youts(n, "(Greater,");

			do {

				youtc(',');

				yscan(1);

				y20();

			} while (ys[0] == '>' && ys[1] != '=' && ys[1] != '>');

			youtc(')');

		} else if (ys[0] == '>' && ys[1] == '=') {

			youts(n, "(GreaterEqual,");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '>' && ys[1] == '=');

			youtc(')');

		} else if (ys[0] == '<' && ys[1] != '=') {

			youts(n, "(Less,");

			do {

				youtc(',');

				yscan(1);

				y20();

			} while (ys[0] == '<' && ys[1] != '=');

			youtc(')');

		} else if (ys[0] == '<' && ys[1] == '=') {

			youts(n, "(LessEqual,");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '<' && ys[1] == '=');

			youtc(')');

		} else

			break;

}

/* expr + expr + expr */

y20()

{

	int n = yn;

	yy20();

	if (ys[0] == '+' && ys[1] != '+' && ys[1] != '=') {

		youts(n, "(Plus,");

		do {

			youtc(',');

			yscan(1);

			yy20();

		} while (ys[0] == '+' && ys[1] != '+' && ys[1] != '=');

		youtc(')');

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

		youts(n, "(Plus,");

		youts(yn, ",(Times,");

		y19();

		youts(yn, ",-1))");

	}

}

/* expr expr expr */

/* expr * expr * expr */

y19()

{

	int n = yn;

	y18();

	if (yisfactor()) {

		youts(n, "(Times,");

		do {

			if (*ys == '*')

				yscan(1);

			youtc(',');

			y18();

		} while (yisfactor());

		youtc(')');

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

		youts(n, "(Times,");

		youts(yn, ",(Power,");

		y17();

		youts(yn, ",-1))");

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

		youts(yn, "(Times,-1,");

		y16();

		youtc(')');

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

		youts(n, "(Power,");

		youtc(',');

		yscan(1);

		y16();

		youtc(')');

	}

}

/* expr . expr . expr */

y15()

{

	int n = yn;

	y14();

	if (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1])) {

		youts(n, "(Dot,");

		do {

			youtc(',');

			yscan(1);

			y14();

		} while (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1]));

		youtc(')');

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

		youts(n, "(NonCommutativeMultiply,");

		do {

			yscan(2);

			youtc(',');

			y13();

		} while (ys[0] == '*' && ys[1] == '*');

		youtc(')');

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

		youts(n, "(Factorial,");

		youtc(')');

	} else if (ys[0] == '!' && ys[1] == '!') {

		yscan(2);

		youts(n, "(Factorial2,");

		youtc(')');

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

		youts(n, "(Map,");

		youtc(',');

		y11();

		youtc(')');

	} else if (ys[0] == '/' && ys[1] == '/' && ys[2] == '@') {

		yscan(3);

		youts(n, "(MapAll,");

		youtc(',');

		y11();

		youtc(')');

	} else if (ys[0] == '@' && ys[1] == '@') {

		yscan(2);

		youts(n, "(Apply,");

		youtc(',');

		y11();

		youtc(')');

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

		youtc(')');

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

		youts(yn, "(PreIncrement,");

		y7();

		youtc(')');

	} else if (ys[0] == '-' && ys[1] == '-') {

		yscan(2);

		youts(yn, "(PreDecrement,");

		y7();

		youtc(')');

	} else {

		y7();

		if (ys[0] == '+' && ys[1] == '+') {

			yscan(2);

			youts(n, "(Increment,");

			youtc(')');

		} else if (ys[0] == '-' && ys[1] == '-') {

			yscan(2);

			youts(n, "(Decrement,");

			youtc(')');

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

			youtc(',');

			yscan(1);

			yexpr();

			while (*ys == ',') {

				youtc(',');

				yscan(1);

				yexpr();

			}

			if (ys[0] == ']' && ys[1] != ']')

				yscan(1);

			else

				yerr(3);	/* ] expected */

			youtc(')');

		} else if (ys[0] == '[' && ys[1] == '[') {

			youts(n, "(Part,");

			youtc(',');

			yscan(2);

			yexpr();

			while (*ys == ',') {

				youtc(',');

				yscan(1);

				yexpr();

			}

			if (ys[0] == ']' && ys[1] == ']')

				yscan(2);

			else

				yerr(4);	/* ]] expected */

			youtc(')');

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

		youts(n, "(PatternTest,");

		y5();

		youtc(')');

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

	if (*ys == '$' || isalpha(*ys)) {

		do

			youtc(*ys++);

		while (*ys == '$' || isalnum(*ys));

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

		yscan(0);

	} else if (ys[0] == '.' && isdigit(ys[1])) {

		ys++;

		youts(yn, "0.");

		while (isdigit(*ys))

			youtc(*ys++);

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

		youts(yn, "(List,");

		yexpr();

		while (*ys == ',') {

			yscan(1);

			youtc(',');

			yexpr();

		}

		if (*ys == '}')

			yscan(1);

		else

			yerr(6);	/* } expected */

		youtc(')');

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

	exit();

}

/*---------------------------------------------------------------------------*/

addnum(s)
char *s;
{
	char *t, u[20];
	double d, n;

	if (*s != '(')
		return;

	t = s;

	do {

		t++;

		addnum(t);

		t += arglen(t);

	} while (*t == ',');

	if (!equalhead(s + 1, "Plus"))
		return;

	n = 0.0;

	t = s + arglen(s + 1);

	do {

		t++;

		if (is_numeric(t)) {

			sscanf(t, "%lg", &d);

			n += d;

			delarg(t);

			t--;

		} else

			t += arglen(t);

	} while (*t == ',');

	if (n != 0.0) {
		sprintf(u, "%g", n);
		t = s + arglen(s + 1) + 1;
		insarg(t, u);
	}
}

mulnum(s)
char *s;
{
	char *t, u[20];
	double d, n;

	if (*s != '(')
		return;

	t = s;

	do {

		t++;

		mulnum(t);

		t += arglen(t);

	} while (*t == ',');

	if (!equalhead(s + 1, "Times"))
		return;

	n = 1.0;

	t = s + arglen(s + 1);

	do {

		t++;

		if (is_numeric(t)) {

			sscanf(t, "%lg", &d);

			n *= d;

			delarg(t);

			t--;

		} else

			t += arglen(t);

	} while (*t == ',');

	if (n == 0.0) {
		delarg(s);
		insarg(s, "0");
	} else if (n != 1.0) {
		sprintf(u, "%g", n);
		t = s + arglen(s + 1) + 1;
		insarg(t, u);
	}
}

/* convert (Plus) to 0 */

/* convert (Plus,5) to 5 */

deladd(s)

char *s;

{

	char *t;

	if (*s != '(')

		return;

	t = s;

	do {

		t++;

		deladd(t);

		t += arglen(t);

	} while (*t == ',');

	if (equalhead(s + 1, "Plus")) {

		switch (argcount(s)) {

		case 1:

			delarg(s);

			insarg(s, "1");

			break;

		case 2:

			delhead(s);

			break;

		}

	}

}

/* convert (Times) to 1 */

/* convert (Times,5) to 5 */

delmul(s)

char *s;

{

	char *t;

	if (*s != '(')

		return;

	t = s;

	do {

		t++;

		delmul(t);

		t += arglen(t);

	} while (*t == ',');

	if (equalhead(s + 1, "Times")) {

		switch (argcount(s)) {

		case 1:

			delarg(s);

			insarg(s, "0");

			break;

		case 2:

			delhead(s);

			break;

		}

	}

}
