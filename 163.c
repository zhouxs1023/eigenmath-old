/* Scan a \TeX\ file and print text words. */

#include <stdio.h>

#include <ctype.h>

#include <alloc.h>

#define MAX 100

char buf[MAX + 1];

main()

{

	int c;

	while (get_word())

		;

}

get_word()

{

	int c, i;

	do

		c = fgetc(stdin);

	while (isspace(c));

	if (c == EOF)

		return 0;

	else if (c == '\\')

		tex_cmd();

	else if (isalpha(c)) {

		buf[0] = tolower(c);

		for (i = 1; i < MAX; i++) {

			c = fgetc(stdin);

			if (isalpha(c))

				buf[i] = tolower(c);

			else

				break;

		}

		buf[i] = 0;

		printf("%s\n", buf);

	}

	return 1;

}

tex_cmd()

{

	int c;

	c = fgetc(stdin);

	if (isalpha(c))

		for (;;) {

			c = fgetc(stdin);

			if (isalpha(c))

				continue;

			else

				break;

		}

}
