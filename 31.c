/* Find F such that $E \cdot B \ne 0$ and $|B|^2-|E|^2 = 0$. */

#include <stdio.h>
#include <stdlib.h>

main()
{
	int a, b, b1, b2, b3, e1, e2, e3;

	printf("find F such that E.B != 0 and |B|^2-|E|^2 = 0\n");
	printf("-100 <= F[a][b] <= 100\n");
	printf("press any key to stop\n");

	for (;;) {

		if (kbhit())
			break;

		e1 = random(201) - 100;
		e2 = random(201) - 100;
		e3 = random(201) - 100;

		b1 = random(201) - 100;
		b2 = random(201) - 100;
		b3 = random(201) - 100;

		a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);

		b = e1 * b1 + e2 * b2 + e3 * b3;

		if (a == 0) {
			printf("\nE=(%d,%d,%d)\n", e1, e2, e3);
			printf("B=(%d,%d,%d)\n", b1, b2, b3);
			printf("|B|^2-|E|^2=%d\n", a);
			printf("E.B=%d\n", b);
		}
	}
}
