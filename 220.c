// CRC using big table lookup.

#include <stdio.h>

unsigned int shift8[256], shift16[65536];

unsigned int
fast_crc(unsigned char *data, int len)
{
	int n;
	unsigned int crc;
	unsigned short *p;

	crc = ~0;

	if ((unsigned long) data & 1) {
		crc ^= *data++;
		crc = shift8[crc & 0xff] ^ (crc >> 8);
		len--;
	}

	p = (unsigned short *) data;
	n = len / 2;

	while (n--) {
		crc ^= *p++;
		crc = shift16[crc & 0xffff] ^ (crc >> 16);
	}

	if (len & 1) {
		data = (unsigned char *) p;
		crc ^= *data;
		crc = shift8[crc & 0xff] ^ (crc >> 8);
	}

	return ~crc;
}

unsigned int
slow_crc(unsigned char *data, int len)
{
	unsigned int crc = ~0;

	while (len--) {
		crc ^= *data++;
		crc = shift8[crc & 0xff] ^ (crc >> 8);
	}

	return ~crc;
}

#define G 0x82f63b78		// iscsi polynomial

//#define G 0xedb88320		// ethernet polynomial

void
init_tables()
{
	int i, k;
	unsigned int crc;

	// small table

	for (k = 0; k < 256; k++) {
		crc = k;
		for (i = 0; i < 8; i++)
			if (crc & 1)
				crc = (crc >> 1) ^ G;
			else
				crc >>= 1;
		shift8[k] = crc;
	}

	// big table

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

unsigned char buf[1000];

main()
{
	int i, crc1, crc2;

	init_tables();

	for (i = 0; i < 1000; i++)
		buf[i] = i;

	// check for each boundary condition of buf

	// even even

	crc1 = fast_crc(buf, 1000);
	crc2 = slow_crc(buf, 1000);
	printf("%08x %08x\n", crc1, crc2);
	if (crc1 ^ crc2) {
		printf("failed\n");
		exit(1);
	}

	// even odd

	crc1 = fast_crc(buf, 999);
	crc2 = slow_crc(buf, 999);
	printf("%08x %08x\n", crc1, crc2);
	if (crc1 ^ crc2) {
		printf("failed\n");
		exit(1);
	}

	// odd even

	crc1 = fast_crc(buf + 1, 999);
	crc2 = slow_crc(buf + 1, 999);
	printf("%08x %08x\n", crc1, crc2);
	if (crc1 ^ crc2) {
		printf("failed\n");
		exit(1);
	}

	// odd odd

	crc1 = fast_crc(buf + 1, 998);
	crc2 = slow_crc(buf + 1, 998);
	printf("%08x %08x\n", crc1, crc2);
	if (crc1 ^ crc2) {
		printf("failed\n");
		exit(1);
	}

	printf("OK\n");
}
