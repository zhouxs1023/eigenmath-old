/* How many ways are there to get to each end point
using a ``step by step'' algorithm? */

#define N 10

long n[N + 1];

main()

{

	int i;

	f(0, 0);

	for (i = 0; i < N + 1; i++)

		printf("x = %d, n[x] = %ld\n", i, n[i]);

}

f(x, t)

int x, t;

{

	if (x + t == N)

		n[x]++;

	else {

		f(x + 1, t);

		f(x, t + 1);

	}

}
