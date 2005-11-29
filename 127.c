/* Converts delay values to equalizer settings */
/* */
/* Input: Relative delay values from dsp. */
/* */
/* Output: Equalizer settings. */
/* */
/* Example: check 3eaa 3eab 3eac 3eb3 */

#include <stdio.h>

int a[24];

main(argc, argv)
int argc;
char *argv[];
{
	int i, m, n;

	/* get input */

	n = argc - 1;

	for (i = 0; i < n; i++)
		sscanf(argv[i + 1], "%x", &a[i]);

	/* subtract cid */

	for (i = 0; i < n; i++)
		a[i] = (a[i] - i) & 0x3fff;

	/* normalize */

	m = a[0];

	for (i = 1; i < n; i++)
		if (a[i] < m)
			m = a[i];

	for (i = 0; i < n; i++)
		a[i] -= m;

	/* extend sign */

	for (i = 0; i < n; i++)
		if (a[i] & 0x2000)
			a[i] |= 0xc000;

	/* normalize */

	m = a[0];

	for (i = 1; i < n; i++)
		if (a[i] > m)
			m = a[i];

	m++;

	for (i = 0; i < n; i++)
		a[i] -= m;

	/* print results */

	for (i = 0; i < n; i++)
		printf("%d %04x\n", a[i], a[i]);
}
