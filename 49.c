/* Test the algorithm for computing $R_2$ (see program 48.)
$R_2$ is computed for several
thousand combinations of $\vec E$ and $\vec B$.  For each combination
$R_2$ is verified to use a right-handed basis.  That is, $e_1 \times
e_2 \cdot e_3 = 1$.  The new basis vectors are the columns in $R_2$.
This can be seen by multiplying $R_2$ and an unrotated basis vector.
$$ \left [ \matrix {
A & B & C \cr
A & B & C \cr
A & B & C \cr
} \right ] \left [ \matrix {
1 \cr
0 \cr
0 \cr
} \right ] = \left [ \matrix {
A \cr
A \cr
A \cr
} \right ] $$
This program uncovered two special cases for computing $R_2$.
First, the second and third elements of the cross product of
$\vec E$ and $\vec B$ cannot be zero.  If they are then
$\vec E$ and $\vec B$ are already on the $x^2$-$x^3$ plane
and no rotation is required.  If this case is not handled
a divide by zero error occurs.  Second, if $\vec E$ is on
on the line of intersection then a divide by zero error occurs
when computing $\theta.$
A test must be included to use $\vec B$ to compute $\theta$
when $E^1=0$. */

#include <stdio.h>
#include <math.h>

double cosphi, sinphi;
double costheta, sintheta;

double E1, E2, E3;
double B1, B2, B3;

double R11, R12, R13;
double R21, R22, R23;
double R31, R32, R33;

main()
{
	int n = 0;
	int e1, e2, e3;
	int b1, b2, b3;
	int x2, x3;

	for (e1 = -2; e1 <= 2; e1++)
	for (e2 = -2; e2 <= 2; e2++)
	for (e3 = -2; e3 <= 2; e3++)

	for (b1 = -2; b1 <= 2; b1++)
	for (b2 = -2; b2 <= 2; b2++)
	for (b3 = -2; b3 <= 2; b3++) {

		/* check cross product */

		x2 = e3 * b1 - e1 * b3;
		x3 = e1 * b2 - e2 * b1;

		if (x2 == 0 && x3 == 0)
			continue;

		E1 = e1;
		E2 = e2;
		E3 = e3;

		B1 = b1;
		B2 = b2;
		B3 = b3;

		check();

		if (++n == 10) {
			n = 0;
			printf(".");
		}
	}
}

check()
{
	double h, p1, p2, p3;

	double w1, w2, w3;
	double x1, x2, x3;
	double y1, y2, y3;
	double z1, z2, z3;

	p2 = E2 * B1 - E1 * B2;
	p3 = E3 * B1 - E1 * B3;

	h = sqrt(p2 * p2 + p3 * p3);

	cosphi = p3 / h;
	sinphi = p2 / h;

	if (E1 == 0.0) {
		p1 = B1;
		p2 = cosphi * B2 - sinphi * B3;
	} else {
		p1 = E1;
		p2 = cosphi * E2 - sinphi * E3;
	}

	costheta = p2 / sqrt(p1 * p1 + p2 * p2);
	sintheta = p1 / sqrt(p1 * p1 + p2 * p2);

	R11 = costheta;
	R12 = -cosphi * sintheta;
	R13 = sinphi * sintheta;

	R21 = cosphi * sintheta;
	R22 = cosphi * cosphi * costheta + sinphi * sinphi;
	R23 = -cosphi * sinphi * costheta + cosphi * sinphi;

	R31 = -sinphi * sintheta;
	R32 = -cosphi * sinphi * costheta + cosphi * sinphi;
	R33 = sinphi * sinphi * costheta + cosphi * cosphi;

	if (fabs(R11) < 1e-6) R11 = 0.0;
	if (fabs(R12) < 1e-6) R12 = 0.0;
	if (fabs(R13) < 1e-6) R13 = 0.0;

	if (fabs(R21) < 1e-6) R21 = 0.0;
	if (fabs(R22) < 1e-6) R22 = 0.0;
	if (fabs(R23) < 1e-6) R23 = 0.0;

	if (fabs(R31) < 1e-6) R31 = 0.0;
	if (fabs(R32) < 1e-6) R32 = 0.0;
	if (fabs(R33) < 1e-6) R33 = 0.0;

	/* e1 x e2 . e3 */

	x1 = R11;
	x2 = R21;
	x3 = R31;

	y1 = R12;
	y2 = R22;
	y3 = R32;

	z1 = R13;
	z2 = R23;
	z3 = R33;

	w1 = x2 * y3 - x3 * y2;
	w2 = x3 * y1 - x1 * y3;
	w3 = x1 * y2 - x2 * y1;

	h = w1 * z1 + w2 * z2 + w3 * z3;

	if (fabs(h - 1.0) > 1e-6) {
		printf("E=(%g,%g,%g)\n", E1, E2, E3);
		printf("B=(%g,%g,%g)\n", B1, B2, B3);
		printf("e1 x e2 . e3 = %g\n", h);
	}
}
