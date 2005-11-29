/* Compute a tensor product showing all combinations of
up and down indices. */

int u[4] = {1, 1, 1, 1};

int v[4] = {1, 1, 1, 1};

main()

{

	printf("\\magnification=1200\n");

	u[3] = 1;

	v[3] = 1;

	f("[T^{ab}]");

	u[3] = 1;

	v[3] = -1;

	f("[{T^a}_b]");

	u[3] = -1;

	v[3] = 1;

	f("[{T_a}^b]");

	u[3] = -1;

	v[3] = -1;

	f("[T_{ab}]");

	printf("\\end\n");

}

f(s)

char *s;

{

	int i, j;

	printf("$$ %s = \\left [ \\matrix {\n", s);

	for (i = 0; i < 4; i++) {

		for (j = 0; j < 3; j++)

			printf("%d & ", u[i] * v[j]);

		printf("%d \\cr\n", u[i] * v[j]);

	}

	printf("} \\right ] $$\n");

}
