/* Build a table for processing SDLC protocol. */

/* Least significant bit is the first bit received. */

#include "168.c"

main()

{

	int i, j, n;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++) {

			f(i, j);

			printf("%d 0x%x <%d d=0x%x w=%d %d> a=%d next=%d\n", i, j, table[i][j].opening_flag, table[i][j].data, table[i][j].fieldwidth, table[i][j].closing_flag, table[i][j].abort, table[i][j].next_state);

		}

	/* count abort states */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].abort)

				n++;

	printf("%d abort states\n", n);

	printf("%d format error states\n", n);

	/* maximum field width */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].fieldwidth > n)

				n = table[i][j].fieldwidth;

	printf("maximum field width = %d bits\n", n);

}
