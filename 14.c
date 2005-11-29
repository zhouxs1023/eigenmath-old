/*  Scan all C source files in the current directory and print
the function names and arguments. */

#include <stdio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

char str[256];

main()
{
	int n;
	struct ffblk b;

	n = findfirst("*.c", &b, 0);

	while (n == 0) {
		process(b.ff_name);
		n = findnext(&b);
	}
}

process(filename)
char *filename;
{
	int i, n, flag = 0;
	FILE *f;

	printf("%s\n", filename);

	f = fopen(filename, "r");

	for (;;) {

		if (fgets(str, 256, f) == NULL)
			break;

		if (*str == '{')
			flag = 1;

		if (flag == 0)
			printf("%s", str);

		if (*str == '}')
			flag = 0;
	}

	fclose(f);

	printf("\n");
}
