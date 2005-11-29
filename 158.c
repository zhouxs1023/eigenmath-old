/* Display DVI file opcodes. */

#include <stdio.h>

char filename[100];

FILE *file;

char buf[1000];

int f;

long c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, p;

long h, v, w, x, y, z;

#define MAX 10

struct {

	long h, v, w, x, y, z;

} stack[MAX];

int sp = -1;

main(argc, argv)

int argc;

char *argv[];

{

	int c;

	strcpy(filename, argv[1]);

	strcat(filename, ".dvi");

	file = fopen(filename, "rb");

	for (;;) {

		c = fgetc(file);

		if (c < 128)

			set_char(c);

		else if (c >= 171 && c <= 234)

			fnt_num(c);

		else

			dvi_cmd(c);

	}

}

fget1()

{

	return fgetc(file);

}

long fget2()

{

	long c1, c2;

	c1 = fgetc(file);

	c2 = fgetc(file);

	if (c1 & 0x80)

		return 0xffff0000 | c1 << 8 | c2;

	else

		return c1 << 8 | c2;

}

long fget3()

{

	long c1, c2, c3;

	c1 = fgetc(file);

	c2 = fgetc(file);

	c3 = fgetc(file);

	if (c1 & 0x80)

		return 0xff000000 | c1 << 16 | c2 << 8 | c3;

	else

		return c1 << 16 | c2 << 8 | c3;

}

long fget4()

{

	long c1, c2, c3, c4;

	c1 = fgetc(file);

	c2 = fgetc(file);

	c3 = fgetc(file);

	c4 = fgetc(file);

	return c1 << 24 | c2 << 16 | c3 << 8 | c4;

}

dvi_cmd(c)

int c;

{

	switch (c) {

	case 139:

		bop();

		break;

	case 140:

		eop();

		break;

	case 141:

		push();

		break;

	case 142:

		pop();

		break;

	case 143:

		right1();

		break;

	case 144:

		right2();

		break;

	case 145:

		right3();

		break;

	case 146:

		right4();

		break;

	case 147:

		w0();

		break;

	case 148:

		w1();

		break;

	case 149:

		w2();

		break;

	case 150:

		w3();

		break;

	case 151:

		w4();

		break;

	case 152:

		x0();

		break;

	case 153:

		x1();

		break;

	case 154:

		x2();

		break;

	case 155:

		x3();

		break;

	case 156:

		x4();

		break;

	case 157:

		down1();

		break;

	case 158:

		down2();

		break;

	case 159:

		down3();

		break;

	case 160:

		down4();

		break;

	case 161:

		y0();

		break;

	case 162:

		y1();

		break;

	case 163:

		y2();

		break;

	case 164:

		y3();

		break;

	case 165:

		y4();

		break;

	case 243:

		fnt_def1();

		break;

	case 247:

		pre();

		break;

	case 248:

		post();

		break;

	default:

		printf("%d ", c);

		error("unknown opcode");

		break;

	}

}

error(s)

char *s;

{

	printf("%s\n", s);

	exit();

}

/* 0--127 */

set_char(c)

int c;

{

	printf("set_char_%d\n", c);

}

fnt_def1()

{

	int i, k, a, l;

	long c, s, d;

	k = fget1();

	c = fget4();

	s = fget4();

	d = fget4();

	a = fget1();

	l = fget1();

	for (i = 0; i < a + l; i++)

		buf[i] = fget1();

	buf[i] = 0;

	printf("fnt_def1 %d %ld %ld %ld %d %d %s\n", k, c, s, d, a, l, buf);

}

/* 139 */

bop()

{

	c0 = fget4();

	c1 = fget4();

	c2 = fget4();

	c3 = fget4();

	c4 = fget4();

	c5 = fget4();

	c6 = fget4();

	c7 = fget4();

	c8 = fget4();

	c9 = fget4();

	p = fget4();

	printf("bop %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n", c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, p);

	sp = -1;

	h = 0;

	v = 0;

	w = 0;

	x = 0;

	y = 0;

	z = 0;

}

/* 140 */

eop()

{

	printf("eop\n");

}

/* 141 */

push()

{

	printf("push\n");

	sp++;

	stack[sp].h = h;

	stack[sp].v = v;

	stack[sp].w = w;

	stack[sp].x = x;

	stack[sp].y = y;

	stack[sp].z = z;

	h = 0;

	v = 0;

	w = 0;

	x = 0;

	y = 0;

	z = 0;

}

/* 142 */

pop()

{

	printf("pop\n");

	if (sp < 0)

		error("stack underflow");

	h = stack[sp].h;

	v = stack[sp].v;

	w = stack[sp].w;

	x = stack[sp].x;

	y = stack[sp].y;

	z = stack[sp].z;

	sp--;

}

/* 143 */

right1()

{

	long b;

	b = fget1();

	printf("right1 %ld\n", b);

	h += b;

}

/* 144 */

right2()

{

	long b;

	b = fget2();

	printf("right2 %ld\n", b);

	h += b;

}

/* 145 */

right3()

{

	long b;

	b = fget3();

	printf("right3 %ld\n", b);

	h += b;

}

/* 146 */

right4()

{

	long b;

	b = fget4();

	printf("right4 %ld\n", b);

	h += b;

}

/* 147 */

w0()

{

	printf("w0\n");

	h += w;

}

/* 148 */

w1()

{

	w = fget1();

	printf("w1 %ld\n", w);

	h += w;

}

/* 149 */

w2()

{

	w = fget2();

	printf("w2 %ld\n", w);

	h += w;

}

/* 150 */

w3()

{

	w = fget3();

	printf("w3 %ld\n", w);

	h += w;

}

/* 151 */

w4()

{

	w = fget4();

	printf("w4 %ld\n", w);

	h += w;

}

/* 152 */

x0()

{

	printf("x0\n");

	h += x;

}

/* 153 */

x1()

{

	x = fget1();

	printf("x1 %ld\n", x);

	h += x;

}

/* 154 */

x2()

{

	x = fget2();

	printf("x2 %ld\n", x);

	h += x;

}

/* 155 */

x3()

{

	x = fget3();

	printf("x3 %ld\n", x);

	h += x;

}

/* 156 */

x4()

{

	x = fget4();

	printf("x4 %ld\n", x);

	h += x;

}

/* 157 */

down1()

{

	long a;

	a = fget1();

	printf("down1 %ld\n", a);

	v += a;

}

/* 158 */

down2()

{

	long a;

	a = fget2();

	printf("down2 %ld\n", a);

	v += a;

}

/* 159 */

down3()

{

	long a;

	a = fget3();

	printf("down3 %ld\n", a);

	v += a;

}

/* 160 */

down4()

{

	long a;

	a = fget4();

	printf("down4 %ld\n", a);

	v += a;

}

/* 161 */

y0()

{

	printf("y0\n");

	v += y;

}

/* 162 */

y1()

{

	y = fget1();

	printf("y1 %ld\n", y);

	v += y;

}

/* 163 */

y2()

{

	y = fget2();

	printf("y2 %ld\n", y);

	v += y;

}

/* 164 */

y3()

{

	y = fget3();

	printf("y3 %ld\n", y);

	v += y;

}

/* 165 */

y4()

{

	y = fget4();

	printf("y4 %ld\n", y);

	v += y;

}

/* 171--234 */

fnt_num(c)

int c;

{

	f = c - 171;

	printf("fnt_num_%d\n", f);

}

pre()

{

	int i, j, k;

	long num, den, mag;

	i = fget1();

	num = fget4();

	den = fget4();

	mag = fget4();

	k = fget1();

	for (j = 0; j < k; j++)

		buf[j] = fget1();

	buf[j] = 0;

	printf("pre %d %ld %ld %ld %d %s\n", i, num, den, mag, k, buf);

}

/* 248 */

post()

{

	printf("post\n");

	exit();

}
