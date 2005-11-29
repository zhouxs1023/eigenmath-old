/* Experimenting with 511 pattern. */

#include <stdio.h>

#define BIT(x, y) (x >> y & 1)

main(argc, argv)
int argc;
char *argv[];
{
	int i, j, n, x1, x2;

	/* traditional algorithm */

	sscanf(argv[1], "%d", &n);

	x1 = n;

	for (i = 0; i < 9; i++)
		if (BIT(x1, 8) ^ BIT(x1, 4))
			x1 = x1 << 1 | 1;
		else
			x1 <<= 1;

	x1 &= 0x1ff;

	/* find a divisor */

	for (i = 0; i < 512; i++) {

		x2 = divide(n, i);

		if (x1 == x2)
			printf("divisor = 0x%03x\n", i);
	}
}

divide(n, d)
int n, d;
{
	int i;

	for (i = 0; i < 9; i++) {
		n <<= 1;
		if (n & 0x200)
			n ^= d;
	}

	return n & 0x1ff;
}
