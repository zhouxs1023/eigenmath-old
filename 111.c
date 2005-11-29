/* Remove header lines from Math Source text. */

#include <stdio.h>

char s[1000];

main()

{

	for (;;) {

		/* skip lines until marker found */

		for (;;) {

			if (fgets(s, 1000, stdin) == NULL)

				exit();

			if (strncmp(s, "##!", 3) == 0)

				break;

		}

		/* print lines until marker found */

		for (;;) {

			if (fgets(s, 1000, stdin) == NULL)

				exit();

			if (strncmp(s, "##!", 3) == 0)

				break;

			printf("%s", s);

		}

	}

}
