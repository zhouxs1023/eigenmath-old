/* Double space a file. */

#include <stdio.h>

char s[1000];

main(argc, argv)

int argc;

char *argv[];

{

	int i, n;

	FILE *f;

	f = fopen(argv[1], "r");

	while (fgets(s, 1000, f)) {

		if (isblank(s))

			continue;

		printf("%s\n", s);

	}

}

isblank(s)

char *s;

{

	int i, n;

	n = strlen(s);

	for (i = 0; i < n; i++)

		if (s[i] > ' ')

			return 0;

	return 1;

}
