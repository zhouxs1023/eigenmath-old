/* Bonding frame demonstration. */

#include <stdio.h>

main()
{
	int i, d = 1;

	for (i = 0; i < 256; i++) {

		printf("frame %3d: ", i + 1);

		switch (i) {

		case 63:
			printf("FAW ");
			break;

		case 127:
			printf("IC  ");
			break;

		case 191:
			printf("FC  ");
			break;

		case 255:
			printf("CRC ");
			break;

		default:
			printf("%3d ", d++);
			break;
		}

		switch ((i + 255) % 256) {

		case 63:
			printf("FAW ");
			break;

		case 127:
			printf("IC  ");
			break;

		case 191:
			printf("FC  ");
			break;

		case 255:
			printf("CRC ");
			break;

		default:
			printf("%3d ", d++);
			break;
		}

		printf("\n");
	}
}
