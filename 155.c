/* Verify algorithm for deriving number of channels
$N$ from application rate in Bonding mode 3. */

#include <stdio.h>

main()

{

	int i;

	for (i = 1; i < 25; i++) {

		check(56L * i);

		check(64L * i);

	}

}

check(a)

long a;

{

	long bcr, n;

	bcr = 56;

	n = (64 * a) / 63 / bcr;

	if (n * 63 * bcr < 64 * a)

		n++;

	if (n != (64 * a + 63 * bcr - 1) / 63 / bcr)

		printf("fail a=%ld bcr=%ld\n", a, bcr);

	bcr = 64;

	n = (64 * a) / 63 / bcr;

	if (n * 63 * bcr < 64 * a)

		n++;

	if (n != (64 * a + 63 * bcr - 1) / 63 / bcr)

		printf("fail a=%ld bcr=%ld\n", a, bcr);


}
