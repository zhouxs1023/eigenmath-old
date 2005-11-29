/* Find a useful generator polynomial of degree 7. */

#include <stdio.h>

char pattern[510];

main()

{

	int i, n = 0;

	for (i = 1; i < 7; i++) {

		generate(i);

		if (is_self_congruent() == 0) {

			printf("%d. polynomial = x^7 + x^%d + 1\n", ++n, i);

			print_pattern();

		}

	}

}

generate(b)

int b;

{

	int i, m, n, r;

	m = 1 << 7 | 1 << b | 1;

	r = 0x40;

	for (i = 0; i < 510; i++) {

		r <<= 1;

		/* if remainder is big enough then subtract */

		if (r & 0x80)

			r ^= m;

		if (r & 1)

			pattern[i] = '1';

		else

			pattern[i] = '0';

	}

}

print_pattern()

{

	int i, k = 0;

	for (i = 0; i < 127; i++) {

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

	for (i = 0; i < 127; i++)

		for (j = 0; j < 127; j++)

			if (i != j) {

				for (k = 0; k < 7; k++)

					if (pattern[i + k] != pattern[j + k])

						break;

				if (k == 7)

					return 1;

			}

	return 0;

}
