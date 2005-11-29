/* This program generates a table of time slot usage. */

#include <stdio.h>

int ts[32], bw;

main()
{
	int i, j, k;

	for (i = 0; i < 32; i++) {

		f(0, i, 1);

		printf("\fDO NOT USE TIME SLOT 16:\n");

		printf("     SW5-1 (TOP CHANNEL)\n");

		printf("     SW9-1 (BOTTOM CHANNEL)\n");

		printf("     OFF\n");

		printheader(i);

		for (j = 0; j < 32; j++) {

			f(0, i, j);

			printf("     ");

			printswitch(~j);

			/* print time slots used */

			for (k = 0; k < 32; k++)

				if (ts[k])

					printf("%02d ", k);

						else

					printf("-- ");


			printf("\n");

		}

	}

	for (i = 0; i < 32; i++) {

		f(1, i, 1);

		printf("\fOK TO USE TIME SLOT 16:\n");

		printf("     SW5-1 (TOP CHANNEL)\n");

		printf("     SW9-1 (BOTTOM CHANNEL)\n");

		printf("     ON\n");

		printheader(i);

		for (j = 0; j < 32; j++) {

			f(1, i, j);

			printf("     ");

			printswitch(~j);

			/* print time slots used */

			for (k = 0; k < 32; k++)

				if (ts[k])

					printf("%02d ", k);

						else

					printf("-- ");


			printf("\n");

		}

	}


}


/*
	u		use time slot 16

	n		number of time slots

	b		base time slot
*/

f(u, n, b)
int u, n, b;
{
	int i, x;

	/* clear */

	for (i = 0; i < 32; i++)
		ts[i] = 0;

	/* flag time slots */

	x = b;

	for (i = 0; i < n; i++) {

		if (x == 0)
			x++;

		if (x == 16 && u == 0)
			x++;

		ts[x] = 1;

		x = (x + 1) % 32;
	}

	bw = 0;

	for (i = 0; i < 32; i++)

		bw += ts[i];

}

printswitch(n)

int n;

{

	if (n & 0x10)

		printf("ON    ");

	else

		printf("OFF   ");

	if (n & 0x08)

		printf("ON    ");

	else

		printf("OFF   ");

	if (n & 0x04)

		printf("ON    ");

	else

		printf("OFF   ");

	if (n & 0x02)

		printf("ON    ");

	else

		printf("OFF   ");

	if (n & 0x01)

		printf("ON    ");

	else

		printf("OFF   ");

}

printheader(n)

int n;

{

	printf("BANDWIDTH = %d KBPS (%d x 64 KBPS):\n", 64 * bw, bw);

	printf("     SW3-8 SW4-1 SW4-2 SW4-3 SW4-4 SW4-5 (TOP CHANNEL)\n");

	printf("     SW7-8 SW8-1 SW8-2 SW8-3 SW8-4 SW8-5 (BOTTOM CHANNEL)\n");

	printf("     ON    ");

	printswitch(n);

	printf("\n");

	printf("BANDWIDTH = %d KBPS (%d x 56 KBPS):\n", 56 * bw, bw);

	printf("     SW3-8 SW4-1 SW4-2 SW4-3 SW4-4 SW4-5 (TOP CHANNEL)\n");

	printf("     SW7-8 SW8-1 SW8-2 SW8-3 SW8-4 SW8-5 (BOTTOM CHANNEL)\n");

	printf("     OFF   ");

	printswitch(n);

	printf("\nBASE TIME SLOT:\n");

	printf("     SW2-1 SW2-2 SW2-3 SW2-4 SW2-5 (TOP CHANNEL)\n");

	printf("     SW6-1 SW6-2 SW6-3 SW6-4 SW6-5 (BOTTOM CHANNEL)         TIME SLOTS USED\n");

}
