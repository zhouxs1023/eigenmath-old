/* Simulate ground start logic. */

#include <stdio.h>

main()
{
	/* inputs */

	/* l = 1 for loop closed */

	/* rgnd = 1 for ring ground */

	int l, rgnd, ra, rb;

	/* outputs */

	/* ring = 1 for ringing voltage */

	/* tgnd = 1 for tip ground */

	int ring, tgnd, ta, tb;

	for (l = 0; l < 2; l++)
		for (rgnd = 0; rgnd < 2; rgnd++)
			for (ra = 0; ra < 2; ra++)
				for (rb = 0; rb < 2; rb++) {

					if (ra == 1)
						tgnd = 0;
					else
						tgnd = 1;

					if (l == 1) {

						ring = 0;

						if (ra == 0) {
							ta = 1;
							tb = 1;
						} else {
							ta = 0;
							tb = 0;
						}

					} else {

						if (ra == 0 && rb == 0 && rgnd == 0)
							ring = 1;
						else
							ring = 0;

						if (ra == 0 && rgnd == 1)
							ta = 1;
						else
							ta = 0;

						if (ra == 1 && rgnd == 1)
							tb = 0;
						else
							tb = 1;
					}

					if (l == 0)
						printf("loop=open   ");
					else
						printf("loop=closed ");

					if (rgnd == 0)
						printf("ringground=no  ");
					else
						printf("ringground=yes ");

					if (ra == 0)
						printf("a=0 ");
					else
						printf("a=1 ");

					if (rb == 0)
						printf("b=0 ");
					else
						printf("b=1 ");

					if (ring == 0)
						printf("ringing=no  ");
					else
						printf("ringing=yes ");

					if (tgnd == 0)
						printf("tipground=no  ");
					else
						printf("tipground=yes ");

					if (ta == 0)
						printf("a=0 ");
					else
						printf("a=1 ");

					if (tb == 0)
						printf("b=0 ");
					else
						printf("b=1 ");

					printf("\n");
				}
}
