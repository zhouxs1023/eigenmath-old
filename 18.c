/* Is $F:M\rightarrow M$ skew symmetric? */

#include <stdio.h>

char str[256];

main()
{
	double a[4][4];

	printf("is F:M->M skew-symmetric?  yes if F has the form\n\n");

	printf("    0    a    d    f\n\n");
	printf("   -a    0    b    e\n\n");
	printf("   -d   -b    0    c\n\n");
	printf("    f    e    c    0\n\n");

	printf("example:\n");

	printf("  0  1  2  3\n");
	printf(" -1  0  4  5\n");
	printf(" -2 -4  0  6\n");
	printf("  3  5  6  0\n\n");

	printf("row 1? ");
	scanf("%lg%lg%lg%lg", &a[0][0], &a[0][1], &a[0][2], &a[0][3]);

	printf("row 2? ");
	scanf("%lg%lg%lg%lg", &a[1][0], &a[1][1], &a[1][2], &a[1][3]);

	printf("row 3? ");
	scanf("%lg%lg%lg%lg", &a[2][0], &a[2][1], &a[2][2], &a[2][3]);

	printf("row 4? ");
	scanf("%lg%lg%lg%lg", &a[3][0], &a[3][1], &a[3][2], &a[3][3]);

	if (isskewsymmetric(a))
		printf("it is skew-symmetric\n");
	else
		printf("it is not skew-symmetric\n");
}

isskewsymmetric(a)
double a[4][4];
{
	if (
	a[0][0] == 0.0 &&
	a[1][1] == 0.0 &&
	a[2][2] == 0.0 &&
	a[3][3] == 0.0 &&
	a[0][1] == -a[1][0] &&
	a[0][2] == -a[2][0] &&
	a[1][2] == -a[2][1] &&
	a[0][3] == a[3][0] &&
	a[1][3] == a[3][1] &&
	a[2][3] == a[3][2])
		return 1;
	else
		return 0;
}
