/* Experimental Mathematica parser. */

#include <stdio.h>

#include <ctype.h>

#include <alloc.h>

extern char *sderiv();

char s1[1000], s2[1000];

main()

{
	int n = 1;

	for (;;) {

		printf("\nIn[%d]: ", n);

		gets(s2);

		inputformtofullform(s1, s2);

		printf("%s\n", s1);

		test(s1);

		printf("Out[%d]: %s\n", n, s1);

		n++;

	}

}

char *alloc(n)

int n;

{

	char *p;

	p = malloc(n);

	if (p == NULL) {

		printf("out of memory\n");

		exit();

	}

	return p;

}

test(s)

char *s;

{

	if (equalhead(s, "Times"))

		flatten(s + 5, -1, "Times");

}

flatten(s, n, head)

char *s, *head;

int n;

{

	char *t = s;

	/* check level */

	if (n == 0)

		return;

	/* scan head */

	while (isalnum(*t))

		t++;

	if (*t == '[') {

		flattenarg(t + 1, n, head);

		if (equalhead(s, head))

			removehead(s);

	}

}

flattenarg(s, n, head)

char *s, *head;

int n;

{

	char *t;

	t = s + arglen(s);

	if (*t == ',')

		flattenarg(t + 1, n, head);

	flatten(s, n - 1, head);

}

equalhead(s, head)

char *s, *head;

{

	while (*head)

		if (*s++ != *head++)

			return 0;

	if (*s == '[')

		return 1;

	else

		return 0;

}

/* convert head [ expr ] to expr */ 

removehead(s)

char *s;

{
	int n = 0;

	char *t = s;

	/* scan head */

	while (*t++ != '[')

		;

	/* shift left */

	for (;;) {

		if (*t == '[')

			n++;

		else if (*t == ']')

			if (n == 0)

				break;

			else

				n--;

		*s++ = *t++;

	}

	t++;	/* skip closing ] */

	while (*t)

		*s++ = *t++;

	*s = '\0';

}

/* s points to the first argument */

rotateargs(s)

char *s;

{
	int i, n, len;

	char *arg, *t, *alloc();

	static char argbuf[100];

	len = arglen(s);

	if (len > 100)

		arg = alloc(len);

	else

		arg = argbuf;

	for (i = 0; i < len; i++)

		arg[i] = s[i];

	t = s + len;

	while (*t == ',') {

		t++;

		n = arglen(t);

		for (i = 0; i < n; i++)

			*s++ = *t++;

		*s++ = ',';

	}

	for (i = 0; i < len; i++)

		s[i] = arg[i];

	if (len > 100)

		free(arg);

}

/* s points to start of argument */

arglen(s)

char *s;

{

	int len = 0, n = 0;

	for (;;) {

		switch (*s++) {

		case '\0':

		case ',':

			return len;

		case '[':

			n++;

			break;

		case ']':

			if (n == 0)

				return len;

			else

				n--;

			break;

		default:

			break;

		}

		len++;

	}

}

/* s points to the first argument */

argcount(s)

char *s;

{

	int n;

	if (*s == '\0' || *s == ']')

		return 0;

	s += arglen(s);

	n = 1;

	while (*s == ',') {

		s++;

		s += arglen(s);

		n++;

	}

	return n;

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

		youts(n, "Or[");

		do {

			youtc(',');

			yscan(2);

			y23();

		} while (ys[0] == '|' && ys[1] == '|');

		youtc(']');

	}

}

/* expr && expr && expr */

y23()

{

	int n = yn;

	y22();

	if (ys[0] == '&' && ys[1] == '&') {

		youts(n, "And[");

		do {

			youtc(',');

			yscan(2);

			y22();

		} while (ys[0] == '&' && ys[1] == '&');

		youtc(']');

	}

}

/* ! expr */

y22()

{

	if (*ys == '!') {

		youts(yn, "Not[");

		yscan(1);

		y22();

		youtc(']');

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

			youts(n, "Equal[");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '=');

			youtc(']');

		} else if (ys[0] == '!' && ys[1] == '=') {

			youts(n, "Unequal[");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '=' && ys[1] == '!');

			youtc(']');

		} else if (ys[0] == '>' && ys[1] != '=' && ys[1] != '>') {

			youts(n, "Greater[");

			do {

				youtc(',');

				yscan(1);

				y20();

			} while (ys[0] == '>' && ys[1] != '=' && ys[1] != '>');

			youtc(']');

		} else if (ys[0] == '>' && ys[1] == '=') {

			youts(n, "GreaterEqual[");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '>' && ys[1] == '=');

			youtc(']');

		} else if (ys[0] == '<' && ys[1] != '=') {

			youts(n, "Less[");

			do {

				youtc(',');

				yscan(1);

				y20();

			} while (ys[0] == '<' && ys[1] != '=');

			youtc(']');

		} else if (ys[0] == '<' && ys[1] == '=') {

			youts(n, "LessEqual[");

			do {

				youtc(',');

				yscan(2);

				y20();

			} while (ys[0] == '<' && ys[1] == '=');

			youtc(']');

		} else

			break;

}

/* expr + expr + expr */

y20()

{

	int n = yn;

	yy20();

	if (ys[0] == '+' && ys[1] != '+' && ys[1] != '=') {

		youts(n, "Plus[");

		do {

			youtc(',');

			yscan(1);

			yy20();

		} while (ys[0] == '+' && ys[1] != '+' && ys[1] != '=');

		youtc(']');

	}

}

/* expr - expr */

/* ( expr - expr ) - expr */

yy20()

{

	int n = yn;

	y19();

	while (ys[0] == '-' && ys[1] != '-' && ys[1] != '=') {

		youts(n, "Subtract[");

		youtc(',');

		yscan(1);

		y19();

		youtc(']');

	}

}

/* expr expr expr */

/* expr * expr * expr */

y19()

{

	int n = yn;

	y18();

	if (yisfactor()) {

		youts(n, "Times[");

		do {

			if (*ys == '*')

				yscan(1);

			youtc(',');

			y18();

		} while (yisfactor());

		youtc(']');

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

		youts(n, "Divide[");

		youtc(',');

		y17();

		youtc(']');

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

		youts(yn, "Times[-1,");

		y16();

		youtc(']');

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

		youts(n, "Power[");

		youtc(',');

		yscan(1);

		y16();

		youtc(']');

	}

}

/* expr . expr . expr */

y15()

{

	int n = yn;

	y14();

	if (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1])) {

		youts(n, "Dot[");

		do {

			youtc(',');

			yscan(1);

			y14();

		} while (ys[0] == '.' && ys[1] != '.' && !isdigit(ys[1]));

		youtc(']');

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

		youts(n, "NonCommutativeMultiply[");

		do {

			yscan(2);

			youtc(',');

			y13();

		} while (ys[0] == '*' && ys[1] == '*');

		youtc(']');

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

		youts(n, "Factorial[");

		youtc(']');

	} else if (ys[0] == '!' && ys[1] == '!') {

		yscan(2);

		youts(n, "Factorial2[");

		youtc(']');

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

		youts(n, "Map[");

		youtc(',');

		y11();

		youtc(']');

	} else if (ys[0] == '/' && ys[1] == '/' && ys[2] == '@') {

		yscan(3);

		youts(n, "MapAll[");

		youtc(',');

		y11();

		youtc(']');

	} else if (ys[0] == '@' && ys[1] == '@') {

		yscan(2);

		youts(n, "Apply[");

		youtc(',');

		y11();

		youtc(']');

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

		youtc(']');

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

		youts(yn, "PreIncrement[");

		y7();

		youtc(']');

	} else if (ys[0] == '-' && ys[1] == '-') {

		yscan(2);

		youts(yn, "PreDecrement[");

		y7();

		youtc(']');

	} else {

		y7();

		if (ys[0] == '+' && ys[1] == '+') {

			yscan(2);

			youts(n, "Increment[");

			youtc(']');

		} else if (ys[0] == '-' && ys[1] == '-') {

			yscan(2);

			youts(n, "Decrement[");

			youtc(']');

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

			youtc('[');

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

			youtc(']');

		} else if (ys[0] == '[' && ys[1] == '[') {

			youts(n, "Part[");

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

			youtc(']');

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

		youts(n, "PatternTest[");

		y5();

		youtc(']');

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

		youts(yn, "List[");

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

		youtc(']');

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
