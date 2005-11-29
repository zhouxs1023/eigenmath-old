/* Scans the Math Source catalog and prints titles. */

#include <stdio.h>

#include <ctype.h>

char s[1000];

main()

{

	for (;;) {

		if (fgets(s, 1000, stdin) == NULL)

			exit();

		if (isdigit(*s))

			printf("%s", s);

	}

}
