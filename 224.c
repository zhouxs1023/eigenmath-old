// statistical methods

#include <stdio.h>

#define N 8

double x[N] = {849,903,890,1092,362,900,1006,672};
double y[N] = {96,41,31,124,46,53,113,174};

main()
{
	int i;
	double d = 0.0, s2 = 0.0;
	for (i = 0;i < N; i++)
		d += x[i] - y[i];
	d /= N;
	for (i = 0; i < N; i++)
		s2 += (x[i] - y[i] - d) * (x[i] - y[i] - d);
	s2 /= N - 1;
	printf("%lg %lg\n", d, s2);
}
