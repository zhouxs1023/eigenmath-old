/* Verify algorithm for deriving number of channels
$N$ from application rate in Bonding mode 3. */

#include <stdio.h>

main()

{

	int i;

	for (i = 1; i < 25; i++) {

		check(56 * i);

		check(64 * i);

	}

}

check(a)

int a;

{

	int bcr, n;

	bcr = 56;

	n = (64 * a) / 63 / bcr;

	if (bcr * n * 63 / 64 < a)

		n++;

	else

		printf("a=%d bcr=56\n", a);

	bcr = 64;

	n = (64 * a) / 63 / bcr;

	if (bcr * n * 63 / 64 < a)

		n++;

	else

		printf("a=%d bcr=64\n", a);

}
