/* Compare the coefficients for $(x+1)^{1000}$ with the
function $0.0255225\,e^{-(500-x)^2/500}$. */

#include <math.h>

#define DEGREE 1000

double coeff[DEGREE + 1];

main()

{

	int i, j;

	double x;

	for (i = 0; i < DEGREE + 1; i++) {

		coeff[i] = 1.0;

		for (j = i - 1; j > 0; j--)

			coeff[j] += coeff[j - 1];

	}

	/* normalize */

/*	x = coeff[DEGREE / 2];

	for (i = 0; i < DEGREE + 1; i++)

		coeff[i] /= x;
*/
	x = 0.0;

	for (i = 0; i < DEGREE + 1; i++)

		x += coeff[i];

	for (i = 0; i < DEGREE + 1; i++)

		coeff[i] /= x;

	for (i = 0; i < DEGREE + 1; i++) {

		x = (double) i - 500.0;

		x = 0.025225 * exp(-x * x / 500.0);

		printf("coeff=%lg, approx=%lg, err=%lg\n", coeff[i], x, (coeff[i] - x) / coeff[i]);

	}

}
