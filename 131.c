/* Experimenting with 511 pattern. */

#include <stdio.h>

#define BIT(x, y) (x >> y & 1)

int p1[512], p2[512];

main()
{
	int i, j, n;

	/* traditional algorithm */

	for (i = 0; i < 512; i++) {
		n = i;
		for (j = 0; j < 7; j++)
			if (BIT(n, 8) ^ BIT(n, 4))
				n = n << 1 | 1;
			else
				n <<= 1;
		p1[i] = n & 0x1ff;
	}

	/* divisor algorithm */

	for (i = 0; i < 512; i++) {
		n = i;
		for (j = 0; j < 7; j++) {
			n <<= 1;
			if (n & 0x200)
				n ^= 0x011;
		}
		p2[i] = n & 0x1ff;
	}

	for (i = 0; i < 512; i++)
		printf("%3d. %3d %3d\n", i, p1[i], p2[i]);
}
