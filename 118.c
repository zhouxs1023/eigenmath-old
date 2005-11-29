/* Generate CRC table for Bonding engine.
Use the polynomial $x^7+x+1$ for disconnect. */

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

	int x, y, i;

	x = next8();

	for (i = 0; i < 127; i++) {

		y = next8();

		b[x & 0x7f] = y;

		b[x | 0x80] = y;

		x = y;

	}

	b[0] = 0x7f;

	b[128] = 0x7f;

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
