/* convert linefeed to newline */

#include <stdio.h>

main()
{
	int c;
	for (;;) {
		c = fgetc(stdin);
		if (c == EOF)
			break;
		if (c == 10)
			fputc('\n', stdout);
		else
			fputc(c, stdout);
	}
}
