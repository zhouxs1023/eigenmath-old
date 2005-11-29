/* Compute $\Lambda(\beta)$ using the following formulas:
$$A={|\vec E|^2+|\vec B|^2 \over |\vec E \times \vec B|}$$
$$\beta={-A+\sqrt{A^2-4}\over 2} $$ */

#include <stdio.h>
#include <math.h>

double E1 = 1.0;
double E2 = 2.0;
double E3 = 3.0;

double B1 = 4.0;
double B2 = 5.0;
double B3 = 6.0;

double F11, F12, F13, F14;
double F21, F22, F23, F24;
double F31, F32, F33, F34;
double F41, F42, F43, F44;

double R2_11, R2_12, R2_13, R2_14;
double R2_21, R2_22, R2_23, R2_24;
double R2_31, R2_32, R2_33, R2_34;
double R2_41, R2_42, R2_43, R2_44;

double Lb11, Lb12, Lb13, Lb14;
double Lb21, Lb22, Lb23, Lb24;
double Lb31, Lb32, Lb33, Lb34;
double Lb41, Lb42, Lb43, Lb44;

main()
{
	F11 = 0.0;	F12 = B3;	F13 = -B2;	F14 = E1;
	F21 = -B3;	F22 = 0.0;	F23 = B1;	F24 = E2;
	F31 = B2;	F32 = -B1;	F33 = 0.0;	F34 = E3;
	F41 = E1;	F42 = E2;	F43 = E3;	F44 = 0.0;

	DeriveR2FromF();

	DeriveLambdabetaFromF();

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

	/* L R */

	a11 = Lb11 * R2_11 + Lb12 * R2_21 + Lb13 * R2_31 + Lb14 * R2_41;
	a12 = Lb11 * R2_12 + Lb12 * R2_22 + Lb13 * R2_32 + Lb14 * R2_42;
	a13 = Lb11 * R2_13 + Lb12 * R2_23 + Lb13 * R2_33 + Lb14 * R2_43;
	a14 = Lb11 * R2_14 + Lb12 * R2_24 + Lb13 * R2_34 + Lb14 * R2_44;

	a21 = Lb21 * R2_11 + Lb22 * R2_21 + Lb23 * R2_31 + Lb24 * R2_41;
	a22 = Lb21 * R2_12 + Lb22 * R2_22 + Lb23 * R2_32 + Lb24 * R2_42;
	a23 = Lb21 * R2_13 + Lb22 * R2_23 + Lb23 * R2_33 + Lb24 * R2_43;
	a24 = Lb21 * R2_14 + Lb22 * R2_24 + Lb23 * R2_34 + Lb24 * R2_44;

	a31 = Lb31 * R2_11 + Lb32 * R2_21 + Lb33 * R2_31 + Lb34 * R2_41;
	a32 = Lb31 * R2_12 + Lb32 * R2_22 + Lb33 * R2_32 + Lb34 * R2_42;
	a33 = Lb31 * R2_13 + Lb32 * R2_23 + Lb33 * R2_33 + Lb34 * R2_43;
	a34 = Lb31 * R2_14 + Lb32 * R2_24 + Lb33 * R2_34 + Lb34 * R2_44;

	a41 = Lb41 * R2_11 + Lb42 * R2_21 + Lb43 * R2_31 + Lb44 * R2_41;
	a42 = Lb41 * R2_12 + Lb42 * R2_22 + Lb43 * R2_32 + Lb44 * R2_42;
	a43 = Lb41 * R2_13 + Lb42 * R2_23 + Lb43 * R2_33 + Lb44 * R2_43;
	a44 = Lb41 * R2_14 + Lb42 * R2_24 + Lb43 * R2_34 + Lb44 * R2_44;

	/* L R F */

	b11 = a11 * F11 + a12 * F21 + a13 * F31 + a14 * F41;
	b12 = a11 * F12 + a12 * F22 + a13 * F32 + a14 * F42;
	b13 = a11 * F13 + a12 * F23 + a13 * F33 + a14 * F43;
	b14 = a11 * F14 + a12 * F24 + a13 * F34 + a14 * F44;

	b21 = a21 * F11 + a22 * F21 + a23 * F31 + a24 * F41;
	b22 = a21 * F12 + a22 * F22 + a23 * F32 + a24 * F42;
	b23 = a21 * F13 + a22 * F23 + a23 * F33 + a24 * F43;
	b24 = a21 * F14 + a22 * F24 + a23 * F34 + a24 * F44;

	b31 = a31 * F11 + a32 * F21 + a33 * F31 + a34 * F41;
	b32 = a31 * F12 + a32 * F22 + a33 * F32 + a34 * F42;
	b33 = a31 * F13 + a32 * F23 + a33 * F33 + a34 * F43;
	b34 = a31 * F14 + a32 * F24 + a33 * F34 + a34 * F44;

	b41 = a41 * F11 + a42 * F21 + a43 * F31 + a44 * F41;
	b42 = a41 * F12 + a42 * F22 + a43 * F32 + a44 * F42;
	b43 = a41 * F13 + a42 * F23 + a43 * F33 + a44 * F43;
	b44 = a41 * F14 + a42 * F24 + a43 * F34 + a44 * F44;

	/* L R F R^-1 */

	a11 = b11 * R2_11 + b12 * R2_12 + b13 * R2_13 - b14 * R2_14;
	a12 = b11 * R2_21 + b12 * R2_22 + b13 * R2_23 - b14 * R2_24;
	a13 = b11 * R2_31 + b12 * R2_32 + b13 * R2_33 - b14 * R2_34;
	a14 = -b11 * R2_41 - b12 * R2_42 - b13 * R2_43 + b14 * R2_44;

	a21 = b21 * R2_11 + b22 * R2_12 + b23 * R2_13 - b24 * R2_14;
	a22 = b21 * R2_21 + b22 * R2_22 + b23 * R2_23 - b24 * R2_24;
	a23 = b21 * R2_31 + b22 * R2_32 + b23 * R2_33 - b24 * R2_34;
	a24 = -b21 * R2_41 - b22 * R2_42 - b23 * R2_43 + b24 * R2_44;

	a31 = b31 * R2_11 + b32 * R2_12 + b33 * R2_13 - b34 * R2_14;
	a32 = b31 * R2_21 + b32 * R2_22 + b33 * R2_23 - b34 * R2_24;
	a33 = b31 * R2_31 + b32 * R2_32 + b33 * R2_33 - b34 * R2_34;
	a34 = -b31 * R2_41 - b32 * R2_42 - b33 * R2_43 + b34 * R2_44;

	a41 = b41 * R2_11 + b42 * R2_12 + b43 * R2_13 - b44 * R2_14;
	a42 = b41 * R2_21 + b42 * R2_22 + b43 * R2_23 - b44 * R2_24;
	a43 = b41 * R2_31 + b42 * R2_32 + b43 * R2_33 - b44 * R2_34;
	a44 = -b41 * R2_41 - b42 * R2_42 - b43 * R2_43 + b44 * R2_44;

	/* L R F R^-1 L^-1 */

	b11 = a11 * Lb11 + a12 * Lb12 + a13 * Lb13 - a14 * Lb14;
	b12 = a11 * Lb21 + a12 * Lb22 + a13 * Lb23 - a14 * Lb24;
	b13 = a11 * Lb31 + a12 * Lb32 + a13 * Lb33 - a14 * Lb34;
	b14 = -a11 * Lb41 - a12 * Lb42 - a13 * Lb43 + a14 * Lb44;

	b21 = a21 * Lb11 + a22 * Lb12 + a23 * Lb13 - a24 * Lb14;
	b22 = a21 * Lb21 + a22 * Lb22 + a23 * Lb23 - a24 * Lb24;
	b23 = a21 * Lb31 + a22 * Lb32 + a23 * Lb33 - a24 * Lb34;
	b24 = -a21 * Lb41 - a22 * Lb42 - a23 * Lb43 + a24 * Lb44;

	b31 = a31 * Lb11 + a32 * Lb12 + a33 * Lb13 - a34 * Lb14;
	b32 = a31 * Lb21 + a32 * Lb22 + a33 * Lb23 - a34 * Lb24;
	b33 = a31 * Lb31 + a32 * Lb32 + a33 * Lb33 - a34 * Lb34;
	b34 = -a31 * Lb41 - a32 * Lb42 - a33 * Lb43 + a34 * Lb44;

	b41 = a41 * Lb11 + a42 * Lb12 + a43 * Lb13 - a44 * Lb14;
	b42 = a41 * Lb21 + a42 * Lb22 + a43 * Lb23 - a44 * Lb24;
	b43 = a41 * Lb31 + a42 * Lb32 + a43 * Lb33 - a44 * Lb34;
	b44 = -a41 * Lb41 - a42 * Lb42 - a43 * Lb43 + a44 * Lb44;

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

	printf(" $$ \\Lambda(\\beta) = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lb11, Lb12, Lb13, Lb14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lb21, Lb22, Lb23, Lb24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lb31, Lb32, Lb33, Lb34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lb41, Lb42, Lb43, Lb44);
	printf(" } \\right ] $$ \n");

	printf(" $$ \\Lambda(\\beta) R_2 F R_2^{-1} \\Lambda(\\beta)^{-1} = \\left [ \\matrix { \n");
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

	e1 = F41;
	e2 = F42;
	e3 = F43;

	b1 = F23;
	b2 = F31;
	b3 = F12;

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

DeriveLambdabetaFromF()
{
	double A, beta, gamma;

	double e1, e2, e3;
	double b1, b2, b3;
	double x1, x2, x3;

	e1 = F41;
	e2 = F42;
	e3 = F43;

	b1 = F23;
	b2 = F31;
	b3 = F12;

	x1 = e2 * b3 - e3 * b2;
	x2 = e3 * b1 - e1 * b3;
	x3 = e1 * b2 - e2 * b1;

	A = (e1 * e1 + e2 * e2 + e3 * e3 + b1 * b1 + b2 * b2 + b3 * b3) / sqrt(x1 * x1 + x2 * x2 + x3 * x3);

	beta = (-A + sqrt(A * A - 4.0)) / 2.0;

	gamma = 1.0 / sqrt(1.0 - beta * beta);

	Lb11 = gamma;
	Lb12 = 0.0;
	Lb13 = 0.0;
	Lb14 = -beta * gamma;

	Lb21 = 0.0;
	Lb22 = 1.0;
	Lb23 = 0.0;
	Lb24 = 0.0;

	Lb31 = 0.0;
	Lb32 = 0.0;
	Lb33 = 1.0;
	Lb34 = 0.0;

	Lb41 = -beta * gamma;
	Lb42 = 0.0;
	Lb43 = 0.0;
	Lb44 = gamma;
}
