/* Generate discrete $\psi_{100}(r)$, $\psi_{200}(r)$ and $\psi_{300}(r)$.
The number of samples is 1000.
The distance between samples is $dr=10^{-12}\rm\,m$. */

#include <stdio.h>

#include <math.h>

float pi = M_PI;

main()

{

	int n;

	FILE *f;

	float psi, r;

	float a0 = 5.2917715e-11;

	float dr = 1e-12;

	f = fopen("psi100", "w");

	for (n = 0; n < 1000; n++) {

		r = n * dr;

		psi = 1.0 / sqrt(pi)

			* pow(a0, -1.5)

			* exp(-r / a0);

		fprintf(f, "%+1.7e\n", psi);

	}

	fclose(f);

	f = fopen("psi200", "w");

	for (n = 0; n < 1000; n++) {

		r = n * dr;

		psi = 1.0 / 4.0 / sqrt(2.0 * pi)

			* pow(a0, -1.5)

			* (2.0 - r / a0)

			* exp(-r / 2.0 / a0);

		fprintf(f, "%+1.7e\n", psi);

	}

	fclose(f);

	f = fopen("psi300", "w");

	for (n = 0; n < 1000; n++) {

		r = n * dr;

		psi = 1.0 / 81.0 / sqrt(3.0 * pi)

			* pow(a0, -1.5)

			* (27.0 - 18.0 * r / a0 + 2.0 * r * r / a0 / a0)

			* exp(-r / 3.0 / a0);

		fprintf(f, "%+1.7e\n", psi);

	}

	fclose(f);

}
