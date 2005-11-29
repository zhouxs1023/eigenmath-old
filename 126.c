/* Generate look-up table for 63 pattern taken 7 bits at a time.
Compares two polynomials. */

#include <stdio.h>

main()
{
	int i, u = 0x3f;

	for (i = 0; i < 63; i++) {
		if ((u ^ u << 1) & 0x20) {
			printf("1");
			u = u << 1 | 1;
		} else {
			printf("0");
			u <<= 1;
		}
	}

	printf("\n");

	u = 0x20;

	for (i = 0; i < 63; i++) {
		if ((u ^ u << 5) & 0x20) {
			printf("1");
			u = u << 1 | 1;
		} else {
			printf("0");
			u <<= 1;
		}
	}
}
		