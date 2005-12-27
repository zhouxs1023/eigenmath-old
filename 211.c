// Quick program to solve Exercise 3.

#include <stdio.h>

#define M 1000

char *var[11] = {"Z", "X1", "X2'", "X2''", "X3", "R", "S1", "S2", "S3", "S4", "S5"};

int basic[6] = {0, 6, 5, 8, 9, 10};

double c[6][12] = {
	{1,	M-6,	2*M-4,	4-2*M,	-M-8,	0,	0,	M,	0,	0,	0,	-M},
	{0,	1,	1,	-1,	1,	0,	1,	0,	0,	0,	0,	4},
	{0,	-1,	-2,	2,	1,	1,	0,	1,	0,	0,	0,	1},
	{0,	2,	0,	0,	3,	0,	0,	0,	1,	0,	0,	8},
	{0,	0,	2,	-2,	4,	0,	0,	0,	0,	1,	0,	10},
	{0,	1,	2,	-2,	3,	0,	0,	0,	0,	0,	1,	7}
};

main()
{
	int i, j, k, pivot_row, pivot_column, iteration = 0;
	double s, t, pivot_element;

	while (1) {

		printf("\nIteration %d\n\n", ++iteration);

		printf("Basic    Z        X1       X2'      X2''     X3       R        S1       S2       S3       S4       S5       Solution\n");

		for (i = 0; i < 6; i++) {
			printf("%-9s", var[basic[i]]);
			for (j = 0; j < 12; j++)
				printf("%-9g", c[i][j]);
			printf("\n");
		}

		printf("\n");

		// find the entering variable (most negative non-basic variable)

		pivot_column = 0;
		for (i = 1; i < 11; i++) {
			// check to see if "i" is a basic variable
			for (j = 0; j < 6; j++)
				if (basic[j] == i)
					break;
			if (j < 6)
				continue; // it's a basic variable
			if (pivot_column == 0 || c[0][i] < c[0][pivot_column])
				pivot_column = i;
		}

		// if non-negative then done

		if (c[0][pivot_column] >= 0) {
			printf("done\n");
			break;
		}

		printf("%s is the entering variable\n", var[pivot_column]);

		// find the leaving variable

		pivot_row = 0;
		for (i = 1; i < 6; i++) {
			if (c[i][pivot_column] <= 0.0)
				continue;
			t = c[i][11] / c[i][pivot_column];
			if (pivot_row == 0 || t < s) {
				s = t;
				pivot_row = i;
			}
		}

		if (pivot_row == 0) {
			printf("cannot determine the leaving variable\n");
			exit(0);
		}

		printf("%s is the leaving variable\n", var[basic[pivot_row]]);

		pivot_element = c[pivot_row][pivot_column];

		// new pivot row

		for (i = 0; i < 12; i++)
			c[pivot_row][i] /= pivot_element;

		// other rows

		for (i = 0; i < 6; i++) {
			if (i == pivot_row)
				continue;
			t = c[i][pivot_column];
			for (j = 0; j < 12; j++)
				c[i][j] -= t * c[pivot_row][j];
		}

		// new basic variable

		basic[pivot_row] = pivot_column;
	}
}
