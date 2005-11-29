/* print hex value of keystroke */

#include <stdio.h>

main()
{
	int c;

	while (!kbhit())
		;

	c = getch();

	if (c)
		printf("%d\n", c);
	else
		printf("0 %d\n", getch());
}
