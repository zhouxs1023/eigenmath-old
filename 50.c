/* Yet another version of $R_2$.  There are additional special cases.
If $\vec E$ and $\vec B$ are linearly dependent then use the plane
that contains $\vec E$ and $\vec B$ and is perpendicular to the
$x^2$-$x^3$ plane (contains the $x^1$ axis.)  Therefore the point
$P=(0,E^2,E^3)$ is a point in the intersection.  Another special case
arises: $E^2=E^3=0$ ($\vec E$ is zero or lies on the $x^1$ axis.)
Then use $P=(0,B^2,B^3)$.  Another special case arises:
$B^2=B^3=0$ ($\vec B$ lies on the $x^1$ axis.)  In this case both
$\vec E$ and $\vec B$ lie on the $x^1$ axis so no $\phi$
rotation is required.  Use $P=(0,0,1)$. */

#include <stdio.h>
#include <math.h>

double F11, F12, F13, F14;
double F21, F22, F23, F24;
double F31, F32, F33, F34;
double F41, F42, F43, F44;

double R2_11, R2_12, R2_13, R2_14;
double R2_21, R2_22, R2_23, R2_24;
double R2_31, R2_32, R2_33, R2_34;
double R2_41, R2_42, R2_43, R2_44;

main()
{
	int n = 0;
	int e1, e2, e3;
	int b1, b2, b3;

	for (e1 = -2; e1 <= 2; e1++)
	for (e2 = -2; e2 <= 2; e2++)
	for (e3 = -2; e3 <= 2; e3++)

	for (b1 = -2; b1 <= 2; b1++)
	for (b2 = -2; b2 <= 2; b2++)
	for (b3 = -2; b3 <= 2; b3++) {

		F11 = 0.0;	F12 = b3;	F13 = -b2;	F14 = e1;
		F21 = -b3;	F22 = 0.0;	F23 = b1;	F24 = e2;
		F31 = b2;	F32 = -b1;	F33 = 0.0;	F34 = e3;
		F41 = e1;	F42 = e2;	F43 = e3;	F44 = 0.0;

		DeriveR2FromF();

		check();

		if (++n == 10) {
			n = 0;
			printf(".");
		}

		if (kbhit())
			exit();
	}
}

check()
{
	double x;

	double b1, b2, b3;
	double e1, e2, e3;
	double w1, w2, w3;
	double x1, x2, x3;
	double y1, y2, y3;
	double z1, z2, z3;

	e1 = F41;
	e2 = F42;
	e3 = F43;

	b1 = F23;
	b2 = F31;
	b3 = F12;

	x = R2_11 * e1 + R2_12 * e2 + R2_13 * e3;

	if (fabs(x) > 1e-6)
		fail("does not eliminate E1");

	x = R2_11 * b1 + R2_12 * b2 + R2_13 * b3;

	if (fabs(x) > 1e-6)
		fail("does not eliminate B1");

	/* e1 x e2 . e3 */

	x1 = R2_11;
	x2 = R2_21;
	x3 = R2_31;

	y1 = R2_12;
	y2 = R2_22;
	y3 = R2_32;

	z1 = R2_13;
	z2 = R2_23;
	z3 = R2_33;

	w1 = x2 * y3 - x3 * y2;
	w2 = x3 * y1 - x1 * y3;
	w3 = x1 * y2 - x2 * y1;

	x = w1 * z1 + w2 * z2 + w3 * z3;

	if (fabs(x - 1.0) > 1e-6)
		fail("not right-handed");
}

fail(s)
char *s;
{
	double b1, b2, b3;
	double e1, e2, e3;

	e1 = F41;
	e2 = F42;
	e3 = F43;

	b1 = F23;
	b2 = F31;
	b3 = F12;

	printf("$s\n", s);

	printf("E=(%d,%d,%d)\n", e1, e2, e3);
	printf("B=(%d,%d,%d)\n", b1, b2, b3);

	exit();
}

DeriveR2FromF()
{
	double h, p1, p2, p3;

	double e1, e2, e3;
	double b1, b2, b3;

	double cosphi, sinphi;
	double costheta, sintheta;

	e1 = F41;	e2 = F42;	e3 = F43;
	b1 = F23;	b2 = F31;	b3 = F12;

	p2 = e2 * b1 - e1 * b2;
	p3 = e3 * b1 - e1 * b3;

	/* special case */

	/* E and B are linearly dependent, i.e. B == k E */

	/* p2 = e2 * k * e1 - e1 * k * e2 == 0 */

	/* p3 = e3 * k * e1 - e1 * k * e3 == 0 */

	if (fabs(p2) < 1e-6 && fabs(p3) < 1e-6) {

		p2 = e2;
		p3 = e3;

		/* special case */

		/* E lies on the x1 axis (or is zero) */

		if (p2 == 0.0 && p3 == 0.0) {

			p2 = b2;
			p3 = b3;

			/* special case */

			/* B lies on the x1 axis (or is zero) */

			if (p2 == 0.0 && p3 == 0.0) {

				/* no rotation */

				p2 = 0;
				p3 = 1.0;
			}
		}
	}

	h = sqrt(p2 * p2 + p3 * p3);

	cosphi = p3 / h;
	sinphi = p2 / h;

	p1 = e1;
	p2 = cosphi * e2 - sinphi * e3;

	/* special case */

	/* rotated E lies on the x^3 axis (or is zero) */

	if (fabs(p1) < 1e-6 && fabs(p2) < 1e-6) {

		p1 = b1;
		p2 = cosphi * b2 - sinphi * b3;

		/* special case */

		/* rotated B lies on the x^3 axis (or is zero) */

		if (fabs(p1) < 1e-6 && fabs(p2) < 1e-6) {

			/* no rotation */

			p1 = 0.0;
			p2 = 1.0;
		}
	}

	costheta = p2 / sqrt(p1 * p1 + p2 * p2);
	sintheta = p1 / sqrt(p1 * p1 + p2 * p2);

	R2_11 = costheta;
	R2_12 = -cosphi * sintheta;
	R2_13 = sinphi * sintheta;
	R2_14 = 0.0;

	R2_21 = cosphi * sintheta;
	R2_22 = cosphi * cosphi * costheta + sinphi * sinphi;
	R2_23 = -cosphi * sinphi * costheta + cosphi * sinphi;
	R2_24 = 0.0;

	R2_31 = -sinphi * sintheta;
	R2_32 = -cosphi * sinphi * costheta + cosphi * sinphi;
	R2_33 = sinphi * sinphi * costheta + cosphi * cosphi;
	R2_34 = 0.0;

	R2_41 = 0.0;
	R2_42 = 0.0;
	R2_43 = 0.0;
	R2_44 = 1.0;

	if (fabs(R2_11) < 1e-6) R2_11 = 0.0;
	if (fabs(R2_12) < 1e-6) R2_12 = 0.0;
	if (fabs(R2_13) < 1e-6) R2_13 = 0.0;

	if (fabs(R2_21) < 1e-6) R2_21 = 0.0;
	if (fabs(R2_22) < 1e-6) R2_22 = 0.0;
	if (fabs(R2_23) < 1e-6) R2_23 = 0.0;

	if (fabs(R2_31) < 1e-6) R2_31 = 0.0;
	if (fabs(R2_32) < 1e-6) R2_32 = 0.0;
	if (fabs(R2_33) < 1e-6) R2_33 = 0.0;
}
