/* Plot $\psi$ for a range of eigenenergies. */

double energy;

double c[100], psi_at_x;

main()

{

	int i;

	for (i = 0; i < 2000; i++) {

		energy = 0.001 * i;

		try();

	}

}

try()

{

	int i;

	c[0] = 0.0;

	c[1] = 1.0;

	for (i = 2; i < 100; i++)

		c[i] = (-energy * c[i - 2] - c[i - 1]) / i / (i + 1);

	/* compute function psi */

	for (i = 0; i < 100; i++)

		psi(100);

	printf("energy = %lg, psi[100] = %lg\n", energy, psi_at_x);

}

psi(x)

int x;

{

	int i;

	double xx;

	xx = 1.0;

	psi_at_x = 0.0;

	for (i = 0; i < 100; i++) {

		psi_at_x += c[i] * xx;

		xx *= x;

	}

}

/*

plot()

{

	int a, x, y;

	for (y = 21; y > 0; y--) {

		for (x = 0; x < 78; x++) {

			a = 20.0 * psi[x] + 0.5;

			if (y > a && a >= y - 1)

				printf("*");

			else

				printf(".");

		}

		printf("\n");

	}

}

scale()

{

	int i;

	double a, min, max;

	min = psi[0];

	max = psi[0];

	for (i = 1; i < 100; i++) {

		if (psi[i] < min)

			min = psi[i];

		if (psi[i] > max)

			max = psi[i];

	}

	if (min == max)

		a = 1.0;

	else

		a = 1.0 / (max - min);

	for (i = 0; i < 100; i++)

		psi[i] = a * (psi[i] - min);

}

*/
