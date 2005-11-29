/* Verify that the look-up table for $x^7+x+1$ matches
the expected bit sequence. */

#include <stdio.h>

int table[256];

main()

{

	int a, b, i;

	generate_table();

	/* compare */

	a = next8();

	for (i = 0; i < 1000; i++) {

		b = next8();

		if (b != table[a])

			break;

		a = b;

	}

	if (i == 1000)

		printf("ok\n");

	else {

		printf("fail i=%d a=%02x b=%02x table[a]=%02x\n", i, a, b, table[a]);

		printf("a=");

		print_binary(a);

		printf("\n");

		printf("b=");

		print_binary(b);

		printf("\n");

	}

}

generate_table()

{

	int a, b, i;

	a = next8();

	for (i = 0; i < 127; i++) {

		b = next8();

		table[a & 0x7f] = b;

		table[a | 0x80] = b;

		a = b;

	}

}

int r = 0x40;

next8()

{

	int b, i;

	b = 0;

	for (i = 0; i < 8; i++) {

		r <<= 1;

		b <<= 1;

		if (r & 0x80) {

			r ^= 0x83;

			b |= 1;

		}

	}

	return b;

}

print_binary(a)

int a;

{

	int i;

	for (i = 0; i < 8; i++) {

		a <<= 1;

		if (a & 0x100)

			printf("1");

		else

			printf("0");

	}

}
