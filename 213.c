// This program solves the LP problem on p. 99 of the O.R. book by Taha.

#include <stdio.h>

#define NROW 4	// including the Z row
#define NCOL 8	// including the solution column

char *var[NCOL] = {"r", "X1", "X2", "S1", "S2", "R1", "R2", "Sol'n"};

int basic[NROW] = {0, 5, 6, 4}; // r, R1, R2, S2

//	Basic	r	X1	X2	S1	S2	R1	R2	Sol'n
//	r	1	7	4	-1	0	0	0	9
//	R1	0	3	1	0	0	1	0	3
//	R2	0	4	3	-1	0	0	1	6
//	S2	0	1	2	0	1	0	0	4

double c[NROW][NCOL] = {
	{1,	7,	4,	-1,	0,	0,	0,	9},
	{0,	3,	1,	0,	0,	1,	0,	3},
	{0,	4,	3,	-1,	0,	0,	1,	6},
	{0,	1,	2,	0,	1,	0,	0,	4},
};

main()
{
	int i, j, k, pivot_row, pivot_column, iteration = 0;
	double s, t, pivot_element;

	while (1) {

		// print the tableau

		printf("\nIteration %d\n\n", iteration++);
		printf("%-8s", "Basic");
		for (i = 0; i < NCOL; i++)
			printf("%-8s", var[i]);
		printf("\n");
		for (i = 0; i < NROW; i++) {
			printf("%-8s", var[basic[i]]);
			for (j = 0; j < NCOL; j++)
				printf("%-8g", c[i][j]);
			printf("\n");
		}
		printf("\n");

		// find the entering variable (most positive non-basic variable)

		pivot_column = 0;
		for (i = 1; i < NCOL - 1; i++) {
			// check to see if "i" is a basic variable
			for (j = 1; j < NROW; j++)
				if (basic[j] == i)
					break;
			if (j < NROW)
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
		for (i = 1; i < NROW; i++) {
			if (c[i][pivot_column] <= 0.0)
				continue;
			t = c[i][NCOL - 1] / c[i][pivot_column];
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

		for (i = 0; i < NCOL; i++)
			c[pivot_row][i] /= pivot_element;

		// other rows

		for (i = 0; i < NROW; i++) {
			if (i == pivot_row)
				continue;
			t = c[i][pivot_column];
			for (j = 0; j < NCOL; j++) {
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
