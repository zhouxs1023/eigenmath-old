/* Find a useful generator polynomial of degree 8. */

#include <stdio.h>

char pattern[510];

main()

{

	int i, j, k, l, n = 0;

	for (i = 0; i < 8; i++)

		for (j = i; j < 8; j++)

			for (k = j; k < 8; k++)

				for (l = k; l < 8; l++) {

					generate(i, j, k, l);

					if (is_self_congruent() == 0) {

						printf("\ntrial #%d\n", ++n);

						printf("polynomial = x^8 + x^%d + x^%d + x^%d + x^%d\n", l, k, j, i);

						print_pattern();

					}

				}

}

generate(b1, b2, b3, b4)

int b1, b2, b3, b4;

{

	int i, m, n, s;

	s = 0x80;

	m = 1 << 8 | 1 << b1 | 1 << b2 | 1 << b3 | 1 << b4;

	for (i = 0; i < 510; i++) {

		s <<= 1;

		/* if remainder is big enough then subtract */

		if (s & 0x100)

			s ^= m;

		if (s & 1)

			pattern[i] = '1';

		else

			pattern[i] = '0';

	}

}

print_pattern()

{

	int i, k = 0;

	for (i = 0; i < 255; i++) {

		printf("%c", pattern[i]);

		if (++k == 64) {

			k = 0;

			printf("\n");

		}

	}

	printf("\n");

}

is_self_congruent()

{

	int i, j, k;

	for (i = 0; i < 255; i++)

		for (j = 0; j < 255; j++)

			if (i != j) {

				for (k = 0; k < 8; k++)

					if (pattern[i + k] != pattern[j + k])

						break;

				if (k == 8)

					return 1;

			}

	return 0;

}
