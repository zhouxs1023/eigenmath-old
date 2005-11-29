/* Convert a file to lower case letters. */

#include <stdio.h>

#include <ctype.h>

main()

{

	int c;

	while ((c = fgetc(stdin)) != EOF)

		printf("%c", tolower(c));

}
