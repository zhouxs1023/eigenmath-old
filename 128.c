/* Generate CRC-4. */

#include <stdio.h>

main()
{
	int i;
	for (i = 0; i < 256; i++)
		printf("crc4(%d)=%d\n", i, crc4(i));
}

#define P 0x13

crc4(n)
int n;
{
	n <<= 4;

	if (n & 0x800)
		n ^= P << 7;

	if (n & 0x400)
		n ^= P << 6;

	if (n & 0x200)
		n ^= P << 5;

	if (n & 0x100)
		n ^= P << 4;

	if (n & 0x80)
		n ^= P << 3;

	if (n & 0x40)
		n ^= P << 2;

	if (n & 0x20)
		n ^= P << 1;

	if (n & 0x10)
		n ^= P << 0;

	return n;
}
