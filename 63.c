/* Prove that ${\rm div}(F+G)={\rm div\,}F+{\rm div\,}G$.
Output in \TeX. See exercise 2.7.2, page 128 of Naber. */

#include <stdio.h>

main()
{
	int i, j;

	printf("$$ \\eqalign { {\\rm div}(F+G) = \n");

	/* step 1 */

	printf("\\left [ \\matrix {\n");
	for (i = 1; i <= 4; i++) {
		if (i == 4) printf("-");
		for (j = 1; j <= 4; j++) {
			printf("{\\partial [ {F^%d}_%d (x) + {G^%d}_%d (x) ] \\over \\partial x^%d}\n", j, i, j, i, j);
			if (j < 4) if (i < 4) printf("+"); else printf("-");
		}
		printf("\\cr\n");
	}
	printf("} \\right ] \n");

	/* step 2 */

	printf("\\cr \\cr = \n");

	printf("\\left [ \\matrix {\n");
	for (i = 1; i <= 4; i++) {
		if (i == 4) printf("-");
		for (j = 1; j <= 4; j++) {
			printf("\\left (\n");
			printf("{\\partial {F^%d}_%d (x) \\over \\partial x^%d}\n", j, i, j);
			printf("+{\\partial {G^%d}_%d (x) \\over \\partial x^%d}\n", j, i, j);
			printf("\\right )\n");
			if (j < 4) if (i < 4) printf("+"); else printf("-");
		}
		printf("\\cr\n");
	}
	printf("} \\right ]\n");

	/* step 3 */

	printf("\\cr \\cr = \n");

	printf("\\left [ \\matrix {\n");
	for (i = 1; i <= 4; i++) {
		if (i == 4) printf("-");
		for (j = 1; j <= 4; j++) {
			printf("{\\partial {F^%d}_%d (x) \\over \\partial x^%d}\n", j, i, j);
			if (j < 4) if (i < 4) printf("+"); else printf("-");
		}
		printf("\\cr\n");
	}
	printf("} \\right ]\n");

	printf("+\n");

	printf("\\left [ \\matrix {\n");
	for (i = 1; i <= 4; i++) {
		if (i == 4) printf("-");
		for (j = 1; j <= 4; j++) {
			printf("{\\partial {G^%d}_%d (x) \\over \\partial x^%d}\n", j, i, j);
			if (j < 4) if (i < 4) printf("+"); else printf("-");
		}
		printf("\\cr\n");
	}
	printf("} \\right ]\n");

	/* step 4 */

	printf("\\cr \\cr = {\\rm div \\,} F + {\\rm div \\,} G \n");

	printf("} $$\n");
}
