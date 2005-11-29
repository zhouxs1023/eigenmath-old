/* Remove blank lines. */

#include <stdio.h>

char s[1000];

main()

{

	while (fgets(s, 1000, stdin)) {

		if (*s == '\n')

			;

		else

			fprintf(stdout, "%s", s);

	}

}
