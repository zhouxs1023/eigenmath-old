/* Generate every possible map of three elements, check for monoids. */

#include <stdio.h>

int m[3][3], n, j, k;

#define M(a,b) m[a][b]

main()
{
	for (m[0][0] = 0; m[0][0] < 3; m[0][0]++)
	for (m[0][1] = 0; m[0][1] < 3; m[0][1]++)
	for (m[0][2] = 0; m[0][2] < 3; m[0][2]++)

	for (m[1][0] = 0; m[1][0] < 3; m[1][0]++)
	for (m[1][1] = 0; m[1][1] < 3; m[1][1]++)
	for (m[1][2] = 0; m[1][2] < 3; m[1][2]++)

	for (m[2][0] = 0; m[2][0] < 3; m[2][0]++)
	for (m[2][1] = 0; m[2][1] < 3; m[2][1]++)
	for (m[2][2] = 0; m[2][2] < 3; m[2][2]++)

		f();

	printf("There are %d associative maps.\n", n);
	printf("There are %d maps with left and right identities.\n", j);
	printf("There are %d monoids.\n", k);
}

f()
{
	int a, b, c, l, r;

	for (a = 0; a < 3; a++)
	for (b = 0; b < 3; b++)
	for (c = 0; c < 3; c++)

		if (M(M(a,b),c) != M(a,M(b,c)))
			return;

	n++;

	/* left inverse */

	for (l = 0; l < 3; l++) {
		for (a = 0; a < 3; a++)
			if (M(l,a) != a)
				break;
		if (a == 3)
			break;
	}

	if (l == 3)
		return;

	/* right inverse */

	for (r = 0; r < 3; r++) {
		for (a = 0; a < 3; a++)
			if (M(a,r) != a)
				break;
		if (a == 3)
			break;
	}

	if (r == 3)
		return;

	printf("%d %d %d\n", m[0][0], m[0][1], m[0][2]);
	printf("%d %d %d\n", m[1][0], m[1][1], m[1][2]);
	printf("%d %d %d\n", m[2][0], m[2][1], m[2][2]);
	printf("l=%d, r=%d\n\n", l, r);

	j++;

	if (l == r)
		k++;
}
