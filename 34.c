/* Find the coefficients for the dot product. */

#include <stdio.h>

main()
{
	int i1, i2, j1, j2, v1, v2;

	for (v1 = 0; v1 < 4; v1++)
	for (v2 = v1; v2 < 4; v2++) {

		for (i1 = 0; i1 < 4; i1++)
		for (j1 = 0; j1 < 4; j1++)

		for (i2 = 0; i2 < 4; i2++)
		for (j2 = 0; j2 < 4; j2++)

			if (i1 == i2 && (j1 == v1 && j2 == v2 || j1 == v2 && j2 == v1))
				printf("L%d%d . L%d%d . x%d .x%d\n", i1 + 1, j1 + 1, i2 + 1, j2 + 1, v1 + 1, v2 + 1);

		printf("\n");
	}
}
