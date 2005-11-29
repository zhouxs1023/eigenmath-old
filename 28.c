/* find integers A and B such that $\sqrt{-A+\sqrt{A^2+4B^2}}$ = an integer */

#include <stdio.h>
#include <math.h>

main()
{
	int a, b, x;
	double y;
	printf("find integers A and B such that x = sqrt(-A+sqrt(A^2+4*B^2)) = an integer\n");
	for (a = -100; a <= 100; a++) {
		if (a == 0)
			continue;
		for (b = 1; b <= 100; b++) {
			if (kbhit())
				exit();
			x = y = sqrt(-a + sqrt(a * a + 4.0 * b * b));
			if (x == y)
				printf("a=%d b=%d x=%d\n", a, b, x);
		}
	}
}
