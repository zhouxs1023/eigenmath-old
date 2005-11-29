/* Compute $Lx\cdot Ly$ symbolically. */

#include <stdio.h>

char x[3][3][10], y[3][3][10];

main()
{
	int i, j, k;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			sprintf(x[i][j], "L%d%d . x%d", i + 1, j + 1, j + 1);

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			sprintf(y[i][j], "L%d%d . y%d", i + 1, j + 1, j + 1);

	printf("\nx =\n");

	for (i = 0; i < 3; i++)
		printf("%s + %s + %s\n", x[i][0], x[i][1], x[i][2]);

	printf("\ny =\n");

	for (i = 0; i < 3; i++)
		printf("%s + %s + %s\n", y[i][0], y[i][1], y[i][2]);

	printf("\n");

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			for (k = 0; k < 3; k++)
				printf("%s . %s\n", x[i][j], y[j][k]);
}
