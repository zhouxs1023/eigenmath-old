/* Example program that determines if a matrix forms a group. Nov. 15, 2002 */

#define N 3

int t[N][N] = {
	0, 1, 2,
	1, 2, 0,
	2, 0, 1,
};

int u;

main()
{
	if (p1() == 0)
		printf("not associative\n");
	if (p2() == 0)
		printf("no identity element\n");
	else if (p3() == 0)
		printf("no inverse\n");
}

/* associative? */

p1()
{
	int a, b, c;
	for (a = 0; a < N; a++)
		for (b = 0; b < N; b++)
			for (c = 0; c < N; c++)
				if (t[t[a][b]][c] != t[a][t[b][c]])
					return 0;
	return 1;
}

/* existence of identity element? */

p2()
{
	int a;
	for (u = 0; u < N; u++) {
		for (a = 0; a < N; a++)
			if (t[a][u] == a && t[u][a] == a)
				continue;
			else
				break;
		if (a == N)
			return 1;
	}
	return 0;
}

/* existence of inverse? */

p3()
{
	int a, b;
	for (a = 0; a < N; a++) {
		for (b = 0; b < N; b++)
			if (t[a][b] == u && t[b][a] == u)
				break;
		if (b == N)
			return 0;
	}
	return 1;
}
