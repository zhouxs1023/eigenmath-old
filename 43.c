/* Print a table of all $\epsilon_{abcd} \ne 0.$ */

#include <stdio.h>

main()
{
	int a, b, c, d, i;

	for (a = 1; a <= 4; a++)
	for (b = 1; b <= 4; b++)
	for (c = 1; c <= 4; c++)
	for (d = 1; d <= 4; d++) {

		if (a == b || a == c || a == d || b == c || b == d || c == d)
			continue;

		i = 1;

		if (a > b) i *= -1;
		if (a > c) i *= -1;
		if (a > d) i *= -1;
		if (b > c) i *= -1;
		if (b > d) i *= -1;
		if (c > d) i *= -1;

		printf("$$\\epsilon_{%d%d%d%d}=%d$$\n", a, b, c, d, i);
	}
}
