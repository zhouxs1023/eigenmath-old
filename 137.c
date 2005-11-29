/* Simulate high-speed data card logic. */

#include <stdio.h>

int clk8, b0, b1, b2, c0, c1, db7, se;

struct {

	int b0, b1, b2, c0, c1, db7, se;

} new;

main()

{

	int i;

	printf("n\tclk8\tb0\tb1\tb2\tc0\tc1\tdb7\tse\n");

	for (i = -30; i < 200; i++) {

		clk8 ^= 1;

		if (clk8) {

			new.b0 ^= 1;

			if (b0)

				new.b1 ^= 1;

			if (b0 & b1)

				new.b2 ^= 1;

			if (b0 & b1 & b2)

				new.c0 ^= 1;

			if (b0 & b1 & b2 & c0)

				new.c1 ^= 1;

		} else

			new.db7 = b0 & b1 & b2;

		b0 = new.b0;

		b1 = new.b1;

		b2 = new.b2;

		c0 = new.c0;

		c1 = new.c1;

		db7 = new.db7;

		se = new.c0 & new.c1;

		printf("%d", i);

		print(clk8);

		print(b0);

		print(b1);

		print(b2);

		print(c0);

		print(c1);

		print(db7);

		print(se);

		printf("\n");

	}

}

print(x)

int x;

{

	if (x)

		printf("\t: |:");

	else

		printf("\t:| :");

}
