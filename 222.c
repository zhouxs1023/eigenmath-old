// ANOVA calculation for a homework problem

#include <stdio.h>

#define N 22

double data[N] = {
	20.8, 4.1, 30.0, 24.7, 13.8,
	7.5, 7.5, 11.9, 4.5, 3.1, 8.0, 4.7, 28.1, 10.3, 10.0, 5.1, 2.2,
	9.2, 2.0, 2.5, 6.1, 7.5,
};

main()
{
	int i;
	double avg, sse, sst, t, tss;
	double y1 = 18.68;
	double y2 = 8.575;
	double y3 = 5.46;

	t = 0.0;
	for (i = 0; i < N; i++)
		t += data[i];
	avg = t / N;

	tss = 0.0;
	for (i = 0; i < N; i++)
		tss += (data[i] - avg) * (data[i] - avg);

	sst = 5.0 * (y1 - avg) * (y1 - avg) + 12.0 * (y2 - avg) * (y2 - avg) + 5.0 * (y3 - avg) * (y3 - avg);

	sse = 0.0;
	for (i = 0; i < 5; i++)
		sse += (data[i] - y1) * (data[i] - y1);
	for (i = 5; i < 17; i++)
		sse += (data[i] - y2) * (data[i] - y2);
	for (i = 17; i < 22; i++)
		sse += (data[i] - y3) * (data[i] - y3);

	printf("avg = %g\n", avg);
	printf("tss = %g\n", tss);
	printf("sst = %g\n", sst);
	printf("sse = %g\n", sse);
}
