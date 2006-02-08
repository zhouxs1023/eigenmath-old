// Fast CRC-32 using an enormous lookup table, by G. Weigt.

unsigned char *buf = "The rain in Spain.";

/* G(x) = x^32 + x^26 + x^23 + x^22 + x^16
+ x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4
+ x^2 + x + 1 */

/* G is reflected (bit 0 corresponds to x^31) */

#define G 0xedb88320

main()
{
	method1();
	method2();
}

// method 1 uses bit shifting

method1()
{
	int i, j;
	unsigned int crc = 0;
	for (i = 0; i < 18; i++) {
		crc ^= buf[i];
		for (j = 0; j < 8; j++)
			if (crc & 1)
				crc = (crc >> 1) ^ G;
			else
				crc >>= 1;
	}
	printf("%#x\n", crc);
}

// method 2 uses table lookup

unsigned int shift16[65536];

method2()
{
	int i;
	unsigned int crc = 0;
	make_table();
	for (i = 0; i < 18; i += 2) {
		crc ^= (buf[i + 1] << 8) | buf[i];
		crc = shift16[crc & 0xffff] ^ (crc >> 16);
	}
	printf("%#x\n", crc);
}

make_table()
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
