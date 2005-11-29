/* Print first 2047 bits of 2047-bit pattern.
The shift register is initialized to all ones.
Note that the last eleven bits of the sequence are all ones
showing that the sequence repeats after 2047 bits.
See TA-TSY-000055 Issue 3, April 1987, page 21.
``The 2047-bit pattern may be derived via an eleven-stage
shift register with feedback through an exclusive OR-gate
from the outputs of ninth and eleventh stages.'' */

#include <stdio.h>

main()

{

	int i, s = 0xffff;

	for (i = 0; i < 2047; i++)

		if ((s & 0x500) == 0x400 || (s & 0x500) == 0x100) {

			printf("1");

			s = s << 1 | 1;

		} else {

			printf("0");

			s <<= 1;

		}

}
