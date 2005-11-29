/* Another shortest path demo. */

#define N 6

int n, A[100][100], d[100], s[100], IN[100];

int x = 0, y = 5;

main()
{
	int i, j, p, z, OldDistance;

	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)
			A[i][j] = 1000;

	A[0][1] = 83;
	A[0][2] = 163;
	A[0][3] = 290;
	A[0][4] = 502;
	A[0][5] = 821;

	A[1][2] = 89;
	A[1][3] = 173;
	A[1][4] = 304;
	A[1][5] = 520;

	A[2][3] = 95;
	A[2][4] = 183;
	A[2][5] = 318;

	A[3][4] = 101;
	A[3][5] = 193;

	A[4][5] = 113;

	IN[0] = x;
	n = 1;
	d[x] = 0;

	for (z = 0; z < N; z++)
		if (!in(z)) {
			d[z] = A[x][z];
			s[z] = x;
		}

	printf("Initial state:\n");
	print_state();

	while (!in(y)) {

		p = minz();

		printf("\np = %d\n", p);

		if (p == 0) {
			printf("No path.\n");
			exit(1);
		}

		IN[++n] = p;
		for (z = 0; z < N; z++)
			if (!in(z)) {
				OldDistance = d[z];
				d[z] = min(d[z], d[p] + A[p][z]);
				if (d[z] != OldDistance)
					s[z] = p;
			}

		print_state();
	}

	printf("In reverse order, the path is\n");
	printf("%d\n", y);
	z = y;
	do {
		printf("%d\n", s[z]);
		z = s[z];
	} while (z != x);
}

int min(int x, int y)
{
	if (x <= y)
		return x;
	else
		return y;
}

int in(int z)
{
	int i;
	for (i = 1; i <= n; i++)
		if (IN[i] == z)
			return 1;
	return 0;
}

int minz()
{
	int w, z, p;
	w = 1000;
	p = -1;
	for (z = 1; z <= N; z++)
		if (!in(z) && d[z] < w) {
			w = d[z];
			p = z;
		}
	return p;
}

print_state()
{
	int i;

	printf("IN =");
	for (i = 0; i < n; i++)
		printf(" %d", IN[i]);
	printf("\n");

	printf("d =");
	for (i = 0; i < N; i++)
		printf(" %d", d[i]);
	printf("\n");

	printf("s =");
	for (i = 0; i < N; i++)
		printf(" %d", s[i]);
	printf("\n");
}
