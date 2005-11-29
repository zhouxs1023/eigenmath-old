/* Convert Macintosh font files to C source code.
Pixels are encoded serially across multiple bytes. */

#include <stdio.h>

#include <string.h>

char str[256];

unsigned char bitmap[10000];

int index;

struct {

	int index;

	int y, nrow;

	int h, d, w;

} metric[128];

char infile[128];

char outfile[128];

FILE *f;

main(argc, argv)

int argc;

char *argv[];

{

	strcpy(infile, argv[1]);

	strcat(infile, ".mac");

	strcpy(outfile, argv[1]);

	strcat(outfile, ".h");

	input();

	fixup();

	output(argv[1]);

	printf("%u byte bitmap\n", index);

}

input()

{

	int c, i, n;

	printf("reading %s\n", infile);

	f = fopen(infile, "r");

	if (f == NULL) {

		printf("error opening file\n");

		exit(1);

	}

	for (;;) {

		if (*str == '#')

			get_bits();

		else if (fgets(str, 256, f) == NULL)

			break;

	}

	fclose(f);

	printf("\n");

}

output(s)

char *s;

{

	int i, n = 0;

	FILE *f;

	printf("writing %s\n", outfile);

	f = fopen(outfile, "w");

	if (f == NULL) {

		printf("error opening file\n");

		exit(1);

	}

	/* metric */

	fprintf(f, "struct { int index; char h, d, w; } %s_metric[128] = {\n", s);

	for (i = 0; i < 128; i++) {

		if (i > ' ' && i < 127)

			fprintf(f, "/* %c */ ", i);

		else

			fprintf(f, "/*   */ ");

		fprintf(f, "%d, %d, %d, %d,\n", metric[i].index, metric[i].h, metric[i].d, metric[i].w);

	}

	fprintf(f, "};\n");

	/* bit map */

	fprintf(f, "char %s_bitmap[%d] = {\n", s, index);

	for (i = 0; i < index; i++) {

		fprintf(f, "%d,", bitmap[i]);

		if (++n == 16) {

			fprintf(f, "\n");

			n = 0;

		}

	}

	fprintf(f, "\n};\n");

}

get_bits()

{

	int c, i, m, n, w, x;

	sscanf(str + 1, "%d", &c);

	if (fgets(str, 256, f) == NULL) {

		*str = 0;

		return;

	}

	if (*str != '.' && *str != '*')

		return;

	if (c > 127)

		return;

	printf("%d ", c);

	w = strlen(str) - 1;

	/* get first row */

	metric[c].y = 0;

	for (;;) {

		for (i = 0; i < w; i++)

			if (str[i] == '*')

				break;

		if (i < w)

			break;

		metric[c].y++;

		if (fgets(str, 256, f) == NULL)

			return;

		if (*str != '.' && *str != '*')

			return;

	}

	/* get subsequent rows */

	m = 1;

	n = 0;

	x = index;

	for (;;) {

		n++;

		for (i = 0; i < w; i++) {

			if (str[i] == '*') {

				bitmap[x] |= m;

				metric[c].nrow = n;

			}

			m <<= 1;

			if (m == 0x100) {

				m = 1;

				x++;

				if (x > 9999) {

					printf("bit map overflow\n");

					exit(1);

				}

			}

		}

		if (fgets(str, 256, f) == NULL)

			break;

		if (*str != '.' && *str != '*')

			break;

	}

	metric[c].w = w;

	metric[c].index = index;

	index += (metric[c].nrow * metric[c].w + 7) / 8;

}

fixup()

{

	int b, i;

	b = metric['E'].y + metric['E'].nrow;

	for (i = 0; i < 128; i++)

		if (metric[i].w) {

			metric[i].h = b - metric[i].y;

			metric[i].d = metric[i].y + metric[i].nrow - b;

		}

}
