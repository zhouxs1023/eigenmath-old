/* Enhanced program for deriving $R_2$ using the cross product.		*/
/* When $\vec E$ and $\vec B$ are linearly dependent			*/
/* the cross product is zero.  This is because $\vec E$ and		*/
/* $\vec B$ do not construct a plane.  They do, however,		*/
/* construct a line which can be tilted onto the $x^2$-$x^3$ plane.	*/
/* This program detects a zero cross product and generates		*/
/* the following rotation which eliminates $E^1$:			*/
/* $$\displaylines{							*/
/* R_2 = \left [ \matrix {						*/
/* \cos\theta & -\sin\theta & 0 & 0 \cr					*/
/* \sin\theta & \cos\theta & 0 & 0 \cr					*/
/* 0 & 0 & 1 & 0 \cr							*/
/* 0 & 0 & 0 & 1 } \right ]\hfill\cr					*/
/* \hfill= \left [ \matrix {						*/
/* E^2\over \sqrt{(E^1)^2+(E^2)^2} & -E^1\over \sqrt{(E^1)^2+(E^2)^2}	*/
/* & 0 & 0 \cr								*/
/* E^1\over \sqrt{(E^1)^2+(E^2)^2} & -E^2\over \sqrt{(E^1)^2+(E^2)^2}	*/
/* & 0 & 0 \cr								*/
/* 0 & 0 & 1 & 0 \cr 0 & 0 & 0 & 1 } \right ]\cr} $$			*/
/* To see this start with a right triangle with height $E^1$		*/
/* and base $E^2$.  Then a rotation of $\theta$ makes			*/
/* the height $E^1=0$. From the right triangle we get			*/
/* $\cos\theta={\rm adjacent \over hypotenuse}				*/
/* ={E^2\over \sqrt{(E^1)^2+(E^2)^2}}$					*/
/* and									*/
/* $\sin\theta={\rm opposite \over hypotenuse}				*/
/* ={E^1\over \sqrt{(E^1)^2+(E^2)^2}}$.					*/

#include <stdio.h>
#include <math.h>

double E1 = 1.0;
double E2 = 2.0;
double E3 = 3.0;

double B1 = -1.0;
double B2 = -2.0;
double B3 = -3.0;

double F11, F12, F13, F14;
double F21, F22, F23, F24;
double F31, F32, F33, F34;
double F41, F42, F43, F44;

double R2_11, R2_12, R2_13, R2_14;
double R2_21, R2_22, R2_23, R2_24;
double R2_31, R2_32, R2_33, R2_34;
double R2_41, R2_42, R2_43, R2_44;

double a11, a12, a13, a14;
double a21, a22, a23, a24;
double a31, a32, a33, a34;
double a41, a42, a43, a44;

double b11, b12, b13, b14;
double b21, b22, b23, b24;
double b31, b32, b33, b34;
double b41, b42, b43, b44;

main()
{
	F11 = 0.0;	F12 = B3;	F13 = -B2;	F14 = E1;

	F21 = -B3;	F22 = 0.0;	F23 = B1;	F24 = E2;

	F31 = B2;	F32 = -B1;	F33 = 0.0;	F34 = E3;

	F41 = E1;	F42 = E2;	F43 = E3;	F44 = 0.0;

	DeriveR2FromF();

	/* R2 F */

	a11 = R2_11 * F11 + R2_12 * F21 + R2_13 * F31 + R2_14 * F41;
	a12 = R2_11 * F12 + R2_12 * F22 + R2_13 * F32 + R2_14 * F42;
	a13 = R2_11 * F13 + R2_12 * F23 + R2_13 * F33 + R2_14 * F43;
	a14 = R2_11 * F14 + R2_12 * F24 + R2_13 * F34 + R2_14 * F44;

	a21 = R2_21 * F11 + R2_22 * F21 + R2_23 * F31 + R2_24 * F41;
	a22 = R2_21 * F12 + R2_22 * F22 + R2_23 * F32 + R2_24 * F42;
	a23 = R2_21 * F13 + R2_22 * F23 + R2_23 * F33 + R2_24 * F43;
	a24 = R2_21 * F14 + R2_22 * F24 + R2_23 * F34 + R2_24 * F44;

	a31 = R2_31 * F11 + R2_32 * F21 + R2_33 * F31 + R2_34 * F41;
	a32 = R2_31 * F12 + R2_32 * F22 + R2_33 * F32 + R2_34 * F42;
	a33 = R2_31 * F13 + R2_32 * F23 + R2_33 * F33 + R2_34 * F43;
	a34 = R2_31 * F14 + R2_32 * F24 + R2_33 * F34 + R2_34 * F44;

	a41 = R2_41 * F11 + R2_42 * F21 + R2_43 * F31 + R2_44 * F41;
	a42 = R2_41 * F12 + R2_42 * F22 + R2_43 * F32 + R2_44 * F42;
	a43 = R2_41 * F13 + R2_42 * F23 + R2_43 * F33 + R2_44 * F43;
	a44 = R2_41 * F14 + R2_42 * F24 + R2_43 * F34 + R2_44 * F44;

	/* R2 F R2^-1 */

	b11 = a11 * R2_11 + a12 * R2_12 + a13 * R2_13 - a14 * R2_14;
	b12 = a11 * R2_21 + a12 * R2_22 + a13 * R2_23 - a14 * R2_24;
	b13 = a11 * R2_31 + a12 * R2_32 + a13 * R2_33 - a14 * R2_34;
	b14 = -a11 * R2_41 - a12 * R2_42 - a13 * R2_43 + a14 * R2_44;

	b21 = a21 * R2_11 + a22 * R2_12 + a23 * R2_13 - a24 * R2_14;
	b22 = a21 * R2_21 + a22 * R2_22 + a23 * R2_23 - a24 * R2_24;
	b23 = a21 * R2_31 + a22 * R2_32 + a23 * R2_33 - a24 * R2_34;
	b24 = -a21 * R2_41 - a22 * R2_42 - a23 * R2_43 + a24 * R2_44;

	b31 = a31 * R2_11 + a32 * R2_12 + a33 * R2_13 - a34 * R2_14;
	b32 = a31 * R2_21 + a32 * R2_22 + a33 * R2_23 - a34 * R2_24;
	b33 = a31 * R2_31 + a32 * R2_32 + a33 * R2_33 - a34 * R2_34;
	b34 = -a31 * R2_41 - a32 * R2_42 - a33 * R2_43 + a34 * R2_44;

	b41 = a41 * R2_11 + a42 * R2_12 + a43 * R2_13 - a44 * R2_14;
	b42 = a41 * R2_21 + a42 * R2_22 + a43 * R2_23 - a44 * R2_24;
	b43 = a41 * R2_31 + a42 * R2_32 + a43 * R2_33 - a44 * R2_34;
	b44 = -a41 * R2_41 - a42 * R2_42 - a43 * R2_43 + a44 * R2_44;

	if (fabs(b11) < 1e-6) b11 = 0.0;
	if (fabs(b12) < 1e-6) b12 = 0.0;
	if (fabs(b13) < 1e-6) b13 = 0.0;
	if (fabs(b14) < 1e-6) b14 = 0.0;

	if (fabs(b21) < 1e-6) b21 = 0.0;
	if (fabs(b22) < 1e-6) b22 = 0.0;
	if (fabs(b23) < 1e-6) b23 = 0.0;
	if (fabs(b24) < 1e-6) b24 = 0.0;

	if (fabs(b31) < 1e-6) b31 = 0.0;
	if (fabs(b32) < 1e-6) b32 = 0.0;
	if (fabs(b33) < 1e-6) b33 = 0.0;
	if (fabs(b34) < 1e-6) b34 = 0.0;

	if (fabs(b41) < 1e-6) b41 = 0.0;
	if (fabs(b42) < 1e-6) b42 = 0.0;
	if (fabs(b43) < 1e-6) b43 = 0.0;
	if (fabs(b44) < 1e-6) b44 = 0.0;

	printf(" $$ F = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", F11, F12, F13, F14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F21, F22, F23, F24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F31, F32, F33, F34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F41, F42, F43, F44);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_2 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_11, R2_12, R2_13, R2_14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_21, R2_22, R2_23, R2_24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_31, R2_32, R2_33, R2_34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_41, R2_42, R2_43, R2_44);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_2 F R_2^{-1} = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", b11, b12, b13, b14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b21, b22, b23, b24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b31, b32, b33, b34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b41, b42, b43, b44);
	printf(" } \\right ] $$ \n\n");
}

DeriveR2FromF()
{
	double a, b, c;
	double e1, e2, e3;
	double b1, b2, b3;

	e1 = F41;
	e2 = F42;
	e3 = F43;

	b1 = F23;
	b2 = F31;
	b3 = F12;

	a = e2 * b3 - e3 * b2;
	b = e3 * b1 - e1 * b3;
	c = e1 * b2 - e2 * b1;

	if (a * a + b * b + c * c < 1e-6) {

		/* cross product is zero */

		/* E and B are linearly dependent (on the same line) */

		/* rotate E to the x2-x3 plane */

		R2_11 = e2 / sqrt(e1 * e1 + e2 * e2);
		R2_12 = -e1 / sqrt(e1 * e1 + e2 * e2);
		R2_13 = 0.0;
		R2_14 = 0.0;

		R2_21 = e1 / sqrt(e1 * e1 + e2 * e2);
		R2_22 = e2 / sqrt(e1 * e1 + e2 * e2);
		R2_23 = 0.0;
		R2_24 = 0.0;

		R2_31 = 0.0;
		R2_32 = 0.0;
		R2_33 = 1.0;
		R2_34 = 0.0;

		R2_41 = 0.0;
		R2_42 = 0.0;
		R2_43 = 0.0;
		R2_44 = 1.0;

	} else {

		/* cross product is nonzero */

		/* rotate normal vector to align with x1 */

		R2_11 = a / sqrt(a * a + b * b + c * c);
		R2_12 = b / sqrt(a * a + b * b + c * c);
		R2_13 = c / sqrt(a * a + b * b + c * c);
		R2_14 = 0.0;

		R2_21 = -sqrt(b * b + c * c) / sqrt(a * a + b * b + c * c);
		R2_22 = a * b / sqrt(b * b + c * c) / sqrt(a * a + b * b + c * c);
		R2_23 = a * c / sqrt(b * b + c * c) / sqrt(a * a + b * b + c * c);
		R2_24 = 0.0;

		R2_31 = 0.0;
		R2_32 = -c / sqrt(b * b + c * c);
		R2_33 = b / sqrt(b * b + c * c);
		R2_34 = 0.0;

		R2_41 = 0.0;
		R2_42 = 0.0;
		R2_43 = 0.0;
		R2_44 = 1.0;
	}

	if (fabs(R2_11) < 1e-6) R2_11 = 0.0;
	if (fabs(R2_12) < 1e-6) R2_12 = 0.0;
	if (fabs(R2_13) < 1e-6) R2_13 = 0.0;
	if (fabs(R2_14) < 1e-6) R2_14 = 0.0;

	if (fabs(R2_21) < 1e-6) R2_21 = 0.0;
	if (fabs(R2_22) < 1e-6) R2_22 = 0.0;
	if (fabs(R2_23) < 1e-6) R2_23 = 0.0;
	if (fabs(R2_24) < 1e-6) R2_24 = 0.0;

	if (fabs(R2_31) < 1e-6) R2_31 = 0.0;
	if (fabs(R2_32) < 1e-6) R2_32 = 0.0;
	if (fabs(R2_33) < 1e-6) R2_33 = 0.0;
	if (fabs(R2_34) < 1e-6) R2_34 = 0.0;

	if (fabs(R2_41) < 1e-6) R2_41 = 0.0;
	if (fabs(R2_42) < 1e-6) R2_42 = 0.0;
	if (fabs(R2_43) < 1e-6) R2_43 = 0.0;
	if (fabs(R2_44) < 1e-6) R2_44 = 0.0;
}
