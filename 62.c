/* Compute $R_2$ the easy way, by eliminating $E^1$ and
$B^1$ with simple rotations. ($E^2$ also gets eliminated
because that is the easiest way to eliminate $B^1$.)
So what if there is some arbitrary rotation about the $x^1$ axis?
It will not affect $\beta$. Sample output:
 $$ F = \left [ \matrix { 
               0 &               6 &              -5 &               1 \cr 
              -6 &               0 &               4 &               2 \cr 
               5 &              -4 &               0 &               3 \cr 
               1 &               2 &               3 &               0 \cr 
 } \right ] $$ 
 $$ R_2 = \left [ \matrix { 
        0.408248 &       -0.816497 &        0.408248 &               0 \cr 
        0.872872 &        0.218218 &       -0.436436 &               0 \cr 
        0.267261 &        0.534522 &        0.801784 &               0 \cr 
               0 &               0 &               0 &               1 \cr 
 } \right ] $$ 
 $$ R_2 F R_2^{-1} = \left [ \matrix { 
               0 &         8.55236 &        -1.96396 &               0 \cr 
        -8.55236 &               0 &               0 &               0 \cr 
         1.96396 &               0 &               0 &         3.74166 \cr 
               0 &               0 &         3.74166 &               0 \cr 
 } \right ] $$ */

#include <stdio.h>
#include <math.h>

double E1 = 1.0;
double E2 = 1.0;
double E3 = 1.0;

double B1 = 0.0;
double B2 = 0.0;
double B3 = 0.0;

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
	F11 = 0.0;	F12 = B3;	F13 = -B2;	F14 = E1;
	F21 = -B3;	F22 = 0.0;	F23 = B1;	F24 = E2;
	F31 = B2;	F32 = -B1;	F33 = 0.0;	F34 = E3;
	F41 = E1;	F42 = E2;	F43 = E3;	F44 = 0.0;

	DeriveR2FromF();

	check();
}

check()
{
	double a11, a12, a13, a14;
	double a21, a22, a23, a24;
	double a31, a32, a33, a34;
	double a41, a42, a43, a44;

	double b11, b12, b13, b14;
	double b21, b22, b23, b24;
	double b31, b32, b33, b34;
	double b41, b42, b43, b44;

	/* R F */

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

	/* R F R^-1 */

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
	printf(" } \\right ] $$ \n");

	printf(" $$ R_2 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_11, R2_12, R2_13, R2_14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_21, R2_22, R2_23, R2_24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_31, R2_32, R2_33, R2_34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R2_41, R2_42, R2_43, R2_44);
	printf(" } \\right ] $$ \n");

	printf(" $$ R_2 F R_2^{-1} = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", b11, b12, b13, b14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b21, b22, b23, b24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b31, b32, b33, b34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b41, b42, b43, b44);
	printf(" } \\right ] $$ \n");
}

DeriveR2FromF()
{
	double e1, e2, e3;
	double b1, b2, b3;
	double t1, t2, t3;

	double cos, sin;

	double a11, a12, a13;
	double a21, a22, a23;
	double a31, a32, a33;

	double r11, r12, r13;
	double r21, r22, r23;
	double r31, r32, r33;

	double t11, t12, t13;
	double t21, t22, t23;
	double t31, t32, t33;

	e1 = F41;	e2 = F42;	e3 = F43;

	b1 = F23;	b2 = F31;	b3 = F12;

	a11 = 1.0;	a12 = 0.0;	a13 = 0.0;
	a21 = 0.0;	a22 = 1.0;	a23 = 0.0;
	a31 = 0.0;	a32 = 0.0;	a33 = 1.0;

	if (e1 == 0.0 && e2 == 0.0)
		;
	else {

		/* r operator */

		cos = e2 / sqrt(e1 * e1 + e2 * e2);
		sin = e1 / sqrt(e1 * e1 + e2 * e2);

		r11 = cos;	r12 = -sin;	r13 = 0.0;
		r21 = sin;	r22 = cos;	r23 = 0.0;
		r31 = 0.0;	r32 = 0.0;	r33 = 1.0;

		/* a = r a */

		t11 = r11 * a11 + r12 * a21 + r13 * a31;
		t12 = r11 * a12 + r12 * a22 + r13 * a32;
		t13 = r11 * a13 + r12 * a23 + r13 * a33;

		t21 = r21 * a11 + r22 * a21 + r23 * a31;
		t22 = r21 * a12 + r22 * a22 + r23 * a32;
		t23 = r21 * a13 + r22 * a23 + r23 * a33;

		t31 = r31 * a11 + r32 * a21 + r33 * a31;
		t32 = r31 * a12 + r32 * a22 + r33 * a32;
		t33 = r31 * a13 + r32 * a23 + r33 * a33;

		a11 = t11;	a12 = t12;	a13 = t13;
		a21 = t21;	a22 = t22;	a23 = t23;
		a31 = t31;	a32 = t32;	a33 = t33;

		/* e = r e */

		t1 = r11 * e1 + r12 * e2 + r13 * e3;
		t2 = r21 * e1 + r22 * e2 + r23 * e3;
		t3 = r31 * e1 + r32 * e2 + r33 * e3;

		e1 = t1;	e2 = t2;	e3 = t3;

		/* b = r b */

		t1 = r11 * b1 + r12 * b2 + r13 * b3;
		t2 = r21 * b1 + r22 * b2 + r23 * b3;
		t3 = r31 * b1 + r32 * b2 + r33 * b3;

		b1 = t1;	b2 = t2;	b3 = t3;
	}

	if (e2 == 0.0 && e3 == 0.0)
		;
	else {

		/* r operator */

		cos = e3 / sqrt(e2 * e2 + e3 * e3);
		sin = e2 / sqrt(e2 * e2 + e3 * e3);

		r11 = 1.0;	r12 = 0.0;	r23 = 0.0;
		r21 = 0.0;	r22 = cos;	r23 = -sin;
		r31 = 0.0;	r32 = sin;	r33 = cos;

		/* a = r a */

		t11 = r11 * a11 + r12 * a21 + r13 * a31;
		t12 = r11 * a12 + r12 * a22 + r13 * a32;
		t13 = r11 * a13 + r12 * a23 + r13 * a33;

		t21 = r21 * a11 + r22 * a21 + r23 * a31;
		t22 = r21 * a12 + r22 * a22 + r23 * a32;
		t23 = r21 * a13 + r22 * a23 + r23 * a33;

		t31 = r31 * a11 + r32 * a21 + r33 * a31;
		t32 = r31 * a12 + r32 * a22 + r33 * a32;
		t33 = r31 * a13 + r32 * a23 + r33 * a33;

		a11 = t11;	a12 = t12;	a13 = t13;
		a21 = t21;	a22 = t22;	a23 = t23;
		a31 = t31;	a32 = t32;	a33 = t33;

		/* e = r e */

		t1 = r11 * e1 + r12 * e2 + r13 * e3;
		t2 = r21 * e1 + r22 * e2 + r23 * e3;
		t3 = r31 * e1 + r32 * e2 + r33 * e3;

		e1 = t1;	e2 = t2;	e3 = t3;

		/* b = r b */

		t1 = r11 * b1 + r12 * b2 + r13 * b3;
		t2 = r21 * b1 + r22 * b2 + r23 * b3;
		t3 = r31 * b1 + r32 * b2 + r33 * b3;

		b1 = t1;	b2 = t2;	b3 = t3;
	}

	if (b1 == 0.0 && b2 == 0.0)
		;
	else {

		/* r operator */

		cos = b2 / sqrt(b1 * b1 + b2 * b2);
		sin = b1 / sqrt(b1 * b1 + b2 * b2);

		r11 = cos;	r12 = -sin;	r23 = 0.0;
		r21 = sin;	r22 = cos;	r23 = 0.0;
		r31 = 0.0;	r32 = 0.0;	r33 = 1.0;

		/* a = r a */

		t11 = r11 * a11 + r12 * a21 + r13 * a31;
		t12 = r11 * a12 + r12 * a22 + r13 * a32;
		t13 = r11 * a13 + r12 * a23 + r13 * a33;

		t21 = r21 * a11 + r22 * a21 + r23 * a31;
		t22 = r21 * a12 + r22 * a22 + r23 * a32;
		t23 = r21 * a13 + r22 * a23 + r23 * a33;

		t31 = r31 * a11 + r32 * a21 + r33 * a31;
		t32 = r31 * a12 + r32 * a22 + r33 * a32;
		t33 = r31 * a13 + r32 * a23 + r33 * a33;

		a11 = t11;	a12 = t12;	a13 = t13;
		a21 = t21;	a22 = t22;	a23 = t23;
		a31 = t31;	a32 = t32;	a33 = t33;

		/* e = r e */

		t1 = r11 * e1 + r12 * e2 + r13 * e3;
		t2 = r21 * e1 + r22 * e2 + r23 * e3;
		t3 = r31 * e1 + r32 * e2 + r33 * e3;

		e1 = t1;	e2 = t2;	e3 = t3;

		/* b = r b */

		t1 = r11 * b1 + r12 * b2 + r13 * b3;
		t2 = r21 * b1 + r22 * b2 + r23 * b3;
		t3 = r31 * b1 + r32 * b2 + r33 * b3;

		b1 = t1;	b2 = t2;	b3 = t3;
	}

	if (fabs(a11) < 1e-6) a11 = 0.0;
	if (fabs(a12) < 1e-6) a12 = 0.0;
	if (fabs(a13) < 1e-6) a13 = 0.0;

	if (fabs(a21) < 1e-6) a21 = 0.0;
	if (fabs(a22) < 1e-6) a22 = 0.0;
	if (fabs(a23) < 1e-6) a23 = 0.0;

	if (fabs(a31) < 1e-6) a31 = 0.0;
	if (fabs(a32) < 1e-6) a32 = 0.0;
	if (fabs(a33) < 1e-6) a33 = 0.0;

	R2_11 = a11;	R2_12 = a12;	R2_13 = a13;	R2_14 = 0.0;
	R2_21 = a21;	R2_22 = a22;	R2_23 = a23;	R2_24 = 0.0;
	R2_31 = a31;	R2_32 = a32;	R2_33 = a33;	R2_34 = 0.0;
	R2_41 = 0.0;	R2_42 = 0.0;	R2_43 = 0.0;	R2_44 = 1.0;
}
