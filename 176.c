/* Search for dot-m files and print usage statements. */

#include <stdio.h>

#include <ctype.h>

#include <dir.h>

#include <dos.h>

#include <stdlib.h>

#include <string.h>

char buf[1000];

main()

{

	int n;

	struct ffblk b;

	n = findfirst("*.m", &b, 0);

	while (n == 0) {

		process(b.ff_name);

		n = findnext(&b);

	}

}

process(s)

char *s;

{

	int i, n;

	FILE *f;

/*	fprintf(stderr, "%s\n", s); */

	f = fopen(s, "r");

	for (;;) {

		if (fgets(buf, 1000, f) == NULL)

			break;

		n = strlen(buf);

		/* change line feed to space */

		buf[n - 1] = ' ';

		for (i = 0; i < n - 7; i++)

			if (strncmp(buf + i, "::usage", 7) == 0)

				break;

		if (i < n - 7) {

			printf("%s", buf);

			for (;;) {

				if (fgets(buf, 1000, f) == NULL)

					break;

				if (isblank(buf))

					break;

				printf("%s", buf);

			}

			printf("\n");

		}

	}

	fclose(f);

}

isblank(s)

char *s;

{

	int i, n;

	n = strlen(s);

	/* change line feed to space */

	buf[n - 1] = ' ';

	for (i = 0; i < n; i++)

		if (isspace(s[i]))

			continue;

		else

			return 0;

	return 1;

}
