/* Convert font files created by PXtoCH to C source code.
Pixels are encoded serially across multiple bytes. */

#include <stdio.h>

#include <string.h>

char str[256];

unsigned char bitmap[30000];

int index;

struct {

	int index;

	int height;

	int width;

	int right;

	int left;

	int depth;

	int x_offset;

	int y_offset;

	int raster_width;

	int internal_width;

	int right_kerning;

	int top;

	int bottom;

	int data_row_count;

} metric[128];

char infile[128];

char outfile[128];

FILE *f;

main(argc, argv)

int argc;

char *argv[];

{

	strcpy(infile, argv[1]);

	strcat(infile, ".chr");

	strcpy(outfile, argv[1]);

	strcat(outfile, ".h");

	input();

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

	n = get_number_of_char();

	for (i = 0; i < n; i++) {

		c = get_info();

		get_bits(c);

	}

	fclose(f);

}

output(s)

char *s;

{

	int i, n = 0;

	int h, d, w;

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

		h = metric[i].y_offset + 1;

		d = metric[i].depth - 1;

		w = metric[i].internal_width;

		fprintf(f, "/* chrcode = %o */ %d, %d, %d, %d,\n", i, metric[i].index, h, d, w);

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

get_number_of_char()

{

	int n;

	fgets(str, 256, f);

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &n);

	return n;

}

get_info()

{

	int c, i;

	/* go to page break */

	for (;;) {

		c = fgetc(f);

		if (c == EOF) {

			printf("unexpected end of file\n");

			exit(1);

		}

		if (c == '\f')

			break;

	}

/* chrcode (octal) */

	fgets(str, 256, f);

	sscanf(str + 16, "%o", &i);

	printf("%d ", i);

/* height */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].height);

/* width */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].width);

/* right */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].right);

/* left */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].left);

/* depth */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].depth);

/* x_offset */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].x_offset);

/* y_offset */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].y_offset);

/* tfm width (ignore) */

	fgets(str, 256, f);

/* raster width */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].raster_width);

/* internal width */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].internal_width);

/* right kerning */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].right_kerning);

/* top */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].top);

/* bottom */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].bottom);

/* data row count */

	fgets(str, 256, f);

	sscanf(str + 16, "%d", &metric[i].data_row_count);

/* return chrcode */

	return i;

}

get_bits(c)

int c;

{

	int a, b, i, j, m, n;

	char *s;

	metric[c].index = index;

	m = 1;

	for (i = 0; i < metric[c].data_row_count; i++) {

		for (;;) {

			fgets(str, 256, f);

			n = strlen(str);

			for (a = 0; a < n; a++)

				if (str[a] == '.' || str[a] == '*')

					break;

			for (b = a; b < n; b++)

				if (str[b] != '.' && str[b] != '*')

					break;

			if (a < b)

				break;

		}

		if (b - a != metric[c].internal_width) {

			printf("bad width\n");

			exit(1);

		}

		for (j = a; j < b; j++) {

			if (str[j] == '*')

				bitmap[index] |= m;

			m <<= 1;

			if (m == 0x100) {

				m = 1;

				index++;

			}

		}

	}

	if (m > 1)

		index++;

}
