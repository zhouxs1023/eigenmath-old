/* Print a table of all $\epsilon_{abcd}.$ */

#include <stdio.h>

main()
{
	int a, b, c, d;

	for (a = 1; a <= 4; a++)
	for (b = 1; b <= 4; b++) {
		for (c = 1; c <= 4; c++) {
			printf("$$ \\matrix {\n");
			for (d = 1; d <= 4; d++)
				epsilon(a, b, c, d);
			printf("} $$\n");
		}
		printf("\n");
	}
}

epsilon(a, b, c, d)
int a, b, c, d;
{
	int i = 1;
	if (a == b || a == c || a == d || b == c || b == d || c == d)
		printf("\\epsilon_{%d%d%d%d}=0 &\n", a, b, c, d);
	else {
		if (a > b) i *= -1;
		if (a > c) i *= -1;
		if (a > d) i *= -1;
		if (b > c) i *= -1;
		if (b > d) i *= -1;
		if (c > d) i *= -1;
		printf("\\epsilon_{%d%d%d%d}=%d & \n", a, b, c, d, i);
	}
}
