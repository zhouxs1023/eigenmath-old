/* For what number of time slots does 7e pattern simulate $f_t$? */

#include <stdio.h>

int x[10000];

main()
{
	int i, j, n;

	for (i = 0; i < 10000; i++)
		if ((i & 7) == 0 || (i & 7) == 7)
			x[i] = 0;
		else
			x[i] = 1;

	for (i = 0; i < 24; i++) {

		n = 7 * (i + 1);	/* number of bits in a frame */

		for (j = 0; j < 5000; j++)

			if(
			x[j         ] == 1 &&
			x[j +  2 * n] == 0 &&
			x[j +  4 * n] == 1 &&
			x[j +  6 * n] == 0 &&
			x[j +  8 * n] == 1 &&
			x[j + 10 * n] == 0)
				break;

		if (j < 5000)
			printf("n = %d failed\n", i + 1);
		else
			printf("n = %d ok\n", i + 1);
	}
}
