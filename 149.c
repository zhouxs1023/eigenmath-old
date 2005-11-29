/* Simulate bit alignment algorithm. */

#include <stdio.h>

int flag[256];

#define F if (n == 1) flag[i & 0xff]++; if (++n == 36) n = 0

main()

{

	int i, n = 0;

	for (i = 0; i < 9 * 256; i++) {

		F;

		F;

		F;

		F;

	}

	for (i = 0; i < 256; i++)

		printf("%d", flag[i]);

}
