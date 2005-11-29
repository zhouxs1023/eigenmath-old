/* This program generates an Analog Devices DSP data file for initializing */
/* a CRC table. Least significant bit is the first bit sent and received. */

#include <stdio.h>

main()

{

	int crc, i, j;

	for (i = 0; i < 256; i++) {

		crc = i;

		for (j = 0; j < 8; j++)

			if (crc & 1)

				crc = ((crc >> 1) & 0x7fff) ^ 0x8408;

			else

				crc = (crc >> 1) & 0x7fff;

		printf("%04x\n", crc);

	}

}
