/* Print first 127 bits of 127-bit pattern.
The shift register is initialized to all ones.
Note that the last seven bits of the sequence are all ones
showing that the sequence repeats after 127 bits. */

#include <stdio.h>

main()

{

	int i, s = 0xff;

	for (i = 0; i < 127; i++)

		if ((s & 0x48) == 0x40 || (s & 0x48) == 0x08) {

			printf("1");

			s = s << 1 | 1;

		} else {

			printf("0");

			s <<= 1;

		}

}
