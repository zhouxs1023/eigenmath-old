/* Generate a table for converting gain switch settings to binary. */

#include <stdio.h>

main()
{
	int b, i;

	for (i = 0; i < 256; i++) {

		b = 0;

		if (i & 0x01)
			b -= 1;

		if (i & 0x02)
			b -= 2;

		if (i & 0x04)
			b -= 4;

		if (i & 0x08)
			b -= 8;

		if (i & 0x10)
			b -= 15;

		if (i & 0x20)
			b -= 30;

		if (i & 0x40)
			b -= 60;

		if (i & 0x80)
			b = -b;

		printf("\tDB\t%d\n", b);

	}
}
