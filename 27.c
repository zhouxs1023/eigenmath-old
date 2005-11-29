/* print eigenvalues for random skew-symmetric linear transformation F */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

main()
{
	double a, b, b1, b2, b3, e1, e2, e3, v1, v2, v3, v4;

	printf("Print eigenvalues for random skew-symmetric\n");
	printf("linear transformation F.\n");

	randomize();

	b1 = random(201) - 100;
	b2 = random(201) - 100;
	b3 = random(201) - 100;

	e1 = random(201) - 100;
	e2 = random(201) - 100;
	e3 = random(201) - 100;

	a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);

	b = e1 * b1 + e2 * b2 + e3 * b3;

	v1 = sqrt(-a + sqrt(a * a + 4.0 * b * b)) / sqrt(2.0);

	v2 = -v1;

	v3 = sqrt(a + sqrt(a * a + 4.0 * b * b)) / sqrt(2.0);

	v4 = -v3;

	printf("E=(%g,%g,%g)\n", e1, e2, e3);
	printf("B=(%g,%g,%g)\n", b1, b2, b3);

	printf("a=|B|^2-|E|^2=%g\n", a);

	printf("b=E.B=%g\n", b);

	printf("eigenvalues:\n");
	printf("%g\n", v1);
	printf("%g\n", v2);
	printf("%gi\n", v3);
	printf("%gi\n", v4);
}
