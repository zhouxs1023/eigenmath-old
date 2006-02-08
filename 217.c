// Fast CRC-32C using an enormous lookup table, by G. Weigt.

unsigned char *buf = "The rain in Spain.";

/* G(x) = x^32 + x^26 + x^23 + x^22 + x^16
+ x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4
+ x^2 + x + 1 */

/* G is reflected (bit 0 corresponds to x^31) */

#define G 0xedb88320

main()
{
	init_table(); // for method 2

	method0();
	method1();
	method2();
}

// this method uses bit shifting, checked against ethernet FCS

method0()
{
	unsigned int i, j, r;

	/* first four bytes of packet */

	r = (unsigned int) buf[0]
	  | (unsigned int) buf[1] << 8
	  | (unsigned int) buf[2] << 16
	  | (unsigned int) buf[3] << 24;

	r = ~r; /* invert first 32 bits */

	for (i = 4; i < 18; i++) {

		/* shift 8 bits */

		for (j = 0; j < 8; j++)
			if (r & 1)
				r = (r >> 1) ^ G;
			else
				r >>= 1;

		/* fix up last 8 bits */

		r ^= (unsigned int) buf[i] << 24;
	}

	/* shift in 32 zero bits */

	for (i = 0; i < 32; i++)
		if (r & 1)
			r = (r >> 1) ^ G;
		else
			r >>= 1;

	r = ~r; /* invert result */

//	printf("FCS = %02x %02x %02x %02x\n",
//		r & 0xff, /* 1st byte sent */
//		r >> 8 & 0xff,
//		r >> 16 & 0xff,
//		r >> 24);

	printf("%#x\n", r);
}

// this method also uses bit shifting

method1()
{
	int i, j;
	unsigned int crc = ~0;
	for (i = 0; i < 18; i++) {
		crc ^= buf[i];
		for (j = 0; j < 8; j++)
			if (crc & 1)
				crc = (crc >> 1) ^ G;
			else
				crc >>= 1;
	}
	crc = ~crc;
	printf("%#x\n", crc);
}

// method 2 uses table lookup

unsigned int shift16[65536];

method2()
{
	int i;
	unsigned int crc = ~0;
	for (i = 0; i < 18; i += 2) {
		crc ^= (buf[i + 1] << 8) | buf[i];
		crc = shift16[crc & 0xffff] ^ (crc >> 16);
	}
	crc = ~crc;
	printf("%#x\n", crc);
}

init_table()
{
	int i, k;
	unsigned int crc;
	for (k = 0; k < 65536; k++) {
		crc = k;
		for (i = 0; i < 16; i++)
			if (crc & 1)
				crc = (crc >> 1) ^ G;
			else
				crc >>= 1;
		shift16[k] = crc;
	}
}
