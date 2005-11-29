/* Experiment using Euler angles.  It hasn't worked out yet. */

#include <stdio.h>
#include <math.h>

double cosphi1, sinphi1;
double costheta, sintheta;
double cosphi2, sinphi2;

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

	cosphi1 = x2 / sqrt(x1 * x1 + x2 * x2);
	sinphi1 = x1 / sqrt(x1 * x1 + x2 * x2);

	costheta = 1.0;
	sintheta = 0.0;

	cosphi2 = 1.0;
	sinphi2 = 0.0;

	rotate();

	/* eliminate x^2 */

	cosphi1 = x1 / sqrt(x1 * x1 + x2 * x2);
	sinphi1 = -x2 / sqrt(x1 * x1 + x2 * x2);

	costheta = 1.0;
	sintheta = 0.0;

	cosphi2 = 1.0;
	sinphi2 = 0.0;

	rotate();

	/* eliminate x^1 */

	cosphi1 = 1.0;
	sinphi1 = 0.0;

	costheta = 1.0;
	sintheta = 0.0;

	cosphi2 = x2 / sqrt(x1 * x1 + x2 * x2);
	sinphi2 = x1 / sqrt(x1 * x1 + x2 * x2);

	rotate();

	/* eliminate x^2 */

	cosphi1 = 1.0;
	sinphi1 = 0.0;

	costheta = 1.0;
	sintheta = 0.0;

	cosphi2 = x1 / sqrt(x1 * x1 + x2 * x2);
	sinphi2 = -x2 / sqrt(x1 * x1 + x2 * x2);

	rotate();

	/* eliminate x^2 */

	cosphi1 = 1.0;
	sinphi1 = 0.0;

	costheta = x3 / sqrt(x2 * x2 + x3 * x3);
	sintheta = x2 / sqrt(x2 * x2 + x3 * x3);

	cosphi2 = 1.0;
	sinphi2 = 0.0;

	rotate();

	/* eliminate x^3 */

	cosphi1 = 1.0;
	sinphi1 = 0.0;

	costheta = x2 / sqrt(x2 * x2 + x3 * x3);
	sintheta = -x3 / sqrt(x2 * x2 + x3 * x3);

	cosphi2 = 1.0;
	sinphi2 = 0.0;

	rotate();
}

rotate()
{
	R11 = cosphi2 * cosphi1 - costheta * sinphi1 * sinphi2;
	R12 = -cosphi2 * sinphi1 - costheta * cosphi1 * sinphi2;
	R13 = sinphi2 * sintheta;

	R21 = sinphi2 * cosphi1 + costheta * sinphi1 * cosphi2;
	R22 = -sinphi2 * sinphi1 + costheta * cosphi1 * cosphi2;
	R23 = -cosphi2 * sintheta;

	R31 = sintheta * sinphi1;
	R32 = sintheta * cosphi1;
	R33 = costheta;

	y1 = R11 * x1 + R12 * x2 + R13 * x3;
	y2 = R21 * x1 + R22 * x2 + R23 * x3;
	y3 = R31 * x1 + R32 * x2 + R33 * x3;

	if (fabs(y1) < 1e-6) y1 = 0.0;
	if (fabs(y2) < 1e-6) y2 = 0.0;
	if (fabs(y3) < 1e-6) y3 = 0.0;

	printf("$$ Rx = \\left [ \\matrix { \n");
	printf("%g \\cr \n", y1);
	printf("%g \\cr \n", y2);
	printf("%g \\cr \n", y3);
	printf("} \\right ] $$ \n");
}
