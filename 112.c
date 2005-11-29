/* Find a useful generator polynomial of degree 7. */

#include <stdio.h>

char pattern[510];

main()

{

	int i, j, k, n = 0;

	for (i = 6; i >= 2; i--)

		for (j = i - 1; j >= 1; j--)

			for (k = j - 1; k >= 0; k--) {

				printf("\ntrial #%d\n", ++n);

				printf("taps = %d %d %d %d\n", 7, i, j, k);

				generate(7, i, j, k);

				if (test())

					print_pattern();

			}

}

generate(b1, b2, b3, b4)

int b1, b2, b3, b4;

{

	int i, n, s;

	s = 0xff;

	b1 = 1 << b1;

	b2 = 1 << b2;

	b3 = 1 << b3;

	b4 = 1 << b4;

	for (i = 0; i < 510; i++) {

		n = 0;

		if (s & b1)

			n++;

		if (s & b2)

			n++;

		if (s & b3)

			n++;

		if (s & b4)

			n++;

		if (n & 1) {

			pattern[i] = '1';

			s = s << 1 | 1;

		} else {

			pattern[i] = '0';

			s <<= 1;

		}

	}

}

test()

{

	int i, n;

	for (i = 0; i < 255; i++)

		if (pattern[i] != pattern[i + 255]) {

			printf("length != 255\n");

			return 0;

		}

	n = 0;

	for (i = 0; i < 255; i++)

		if (pattern[i] == '1')

			n++;

	if (n != 128) {

		printf("ones != 128\n");

		return 0;

	}

	if (sequence_test("010") == 0) return 0;

	if (sequence_test("0110") == 0) return 0;

	if (sequence_test("01110") == 0) return 0;

	if (sequence_test("011110") == 0) return 0;

	if (sequence_test("0111110") == 0) return 0;

	if (sequence_test("01111110") == 0) return 0;

/*	if (sequence_test("011111110") == 0) return 0; */

	if (sequence_test("0111111110") == 0) return 0;

	if (sequence_test("101") == 0) return 0;

	if (sequence_test("1001") == 0) return 0;

	if (sequence_test("10001") == 0) return 0;

	if (sequence_test("100001") == 0) return 0;

	if (sequence_test("1000001") == 0) return 0;

	if (sequence_test("10000001") == 0) return 0;

	if (sequence_test("100000001") == 0) return 0;

	if (is_self_congruent()) {

		printf("is self-congruent\n");

		return 0;

	}

	return 1;

}

sequence_test(s)

char *s;

{

	int i, j, l;

	l = strlen(s);

	for (i = 0; i < 255; i++) {

		for (j = 0; j < l; j++)

			if (pattern[i + j] != s[j])

				break;

		if (j == l)

			return 1;

	}

	printf("failed pattern test %s\n", s);

	return 0;

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
