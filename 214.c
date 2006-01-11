// Convert TXT to TeX.

#include <stdio.h>

main(int argc, char *argv[])
{
	int c;
	FILE *f;
	f = fopen(argv[1], "r");
	printf("\\tt\n");
	printf("\\nopagenumbers\n");
	printf("\\obeylines\n");
	printf("\\obeyspaces\n");
	while (1) {
		c = fgetc(f);
		if (c < 0)
			break;
		if (c == '\n')
			printf("\n");
		else if (c == ' ')
			printf("\\ ");
		else
			printf("\\char%d", c);
	}
	printf("\\vfill\\end\n");
}
