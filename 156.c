/* Normalize a TEX file. */

#include <stdio.h>
#include <ctype.h>

#define MAX 65

char s[1000];

int control_word;

main()

{

	int flag = 0, m = 0, n;

	char *space;

	for (;;) {

		n = getword();

		if (n == 0)

			break;

		if (flag && !isalpha(*s))

			space = "";

		else

			space = " ";

		if (m == 0) {

			printf("%s", s);

			m = n;

		} else if (m + n + strlen(space) > MAX) {

			printf("\n%s", s);

			m = n;

		} else {

			printf("%s%s", space, s);

			m = m + n + strlen(space);

		}

		flag = control_word;

	}

	printf("\n");

}

getword()

{

	int c, i;

	control_word = 0;

	/* skip leading spaces */

	while (isspace(c = fgetc(stdin)));

	if (c == EOF)

		return 0;

	*s = c;

	for (i = 1; i < 1000; i++) {

		c = fgetc(stdin);

		if (c == EOF || isspace(c))

			break;

		s[i] = c;

		if (s[i - 1] == '\\' && isalpha(c))

			control_word = 1;

		else if (control_word && !isalpha(c)) {

			ungetc(c, stdin);

			break;

		}

	}

	s[i] = 0;

	return i;

}
