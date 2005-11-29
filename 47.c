/* Generate a rotation that eliminates $x^1$ and maintains the */
/* ratio of $x^2$ and $x^3$. */
/* In other words it does not induce a rotation about the $x^1$ */
/* axis. */
/* Based on program 46. */

#include <stdio.h>
#include <math.h>

double cosphi, sinphi;
double costheta, sintheta;

double x1, x2, x3;
double y1, y2, y3;

double R11, R12, R13;
double R21, R22, R23;
double R31, R32, R33;

main()
{
	x1 = 1.0;
	x2 = 2.0;
	x3 = 3.0;

	/* eliminate x^1 */

	cosphi = x3 / sqrt(x2 * x2 + x3 * x3);
	sinphi = x2 / sqrt(x2 * x2 + x3 * x3);

	costheta = sqrt(x2 * x2 + x3 * x3) / sqrt(x1 * x1 + x2 * x2 + x3 * x3);
	sintheta = x1 / sqrt(x1 * x1 + x2 * x2 + x3 * x3);

	rotate();
}

rotate()
{
	R11 = costheta;
	R12 = -sintheta * sinphi;
	R13 = -sintheta * cosphi;

	R21 = sintheta * sinphi;
	R22 = costheta * sinphi * sinphi + cosphi * cosphi;
	R23 = costheta * cosphi * sinphi - cosphi * sinphi;

	R31 = sintheta * cosphi;
	R32 = costheta * cosphi * sinphi - cosphi * sinphi;
	R33 = costheta * cosphi * cosphi + sinphi * sinphi;

	y1 = R11 * x1 + R12 * x2 + R13 * x3;
	y2 = R21 * x1 + R22 * x2 + R23 * x3;
	y3 = R31 * x1 + R32 * x2 + R33 * x3;

	if (fabs(y1) < 1e-6) y1 = 0.0;
	if (fabs(y2) < 1e-6) y2 = 0.0;
	if (fabs(y3) < 1e-6) y3 = 0.0;

	printf("$$ Rx = \\left [ \\matrix { \n");
	printf("%g & %g & %g \\cr \n", R11, R12, R13);
	printf("%g & %g & %g \\cr \n", R21, R22, R23);
	printf("%g & %g & %g \\cr \n", R31, R32, R33);
	printf("} \\right ] \\left [ \\matrix { \n");
	printf("%g \\cr \n", x1);
	printf("%g \\cr \n", x2);
	printf("%g \\cr \n", x3);
	printf("} \\right ] = \\left [ \\matrix { \n");
	printf("%g \\cr \n", y1);
	printf("%g \\cr \n", y2);
	printf("%g \\cr \n", y3);
	printf("} \\right ] $$ \n");
}
