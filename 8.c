/* Generate ground start state table. */

#include <stdio.h>

main()
{
	int a, b, l, r;

	for (l = 0; l < 2; l++)
	for (r = 0; r < 2; r++)
	for (a = 0; a < 2; a++)
	for (b = 0; b < 2; b++) {

		if (l)
			printf("loop=open   ");
		else
			printf("loop=closed ");

		if (r)
			printf("ringground=yes ");
		else
			printf("ringground=no  ");

		printf("a=%d b=%d ", a, b);

		if (l == 0 && r == 0 && a == 0 && b == 0)
			printf("ringing=yes ");
		else
			printf("ringing=no  ");

		if (a == 0)
			printf("tipground=yes ");
		else
			printf("tipground=no  ");

		printf("a=%d b=%d\n", (a ^ 1) & r | (a ^ 1) & l, (a ^ 1) | (r ^ 1));


	}
}
