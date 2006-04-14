// ANOVA calculation for a homework problem

#include <stdio.h>

#define T 5
#define R 3
#define S 2

double data[T][R][S] = {
	89.8,	88.8,	94.5,	94.2,	90.9,	89.9,
	83.9,	84.0,	73.8,	74.5,	78.1,	79.2,
	79.2,	79.4,	88.3,	88.0,	69.1,	69.4,
	75.1,	74.8,	72.7,	72.8,	73.5,	73.6,
	88.6,	88.0,	82.8,	83.3,	87.8,	87.7,
};

double y, yi[T], yij[T][R];

main()
{
	int i, j, k;
	double t;

	// means

	t = 0.0;
	for (i = 0; i < T; i++)
		for (j = 0; j < R; j++)
			for (k = 0; k < S; k++)
				t += data[i][j][k];
	y = t / (T * R * S);

	printf("Y = %g\n", y);

	for (i = 0; i < T; i++) {
		t = 0.0;
		for (j = 0; j < R; j++)
			for (k = 0; k < S; k++)
				t += data[i][j][k];
		yi[i] = t / (R * S);
		printf("Y%d = %g\n", i + 1, yi[i]);
	}

	for (i = 0; i < T; i++) {
		for (j = 0; j < R; j++) {
			t = 0.0;
			for (k = 0; k < S; k++)
				t += data[i][j][k];
			yij[i][j] = t / S;
			printf("Y%d%d = %g\n", i + 1, j + 1, yij[i][j]);
		}
	}

	t = 0.0;
	for (i = 0; i < T; i++)
		for (j = 0; j < R; j++)
			t += (yij[i][j] - y) * (yij[i][j] - y);
	printf("SS among experimental units %g\n", S * t);

	t = 0.0;
	for (i = 0; i < T; i++)
		t += (yi[i] - y) * (yi[i] - y);
	printf("SS treatment %g\n", R * S * t);

	t = 0.0;
	for (i = 0; i < T; i++)
		for (j = 0; j < R; j++)
			t += (yij[i][j] - yi[i]) * (yij[i][j] - yi[i]);
	printf("SS experimental error %g\n", S * t);

	t = 0.0;
	for (i = 0; i < T; i++)
		for (j = 0; j < R; j++)
			for (k = 0; k < S; k++)
				t += (data[i][j][k] - yij[i][j]) * (data[i][j][k] - yij[j][j]);
	printf("SS sampling error %g\n", t);

	t = 0.0;
	for (i = 0; i < T; i++)
		for (j = 0; j < R; j++)
			for (k = 0; k < S; k++)
				t += (data[i][j][k] - y) * (data[i][j][k] - y);
	printf("SS total %g\n", t);
}
