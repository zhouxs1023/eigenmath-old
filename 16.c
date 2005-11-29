/* print a listing of all "C" files */

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
	FILE *f;

	printf("\fFILE NAME: %s\n", filename);

	f = fopen(filename, "r");

	for (;;) {

		if (fgets(str, 256, f) == NULL)
			break;

		printf("%s", str);
	}

	fclose(f);
}
