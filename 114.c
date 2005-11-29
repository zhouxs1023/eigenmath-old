/* Generate CRC table for Bonding engine. */

#include <stdio.h>

int a[256], b[256];

main()

{

	int i;

	generate_a();

	generate_b();

	for (i = 0; i < 256; i++)

		printf("%04x\n", b[i] << 8 | a[i]);

}

generate_a()

{

	int d, i, r;

	for (d = 0; d < 256; d++) {

		r = d;

		for (i = 0; i < 8; i++) {

			r <<= 1;

			if (r & 0x100)

				r ^= 0x130;

		}

		a[d] = r;

	}

}

generate_b()

{

	int d, i, r;

	b[0] = 0xff;

	for (d = 1; d < 256; d++) {

		r = d;

		for (i = 0; i < 8; i++) {

			r <<= 1;

			if (r & 0x100)

				r ^= 0x187;

		}

		b[d] = r;

	}

}
