/* Simulate DDS bipolar violation alignment algorithm. */

#include <stdio.h>

int ar, ay0, mr2, my1, sr, se;

main()

{

	int i, j;

	for (i = 0; i < 7; i++)

		for (j = 0; j < 14; j++) {

			mr2 = -i;

			print_mr2();

			my1 = 1 << j;

			print_v();

			align();

			print_mr2();

			printf("\n");

		}

}

align()

{

	se = mr2;

	ar = my1;

	sr = ar >> -se;

	ay0 = 0x7f;

	ar = sr & ay0;

	if (ar == 0)

		return;

	if (ar & 0x40)

		se = -8;

	if (ar & 0x20)

		se = -9;

	if (ar & 0x10)

		se = -10;

	if (ar & 0x08)

		se = -11;

	if (ar & 0x04)

		se = -12;

	if (ar & 0x02)

		se = -13;

	if (ar & 0x01)

		se = -14;

	ar = se;

	ay0 = 14;

	ar = ar + ay0;

	ay0 = mr2;

	ar = ay0 - ar;

	ay0 = 7;

	ar = ar + ay0;

	if (ar > 0)

		ar = ar - ay0;

	mr2 = ar;

}

print_mr2()

{

	int b, i;

	b = 0x7f << -mr2;

	for (i = 0; i < 16; i++)

		if (b & 1 << (15 - i))

			printf("*");

		else

			printf("-");

	printf("\n");

}

print_v()

{

	int i;

	for (i = 0; i < 16; i++)

		if (my1 & 1 << (15 - i))

			printf("1");

		else

			printf("0");

	printf("\n");

}
