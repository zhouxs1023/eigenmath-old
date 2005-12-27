// Another simplex demo.

#include <stdio.h>

char *var[5] = {"Z", "X1", "X2", "X3", "S1"};

int basic[3] = {0, 2, 1};

double c[3][8] = {
	{1,	0,	0,	7.1429,	0.1429,	14.5714},
	{0,	0,	1,	0.1429,	0.1429,	0.571429},
	{0,	1,	0,	0.8571,	-0.1429,6.42857},
};

main()
{
	int i, j, k, pivot_row, pivot_column, iteration = 0;
	double s, t, pivot_element;

	while (1) {

		// print the tableau

		printf("\nIteration %d\n\n", ++iteration);
		printf("Basic     r        X1       X2       X3       S1       Solution\n");
		for (i = 0; i < 3; i++) {
			printf("%-9s", var[basic[i]]);
			for (j = 0; j < 6; j++)
				printf(" %-8g", c[i][j]);
			printf("\n");
		}
		printf("\n");

		// find the entering variable (most positive non-basic variable)

		pivot_column = 0;
		for (i = 1; i < 5; i++) {
			// check to see if "i" is a basic variable
			for (j = 0; j < 3; j++)
				if (basic[j] == i)
					break;
			if (j < 3)
				continue; // it's a basic variable
			if (pivot_column == 0 || c[0][i] > c[0][pivot_column])
				pivot_column = i;
		}

		// if non-positive then done

		if (c[0][pivot_column] <= 0) {
			printf("done\n");
			break;
		}

		printf("%s is the entering variable\n", var[pivot_column]);

		// find the leaving variable

		pivot_row = 0;
		for (i = 1; i < 3; i++) {
			if (c[i][pivot_column] <= 0.0)
				continue;
			t = c[i][5] / c[i][pivot_column];
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

		for (i = 0; i < 6; i++)
			c[pivot_row][i] /= pivot_element;

		// other rows

		for (i = 0; i < 3; i++) {
			if (i == pivot_row)
				continue;
			t = c[i][pivot_column];
			for (j = 0; j < 6; j++) {
				s = c[i][j] - t * c[pivot_row][j];
				if (-1e-10 < s && s < 1e-10)
					s = 0.0; // clip
				c[i][j]  = s;
			}
		}

		// new basic variable

		basic[pivot_row] = pivot_column;
	}
}
