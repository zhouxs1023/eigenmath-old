/* Is there a multiplier that yields the same result as
dividing by the polynomial $1+x^{-3}+x^{-5}$?
This program shows that the answer is no.
See Stallings p. 352. */

#include <stdio.h>

long a = 0x5507;

long b = 0x5c69;

main()

{

	int i;

	for (i = 0; i < 64; i++)

		if (test(i))

			printf("0x%02x\n", i);

}

test(multiplier)

int multiplier;

{

	long x, product = 0;

	x = a;

	while (multiplier) {

		if (multiplier & 1)

			product ^= x;

		x <<= 1;

		multiplier >>= 1;

	}

	if ((product >> 5) == b)

		return 1;

	else

		return 0;
}
