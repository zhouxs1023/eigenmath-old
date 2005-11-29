/* verify the IMUX algorithm for calculating rmult and submult */

#include <stdio.h>

int a, br, bcr, m, rmult, submult;

main()

{

	br = 56; bcr = 56;

	test();

	br = 56; bcr = 64;

	test();

	br = 64; bcr = 56;

	test();

	br = 64; bcr = 64;

	test();

}

test()

{

	for (m = 1; m <= 32; m++) {

		a = m * br;

		to_rmult();

		to_a();

		printf("bcr=%d br=%d m=%d a=%d rmult=%d submult=%d\n", bcr, br, m, a, rmult, submult);

		if (a != m * br) {

			printf("fail\n");

			exit();

		}

	}

}

/* convert application rate to rmult and submult */

to_rmult()

{

	rmult = a / bcr;

	submult = (a - bcr * rmult) / 8;

}

/* convert bcr, rmult and submult to a */

to_a()

{

	a = bcr * rmult + 8 * submult;

}
