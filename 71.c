/* This program generates inline DSP code for reading 7 bits of data
from a 16-bit wide FIFO. */

#include <stdio.h>

main()
{
	int i, se = 0;

	for (i = 0; i < 16; i++) {

		se += 7;

		if (se >= 16) {

			printf("y8%02d:\tSR = LSHIFT SI BY %d (LO);\n\n", i, se + 1);

			se -= 16;

			printf("\tSI = DM(I6,M4);\n\n");

			printf("\tSR = SR OR LSHIFT SI BY %d (LO);\n\n", se + 1);

		} else

			printf("y8%02d:\tSR = LSHIFT SI BY %d (LO);\n\n", i, se + 1);

		printf("\tDM(I4,M4) = SR1;\n\n");

		printf("\tI7 = ^y8%02d;\n\n", (i + 1) % 16);

		printf("\tRTS;\n\n");
	}
}
