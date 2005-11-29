/* This program simulates a DDS local loop decoding algorithm.
/* This program contains test vectors that simulate local loop signals.
/* Each vector is converted to DS-0 octets and the result is printed.
/*
/* Example:
/*
/* 111x0v111x0v111x0v111x0v000000000000000000
/*
/* 11111110 (cmi)
/*
/* 11111110 (cmi)
/*
/* 11111110 (cmi)
/*
/* 11111110 (cmi)
/*
/* 10000001
/*
/* 10000001 */

#include <stdio.h>

int bparity, cmi, data, loop, rparity, tparity, vparity;

int rpos, rneg, tpos, tneg;

main()

{

	test("111x0v111x0v111x0v111x0v000000000000000000");
	test("0111x0v111x0v111x0v111x0v00000000000000000");
	test("00111x0v111x0v111x0v111x0v0000000000000000");
	test("000111x0v111x0v111x0v111x0v000000000000000");
	test("0000111x0v111x0v111x0v111x0v00000000000000");
	test("00000111x0v111x0v111x0v111x0v0000000000000");
	test("000000111x0v111x0v111x0v111x0v000000000000");

	test("000000111x0v000000000000000000000000000000");
	test("0000000111x0v00000000000000000000000000000");
	test("00000000111x0v0000000000000000000000000000");
	test("000000000111x0v000000000000000000000000000");
	test("0000000000111x0v00000000000000000000000000");
	test("00000000000111x0v0000000000000000000000000");
	test("000000000000111x0v000000000000000000000000");

	test("000000111x0v111x0v000000000000000000000000");
	test("0000000111x0v111x0v00000000000000000000000");
	test("00000000111x0v111x0v0000000000000000000000");
	test("000000000111x0v111x0v000000000000000000000");
	test("0000000000111x0v111x0v00000000000000000000");
	test("00000000000111x0v111x0v0000000000000000000");
	test("000000000000111x0v111x0v000000000000000000");

	printf("\ntwo cmi with one bit in between\n");

	test("000000111x0v0111x0v00000000000000000000000");
	test("0000000111x0v0111x0v0000000000000000000000");
	test("00000000111x0v0111x0v000000000000000000000");
	test("000000000111x0v0111x0v00000000000000000000");
	test("0000000000111x0v0111x0v0000000000000000000");
	test("00000000000111x0v0111x0v000000000000000000");
	test("000000000000111x0v0111x0v00000000000000000");

}

test(s)

char *s;

{

	int i, n;

	printf("\n%s\n\n", s);

	cmi = 0;

	data = 0;

	rparity = 0;

	tparity = 0;

	vparity = 0;

	tpos = 0;

	tneg = 0;

	n = strlen(s);

	for (i = 0; i < n; i++) {

		tpos <<= 1;

		tneg <<= 1;

		switch (s[i]) {

		case '1':

			if (tparity == 0)

				tpos |= 1;

			else

				tneg |= 1;

			tparity ^= 1;

			vparity ^= 1;

			break;

		case 'x':

			if (vparity == 0) {

				if (tparity == 0)

					tpos |= 1;

				else

					tneg |= 1;

				tparity ^= 1;

				vparity ^= 1;

			}

			break;

		case 'v':

			if (tparity == 1)

				tpos |= 1;

			else

				tneg |= 1;

			vparity = 0;

			break;

		}

		if (i % 6 == 5) {

			shift6();

			if (i > 5)

				send();

		}

	}

}

shift6()

{

	int bpv, i;

	rpos = tpos;

	rneg = tneg;

	/* shift six bits */

	for (i = 0; i < 6; i++) {

		data <<= 1;

		rpos <<= 1;

		rneg <<= 1;

		cmi <<= 1;

		loop <<= 1;

		if ((rpos & 0x40) || (rneg & 0x40))

			data |= 0x01;

		/* generate bpv signal */

		if (rparity == 1 && (rpos & 0x40) || rparity == 0 && (rneg & 0x40))

			bpv = 1;

		else

			bpv = 0;

		/* update parity */

		if (rpos & 0x40)

			rparity = 1;

		if (rneg & 0x40)

			rparity = 0;

		/* zero */

		if (bpv && (data & 0x3b) == 0x01)

			data &= 0xffc0;		/* replace with all zeroes */

		/* cmi */

		if (bpv && (data & 0x3b) == 0x39) {

			data |= 0x3f;	/* replace with all ones */

			cmi |= 0x3f;

		}

		/* loopback */

		if (bpv && (data & 0x3b) == 0x11) {

			data |= 0x3f;	/* replace with all ones */

			loop |= 0x3f;

		}

	}

}

send()

{

	int i, m;

	if ((cmi & 0xfc0) == 0xfc0)

		printf("11111110 (cmi)\n");

	else {

		m = data;

		printf("1");

		for (i = 0; i < 6; i++) {

			if (m & 0x800)

				printf("1");
			else

				printf("0");

			m <<= 1;

		}

		printf("1\n");

	}

}
