/*  Run PKtoPX and PXtoCH for all dot-PK files in the current directory. */

#include <stdio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

char str[256];

main()
{
	int n;
	char *s;
	struct ffblk b;

	n = findfirst("*.pk", &b, 0);

	while (n == 0) {

		if (kbhit())
			break;

		s = b.ff_name;

		while (*s != '.')
			s++;

		*s = 0;

		sprintf(str, "pktopx %s %s", b.ff_name, b.ff_name);

		printf("%s\n", str);

		system(str);

		sprintf(str, "pxtoch %s %s", b.ff_name, b.ff_name);

		printf("%s\n", str);

		system(str);

		n = findnext(&b);
	}
}
