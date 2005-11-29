/* Generate the Annex B table in the Bonding specification. */

#include <stdio.h>

main()

{

	int i;

	int a, bcr, n, rmult, submult;

	bcr = 56;

	for (i = 1; i < 200; i++) {

		a = 8 * i;

		rmult = a / bcr;

		submult = (a - rmult * bcr) / 8;

		n = a / bcr;

		/* round up */

		if (n * bcr < a)

			n++;

		/* cover overhead traffic */

		if (n * bcr - n < a)

			n++;

		printf("a=%-10d rmult=%-10d submult=%-10d n=%-10d\n", a, rmult, submult, n);

	}

}
