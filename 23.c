/* search for E . B = 0 in random integer F */

#include <stdio.h>
#include <stdlib.h>

main()
{
	int b1, b2, b3, e1, e2, e3, k, n = 0;
	long t = 0;

	printf("searching for E . B = 0 in random integer F\n");
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

		if (e1 * b1 + e2 * b2 + e3 * b3 == 0)
			printf("%d E1=%d E2=%d E3=%d B1=%d B2=%d B3=%d\n", ++n, e1, e2, e3, b1, b2, b3);

		t++;
	}

	printf("density=%g\n", (double) n / (double) t);
}
