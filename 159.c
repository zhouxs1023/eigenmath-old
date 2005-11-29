/* Convert font files created by PXtoCH to binary. */

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

	long tfm_width;

	int raster_width;

	int internal_width;

	int right_kerning;

	int top;

	int bottom;

	int data_row_count;

} metric[128];

struct {

	int index;

	char x_offset;

	char y_offset;

	char ncol;

	char nrow;

	long w;

} small_metric[128];

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

	strcat(outfile, ".bin");

	input();

	output();

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

output()

{

	int i;

	FILE *f;

	printf("writing %s\n", outfile);

	f = fopen(outfile, "wb");

	if (f == NULL) {

		printf("error opening file\n");

		exit(1);

	}

	/* metric */

	for (i = 0; i < 128; i++) {

		small_metric[i].index = metric[i].index;

		small_metric[i].x_offset = metric[i].x_offset;

		small_metric[i].y_offset = metric[i].y_offset;

		small_metric[i].ncol = (metric[i].internal_width + 7) / 8;

		small_metric[i].nrow = metric[i].data_row_count;

		small_metric[i].w = metric[i].tfm_width;

	}

	fwrite(small_metric, 10, 128, f);

	/* bit map */

	fwrite(bitmap, 1, index, f);

	fclose(f);

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

/* tfm width */

	fgets(str, 256, f);

	sscanf(str + 16, "%ld", &metric[i].tfm_width);

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
