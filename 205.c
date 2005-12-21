// O.R. -- feasible solution by northwest-corner method

#include <stdio.h>

int s[4] = {300, 100, 100, 100};
int d[3] = {200, 100, 300};
int m[4][3];

main()
{
	int i, j;
	for (i = 0; i < 6; i++) {
		nw();
		for (j = 0; j < 4; j++)
			printf("%5d %5d %5d\n", m[j][0], m[j][1], m[j][2]);
		printf("\n");
	}
}

nw()
{
	static int i, j;
	if (s[i] <= d[j]) {
		m[i][j] = s[i];
		d[j] -= s[i];
		s[i] = 0;
		i++;
	} else {
		m[i][j] = d[j];
		s[i] -= d[j];
		d[j] = 0;
		j++;
	}
}
