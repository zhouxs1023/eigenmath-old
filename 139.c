/* Comparing V.54 and ANSI scrambling algorithms (scramble with 1). */

#include <stdio.h>

main()

{

	int b1 = 0, b2 = 0x7f, i;

	for (i = 0; i < 127; i++) {

		/* v.54 */

		if ((b1 & 9) == 8 || (b1 & 9) == 1)

			;

		else

			b1 |= 0x80;

		b1 >>= 1;

		/* ansi */

		if ((b2 & 9) == 8 || (b2 & 9) == 1)

			b2 |= 0x80;

		else

			;

		b2 >>= 1;

		printf("<%d%d>", (b1 & 0x40) >> 6, (~b2 & 0x40) >> 6);

		if ((b1 & 0x40) != (~b2 & 0x40))

			printf("fail");

	}

}
