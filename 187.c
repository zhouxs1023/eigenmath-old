/* Dijkstra shortest path algorithm demo */

#define YA 1
#define YB 2
#define YC 3
#define YD 4
#define YE 5
#define YF 6
#define YG 7
#define YH 8

#define N 8

#define M 1000000

int n, A[100][100], d[100], s[100], IN[100];

int x = 1, y = 8;

char *city[9] = {
	"",
	"New York",
	"Cleveland",
	"St. Louis",
	"Nashville",
	"Phoenix",
	"Dallas",
	"Salt Lake",
	"Los Angeles",
};

main()
{
	int i, j, p, z, OldDistance, iteration = 0;

	for (i = 0; i < 100; i++)
		for (j = 0; j < 100; j++)
			A[i][j] = M;

	A[1][2] = 400;
	A[1][3] = 900;
	A[1][4] = 800;

	A[2][5] = 1800;
	A[2][6] = 900;

	A[3][5] = 1100;
	A[3][6] = 600;

	A[4][6] = 600;
	A[4][7] = 1200;

	A[5][8] = 400;
	A[5][6] = 900;

	A[6][8] = 1300;
	A[6][7] = 1000;

	A[7][8] = 600;

	IN[1] = x;
	n = 1;
	d[x] = 0;

	for (z = 1; z <= N; z++)
		if (!in(z)) {
			d[z] = A[x][z];
			s[z] = x;
		}

	while (!in(y)) {

printf("Iteration %d\n\n", ++iteration);

for (i = 2; i <= 8; i++)
	if (d[i] < M) {
		printf("%5d ", d[i]);
		print_path(i);
		printf("\n");
	}
printf("\n");

		p = minz();

printf("The least cost is %s, make it [permanent]\n\n", city[p]);

//		printf("\np = %d\n", p);

		if (p == 0) {
			printf("No path.\n");
			exit(1);
		}

		IN[++n] = p;
		for (z = 1; z <= N; z++)
			if (!in(z)) {
				OldDistance = d[z];
				d[z] = min(d[z], d[p] + A[p][z]);
				if (d[z] != OldDistance)
					s[z] = p;
			}

//		print_state();
	}
#if 0
	printf("In reverse order, the path is\n");
	printf("%d\n", y);
	z = y;
	do {
		printf("%d\n", s[z]);
		z = s[z];
	} while (z != x);
#endif
}

print_path(int n)
{
	if (n == 1)
		printf("[New York]");
	else {
		print_path(s[n]);
		if (in(n))
			printf("  - [%s]", city[n]);
		else
			printf(" - %s", city[n]);
	}
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
	w = M;
	p = 0;
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
	for (i = 1; i <= n; i++)
		printf(" %d", IN[i]);
	printf("\n");

	printf("d =");
	for (i = 1; i <= N; i++)
		printf(" %d", d[i]);
	printf("\n");

	printf("s =");
	for (i = 1; i <= N; i++)
		printf(" %d", s[i]);
	printf("\n");
}
