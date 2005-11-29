/* Print all equations of the external derivative. */

#include <stdio.h>

main()

{

	int a, b, c, n = 0;

	printf("\\magnification=1200 \n");

	for (a = 1; a <= 4; a++)

		for (b = 1; b <= 4; b++)

			for (c = 1; c <= 4; c++) {

				n++;

				printf("$$ \\matrix { {\\bf %d.} & a=%d & b=%d & c=%d & \n", n, a, b, c);

				printf("{\\partial F_{%d%d} \\over \\partial x^%d} \n", a, b, c);

				printf("+ {\\partial F_{%d%d} \\over \\partial x^%d} \n", b, c, a);

				printf("+ {\\partial F_{%d%d} \\over \\partial x^%d} = 0 } $$ \n", c, a, b);

			}

	printf("\\end \n");

}
