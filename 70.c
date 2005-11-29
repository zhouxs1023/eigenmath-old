/* This program generates inline DSP code for writing 7 bits of data
to a 16-bit wide FIFO. */

#include <stdio.h>

main()
{
	int i, j, se = 24;

	/* jump table */

	for (i = 0; i < 16; i++) {

		printf("y6%02d:\tJUMP y7%02d;\t\t\t{ ", 24 - i, 24 - i);

		for (j = 0; j < 16; j++)

			if (j < i)

				printf("d ");

			else

				printf("x ");

		printf("}\n\n");

	}

	for (i = 0; i < 16; i++) {

		se = 24 - i;

		printf("y7%02d:\tSI = DM(I4,M4);\n\n", se);

		printf("\tSR = SR OR LSHIFT SI BY %d (LO);\n\n", se);

		se -= 7;

		if (se < 9) {

			printf("\tDM(I6,M4) = SR1;\n\n");

			printf("\tSR1 = SR0;\n\n");

			printf("\tSR0 = 0;\n\n");

			se += 16;
		}

		printf("\tI7 = ^y6%02d;\n\n", se);

		printf("\tRTS;\n\n");
	}
}

/* print a state table */

test1()
{
	int j, se;

	for (se = 9; se < 25; se++) {

		j = se - 7;

		if (j < 9)
			printf("se=%d shift se=%d load se=%d\n", se, j, j + 16);
		else
			printf("se=%d shift se=%d\n", se, j);
	}
}

/* inline code with counter */

test2()
{
	int i, se = 24;

	for (i = 0; i < 16; i++) {

		printf("y7%02d:\tSR = SR OR LSHIFT AR BY %d,\n\n", i, se);

		printf("\t\tAR = DM(I4,M4);\n\n");

		se -= 7;

		if (se < 9) {

			printf("\tDM(I6,M4) = SR1,\n\n");

			printf("\t\tSR1 = SR0;\n\n");

			printf("\tSR0 = 0;\n\n");

			se += 16;
		}

		printf("\tAF = AF - 1;\n\n");

		printf("\tIF NE JUMP y7%02d;\n\n", (i + 1) % 16);

		printf("\tAR = ^y7%02d;\n\n", (i + 1) % 16);

		printf("\tRTS;\n\n");
	}
}
