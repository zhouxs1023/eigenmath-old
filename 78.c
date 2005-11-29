/* Differentiate (2.7.15) with respect to $x^c$. */

#include <stdio.h>

main()
{
	int c;

	for (c = 1; c <= 4; c++) {

		printf("\n$$\n");

		printf("{\\partial ^ 2 F_{ab} \\over (\\partial x^{%d})^2} +\n", c);

		printf("{\\partial \\over \\partial x^%d} \\left (\n", c);

		printf("{\\partial F_{b%d} \\over \\partial x^a}\n", c);

		printf("+\n");

		printf("{\\partial F_{%da} \\over \\partial x^b}\n", c);

		printf("\\right ) = 0 $$ \n");

	}
}
