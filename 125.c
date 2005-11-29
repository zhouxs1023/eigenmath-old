/* Generate look-up table for 63 pattern taken 7 bits at a time. */

#include <stdio.h>

main()
{
	int i, j, u;

	for (i = 0; i < 128; i++) {
		u = i;
		for (j = 0; j < 7; j++) {
			if ((u ^ u << 1) & 0x20)
				u = u << 1 | 1;
			else
				u <<= 1;
		}
		if ((u & 0x3f) == 0)
			u = 0x3f;
		printf("\t0x%04x,\n", (u & 0x7f) << 8);
	}
}
		