/* Convert ASCII font files from the Macintosh into bit map and font metric
look-up tables in 8086 assembly language.
Assembly language is used because the tables are too big to be compiled
with Turbo C. */

/* makefont.c

convert ascii bit map fonts to assembler source files

to compile this program type

	C>tcc -ml makefont

to create a font file, type

	C>makefont geneva14

	C>tcc -c -B geneva14.asm

font metric

	offset		bitmap offset

	dy		vertical display offset

	w		width in pixels

lookup

	[	]

	[   a   ] <--- bitmap + metric[n].offset

	[   b	]

	[   c	]

	[   d	]

	[   e	]

	[   f	]

	[   g	]

	[   h	]

	[   i	]

	[   j	]

	[   k	]

	[   l	]

	[	] <--- bitmap + metric[n + 1].offset

display

	[   a   ][   b   ][   c   ]

	[   d   ][   e   ][   f   ]

	[   g   ][   h   ][   i   ]

	[   j   ][   k   ][   l   ]

	upper left pixel value = a & 1

	upper left pixel screen coordinate = dy

	number of columns = (w + 7) / 8

*/

#include <stdio.h>

#include <alloc.h>

#include <string.h>

#define MAX 65000

char str[256];

unsigned long bitmap[256][32];

unsigned char *bigbitmap;

unsigned int offset;

struct {

	unsigned int offset;

	unsigned char dy, w;

} metric[256];

char infile[128];

char outfile[128];

main(argc, argv)

int argc;

char *argv[];

{

	if (argc != 2)

		exit();

	bigbitmap = malloc(MAX);

	if (bigbitmap == NULL) {

		printf("out of memory\n");

		exit();

	}

	strcpy(infile, argv[1]);

	strcat(infile, ".mac");

	strcpy(outfile, argv[1]);

	strcat(outfile, ".asm");

	input();

	measure();

	fixup();

	output(argv[1]);

	printf("%u byte bitmap\n", offset);

}

input()

{

	int i, j;

	FILE *f;

	printf("reading %s\n", infile);

	f = fopen(infile, "r");

	if (f == NULL) {

		printf("error opening file\n");

		exit();

	}

	while (!feof(f))

		input2(f);

	fclose(f);

}

input2(f)

FILE *f;

{

	int i, j, n;

	unsigned long m;

	char *s;

	/* get symbol number */

	while (*str != '#')

		if (fgets(str, 256, f) == NULL)

			return;

	n = atoi(str + 1);

	for (i = 0; i < 32; i++) {

		if (fgets(str, 256, f) == NULL)

			return;

		if (*str == '#')

			return;

		m = 1;

		s = str;

		while (m && *s) {

			if (*s++ == '*')

				bitmap[n][i] |= m;

			m <<= 1;

		}

	}

}

measure()

{

	int i, j, t, b, w;

	for (i = 0; i < 256; i++) {

		metric[i].offset = offset;

		w = metric[i].w = width(i);

		if (w) {

			t = metric[i].dy = top(i);

			b = bottom(i);

			for (j = t; j <= b; j++) {

				if (w > 0) {

					check_offset();

					bigbitmap[offset++] = bitmap[i][j];

				}

				if (w > 8) {

					check_offset();

					bigbitmap[offset++] = bitmap[i][j] >> 8;

				}

				if (w > 16) {

					check_offset();

					bigbitmap[offset++] = bitmap[i][j] >> 16;

				}

				if (w > 24) {

					check_offset();

					bigbitmap[offset++] = bitmap[i][j] >> 24;

				}

			}

		}

	}

}

check_offset()

{

	if (offset >= MAX) {

		printf("bit map overflow\n");

		exit();

	}

}

top(n)

int n;

{

	int i;

	for (i = 0; i <= 32; i++)

		if (bitmap[n][i])

			break;

	return i;

}

bottom(n)

int n;

{

	int i;

	for (i = 31; i > -1; i--)

		if (bitmap[n][i])

			break;

	return i;

}

width(n)

int n;

{

	int i;

	unsigned long m, b;

	b = 0;

	for (i = 0; i < 32; i++)

		if (bitmap[n][i] > b)

			b = bitmap[n][i];

	m = 0x80000000;

	for (i = 32; i > 0; i--)

		if (b & m)

			break;

		else

			m >>= 1;

	return i;

}

output(s)

char *s;

{

	int i;

	FILE *f;

	printf("writing %s\n", outfile);

	f = fopen(outfile, "w");

	if (f == NULL) {

		printf("error opening file\n");

		exit();

	}

	strupr(s);

	fprintf(f, "%s_TEXT segment byte public 'CODE'\n", s);

	strlwr(s);

	fprintf(f, "_%s_metric:\n", s);

	fprintf(f, " public _%s_metric\n", s);

	for (i = 0; i < 256; i++) {

		if (i > 0x20 && i < 0x7f)

			fprintf(f, "; %c\n", i);

		else

			fprintf(f, "; 0x%02x\n", i);

		fprintf(f, " dw %u\n", metric[i].offset);

		fprintf(f, " db %u\n", metric[i].dy);

		fprintf(f, " db %u\n", metric[i].w);

	}

	fprintf(f, "_%s_bitmap:\n", s);

	fprintf(f, " public _%s_bitmap\n", s);

	for (i = 0; i < offset; i++)

		fprintf(f, " db %u\n", bigbitmap[i]);

	strupr(s);

	fprintf(f, "%s_TEXT ends\n", s);

	fprintf(f, " end\n");

	fclose(f);

}

/* convert dy to height above base line */

/* dy is number of pixels below top of bitmap */

fixup()

{

	int h, i, n, w;

	/* number of pixel bytes */

	n = metric['E' + 1].offset - metric['E'].offset;

	/* width */

	w = (metric['E'].w + 7) / 8;

	/* distance from top to baseline */

	h = n / w + metric['E'].dy;

	for (i = 0; i < 256; i++)

		metric[i].dy = h - metric[i].dy;

}

