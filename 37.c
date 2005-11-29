/* Derive $\Lambda$ such that $\Lambda F \Lambda^{-1} = F_R$. */

#include <stdio.h>
#include <math.h>

double E1 = 4.0;
double E2 = 4.0;
double E3 = 4.0;

double B1 = 0.0;
double B2 = 0.0;
double B3 = 0.0;

double e1, e2, e3;

double b1, b2, b3;

double t1, t2, t3;

double a11, a12, a13, a14;
double a21, a22, a23, a24;
double a31, a32, a33, a34;
double a41, a42, a43, a44;

double b11, b12, b13, b14;
double b21, b22, b23, b24;
double b31, b32, b33, b34;
double b41, b42, b43, b44;

double t_11, t_12, t_13;
double t_21, t_22, t_23;
double t_31, t_32, t_33;

double R11, R12, R13, R14;
double R21, R22, R23, R24;
double R31, R32, R33, R34;
double R41, R42, R43, R44;

double r1_11, r1_12, r1_13;
double r1_21, r1_22, r1_23;
double r1_31, r1_32, r1_33;

double r2_11, r2_12, r2_13;
double r2_21, r2_22, r2_23;
double r2_31, r2_32, r2_33;

double r3_11, r3_12, r3_13;
double r3_21, r3_22, r3_23;
double r3_31, r3_32, r3_33;

double beta, ygamma;

double L11, L12, L13, L14;
double L21, L22, L23, L24;
double L31, L32, L33, L34;
double L41, L42, L43, L44;

double F11, F12, F13, F14;
double F21, F22, F23, F24;
double F31, F32, F33, F34;
double F41, F42, F43, F44;

double LRFRL11, LRFRL12, LRFRL13, LRFRL14;
double LRFRL21, LRFRL22, LRFRL23, LRFRL24;
double LRFRL31, LRFRL32, LRFRL33, LRFRL34;
double LRFRL41, LRFRL42, LRFRL43, LRFRL44;

double FsubR11, FsubR12, FsubR13, FsubR14;
double FsubR21, FsubR22, FsubR23, FsubR24;
double FsubR31, FsubR32, FsubR33, FsubR34;
double FsubR41, FsubR42, FsubR43, FsubR44;

/* R F R^-1 */

double RFR11, RFR12, RFR13, RFR14;
double RFR21, RFR22, RFR23, RFR24;
double RFR31, RFR32, RFR33, RFR34;
double RFR41, RFR42, RFR43, RFR44;

double R1_11, R1_12, R1_13, R1_14;
double R1_21, R1_22, R1_23, R1_24;
double R1_31, R1_32, R1_33, R1_34;
double R1_41, R1_42, R1_43, R1_44;

double Lambda11, Lambda12, Lambda13, Lambda14;
double Lambda21, Lambda22, Lambda23, Lambda24;
double Lambda31, Lambda32, Lambda33, Lambda34;
double Lambda41, Lambda42, Lambda43, Lambda44;

main()
{
	F11 = 0.0;
	F12 = B3;
	F13 = -B2;
	F14 = E1;

	F21 = -B3;
	F22 = 0.0;
	F23 = B1;
	F24 = E2;

	F31 = B2;
	F32 = -B1;
	F33 = 0.0;
	F34 = E3;

	F41 = E1;
	F42 = E2;
	F43 = E3;
	F44 = 0.0;

	printf(" $$ F = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", F11, F12, F13, F14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F21, F22, F23, F24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F31, F32, F33, F34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", F41, F42, F43, F44);
	printf(" } \\right ] $$ \n\n");

	DeriveFsubR();

	ComputeLambda();
}

DeriveFsubR()
{
	double d;

	DeriveL();

	d = sqrt(LRFRL42 * LRFRL42 + LRFRL43 * LRFRL43);

	R1_11 = 1.0;
	R1_12 = 0.0;
	R1_13 = 0.0;
	R1_14 = 0.0;

	R1_21 = 0.0;
	R1_22 = LRFRL43 / d;
	R1_23 = -LRFRL42 / d;
	R1_24 = 0.0;

	R1_31 = 0.0;
	R1_32 = LRFRL42 / d;
	R1_33 = LRFRL43 / d;
	R1_34 = 0.0;

	R1_41 = 0.0;
	R1_42 = 0.0;
	R1_43 = 0.0;
	R1_44 = 1.0;

	/* R1 LRFRL */

	a11 = R1_11 * LRFRL11 + R1_12 * LRFRL21 + R1_13 * LRFRL31 + R1_14 * LRFRL41;
	a12 = R1_11 * LRFRL12 + R1_12 * LRFRL22 + R1_13 * LRFRL32 + R1_14 * LRFRL42;
	a13 = R1_11 * LRFRL13 + R1_12 * LRFRL23 + R1_13 * LRFRL33 + R1_14 * LRFRL43;
	a14 = R1_11 * LRFRL14 + R1_12 * LRFRL24 + R1_13 * LRFRL34 + R1_14 * LRFRL44;

	a21 = R1_21 * LRFRL11 + R1_22 * LRFRL21 + R1_23 * LRFRL31 + R1_24 * LRFRL41;
	a22 = R1_21 * LRFRL12 + R1_22 * LRFRL22 + R1_23 * LRFRL32 + R1_24 * LRFRL42;
	a23 = R1_21 * LRFRL13 + R1_22 * LRFRL23 + R1_23 * LRFRL33 + R1_24 * LRFRL43;
	a24 = R1_21 * LRFRL14 + R1_22 * LRFRL24 + R1_23 * LRFRL34 + R1_24 * LRFRL44;

	a31 = R1_31 * LRFRL11 + R1_32 * LRFRL21 + R1_33 * LRFRL31 + R1_34 * LRFRL41;
	a32 = R1_31 * LRFRL12 + R1_32 * LRFRL22 + R1_33 * LRFRL32 + R1_34 * LRFRL42;
	a33 = R1_31 * LRFRL13 + R1_32 * LRFRL23 + R1_33 * LRFRL33 + R1_34 * LRFRL43;
	a34 = R1_31 * LRFRL14 + R1_32 * LRFRL24 + R1_33 * LRFRL34 + R1_34 * LRFRL44;

	a41 = R1_41 * LRFRL11 + R1_42 * LRFRL21 + R1_43 * LRFRL31 + R1_44 * LRFRL41;
	a42 = R1_41 * LRFRL12 + R1_42 * LRFRL22 + R1_43 * LRFRL32 + R1_44 * LRFRL42;
	a43 = R1_41 * LRFRL13 + R1_42 * LRFRL23 + R1_43 * LRFRL33 + R1_44 * LRFRL43;
	a44 = R1_41 * LRFRL14 + R1_42 * LRFRL24 + R1_43 * LRFRL34 + R1_44 * LRFRL44;

	/* R1 LRFRL R1^-1 */

	FsubR11 = a11 * R1_11 + a12 * R1_12 + a13 * R1_13 - a14 * R1_14;
	FsubR12 = a11 * R1_21 + a12 * R1_22 + a13 * R1_23 - a14 * R1_24;
	FsubR13 = a11 * R1_31 + a12 * R1_32 + a13 * R1_33 - a14 * R1_34;
	FsubR14 = -a11 * R1_41 - a12 * R1_42 - a13 * R1_43 + a14 * R1_44;

	FsubR21 = a21 * R1_11 + a22 * R1_12 + a23 * R1_13 - a24 * R1_14;
	FsubR22 = a21 * R1_21 + a22 * R1_22 + a23 * R1_23 - a24 * R1_24;
	FsubR23 = a21 * R1_31 + a22 * R1_32 + a23 * R1_33 - a24 * R1_34;
	FsubR24 = -a21 * R1_41 - a22 * R1_42 - a23 * R1_43 + a24 * R1_44;

	FsubR31 = a31 * R1_11 + a32 * R1_12 + a33 * R1_13 - a34 * R1_14;
	FsubR32 = a31 * R1_21 + a32 * R1_22 + a33 * R1_23 - a34 * R1_24;
	FsubR33 = a31 * R1_31 + a32 * R1_32 + a33 * R1_33 - a34 * R1_34;
	FsubR34 = -a31 * R1_41 - a32 * R1_42 - a33 * R1_43 + a34 * R1_44;

	FsubR41 = a41 * R1_11 + a42 * R1_12 + a43 * R1_13 - a44 * R1_14;
	FsubR42 = a41 * R1_21 + a42 * R1_22 + a43 * R1_23 - a44 * R1_24;
	FsubR43 = a41 * R1_31 + a42 * R1_32 + a43 * R1_33 - a44 * R1_34;
	FsubR44 = -a41 * R1_41 - a42 * R1_42 - a43 * R1_43 + b44 * R1_44;

	if (fabs(FsubR11) < 1e-6) FsubR11 = 0.0;
	if (fabs(FsubR12) < 1e-6) FsubR12 = 0.0;
	if (fabs(FsubR13) < 1e-6) FsubR13 = 0.0;
	if (fabs(FsubR14) < 1e-6) FsubR14 = 0.0;

	if (fabs(FsubR21) < 1e-6) FsubR21 = 0.0;
	if (fabs(FsubR22) < 1e-6) FsubR22 = 0.0;
	if (fabs(FsubR23) < 1e-6) FsubR23 = 0.0;
	if (fabs(FsubR24) < 1e-6) FsubR24 = 0.0;

	if (fabs(FsubR31) < 1e-6) FsubR31 = 0.0;
	if (fabs(FsubR32) < 1e-6) FsubR32 = 0.0;
	if (fabs(FsubR33) < 1e-6) FsubR33 = 0.0;
	if (fabs(FsubR34) < 1e-6) FsubR34 = 0.0;

	if (fabs(FsubR41) < 1e-6) FsubR41 = 0.0;
	if (fabs(FsubR42) < 1e-6) FsubR42 = 0.0;
	if (fabs(FsubR43) < 1e-6) FsubR43 = 0.0;
	if (fabs(FsubR44) < 1e-6) FsubR44 = 0.0;

	printf(" $$ R_1 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", R1_11, R1_12, R1_13, R1_14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R1_21, R1_22, R1_23, R1_24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R1_31, R1_32, R1_33, R1_34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R1_41, R1_42, R1_43, R1_44);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ F_R = R_1 \\Lambda(\\theta) R_2 F R_2^{-1} \\Lambda(\\theta)^{-1} R_1^{-1} \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", FsubR11, FsubR12, FsubR13, FsubR14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", FsubR21, FsubR22, FsubR23, FsubR24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", FsubR31, FsubR32, FsubR33, FsubR34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", FsubR41, FsubR42, FsubR43, FsubR44);
	printf(" } \\right ] $$ \n\n");
}

DeriveL()
{
	double a, b, c;

	DeriveR();

	/* construct an L such that e2 / e3 = b2 / b3 */

	a = RFR43 * RFR31;

	b = RFR43 * RFR43 + RFR31 * RFR31 + RFR12 * RFR12;

	c = RFR43 * RFR31;

	beta = (-b + sqrt(b * b - 4.0 * a * c)) / a / 2.0;

	ygamma = 1.0 / sqrt(1 - beta * beta);

	L11 = ygamma;
	L12 = 0.0;
	L13 = 0.0;
	L14 = -beta * ygamma;

	L21 = 0.0;
	L22 = 1.0;
	L23 = 0.0;
	L24 = 0.0;

	L31 = 0.0;
	L32 = 0.0;
	L33 = 1.0;
	L34 = 0.0;

	L41 = -beta * ygamma;
	L42 = 0.0;
	L43 = 0.0;
	L44 = ygamma;

	/* L RFR */

	a11 = L11 * RFR11 + L12 * RFR21 + L13 * RFR31 + L14 * RFR41;
	a12 = L11 * RFR12 + L12 * RFR22 + L13 * RFR32 + L14 * RFR42;
	a13 = L11 * RFR13 + L12 * RFR23 + L13 * RFR33 + L14 * RFR43;
	a14 = L11 * RFR14 + L12 * RFR24 + L13 * RFR34 + L14 * RFR44;

	a21 = L21 * RFR11 + L22 * RFR21 + L23 * RFR31 + L24 * RFR41;
	a22 = L21 * RFR12 + L22 * RFR22 + L23 * RFR32 + L24 * RFR42;
	a23 = L21 * RFR13 + L22 * RFR23 + L23 * RFR33 + L24 * RFR43;
	a24 = L21 * RFR14 + L22 * RFR24 + L23 * RFR34 + L24 * RFR44;

	a31 = L31 * RFR11 + L32 * RFR21 + L33 * RFR31 + L34 * RFR41;
	a32 = L31 * RFR12 + L32 * RFR22 + L33 * RFR32 + L34 * RFR42;
	a33 = L31 * RFR13 + L32 * RFR23 + L33 * RFR33 + L34 * RFR43;
	a34 = L31 * RFR14 + L32 * RFR24 + L33 * RFR34 + L34 * RFR44;

	a41 = L41 * RFR11 + L42 * RFR21 + L43 * RFR31 + L44 * RFR41;
	a42 = L41 * RFR12 + L42 * RFR22 + L43 * RFR32 + L44 * RFR42;
	a43 = L41 * RFR13 + L42 * RFR23 + L43 * RFR33 + L44 * RFR43;
	a44 = L41 * RFR14 + L42 * RFR24 + L43 * RFR34 + L44 * RFR44;

	/* L RFR L^-1 */

	LRFRL11 = a11 * L11 + a12 * L12 + a13 * L13 - a14 * L14;
	LRFRL12 = a11 * L21 + a12 * L22 + a13 * L23 - a14 * L24;
	LRFRL13 = a11 * L31 + a12 * L32 + a13 * L33 - a14 * L34;
	LRFRL14 = -a11 * L41 - a12 * L42 - a13 * L43 + a14 * L44;

	LRFRL21 = a21 * L11 + a22 * L12 + a23 * L13 - a24 * L14;
	LRFRL22 = a21 * L21 + a22 * L22 + a23 * L23 - a24 * L24;
	LRFRL23 = a21 * L31 + a22 * L32 + a23 * L33 - a24 * L34;
	LRFRL24 = -a21 * L41 - a22 * L42 - a23 * L43 + a24 * L44;

	LRFRL31 = a31 * L11 + a32 * L12 + a33 * L13 - a34 * L14;
	LRFRL32 = a31 * L21 + a32 * L22 + a33 * L23 - a34 * L24;
	LRFRL33 = a31 * L31 + a32 * L32 + a33 * L33 - a34 * L34;
	LRFRL34 = -a31 * L41 - a32 * L42 - a33 * L43 + a34 * L44;

	LRFRL41 = a41 * L11 + a42 * L12 + a43 * L13 - a44 * L14;
	LRFRL42 = a41 * L21 + a42 * L22 + a43 * L23 - a44 * L24;
	LRFRL43 = a41 * L31 + a42 * L32 + a43 * L33 - a44 * L34;
	LRFRL44 = -a41 * L41 - a42 * L42 - a43 * L43 + b44 * L44;

	if (fabs(LRFRL11) < 1e-6) LRFRL11 = 0.0;
	if (fabs(LRFRL12) < 1e-6) LRFRL12 = 0.0;
	if (fabs(LRFRL13) < 1e-6) LRFRL13 = 0.0;
	if (fabs(LRFRL14) < 1e-6) LRFRL14 = 0.0;

	if (fabs(LRFRL21) < 1e-6) LRFRL21 = 0.0;
	if (fabs(LRFRL22) < 1e-6) LRFRL22 = 0.0;
	if (fabs(LRFRL23) < 1e-6) LRFRL23 = 0.0;
	if (fabs(LRFRL24) < 1e-6) LRFRL24 = 0.0;

	if (fabs(LRFRL31) < 1e-6) LRFRL31 = 0.0;
	if (fabs(LRFRL32) < 1e-6) LRFRL32 = 0.0;
	if (fabs(LRFRL33) < 1e-6) LRFRL33 = 0.0;
	if (fabs(LRFRL34) < 1e-6) LRFRL34 = 0.0;

	if (fabs(LRFRL41) < 1e-6) LRFRL41 = 0.0;
	if (fabs(LRFRL42) < 1e-6) LRFRL42 = 0.0;
	if (fabs(LRFRL43) < 1e-6) LRFRL43 = 0.0;
	if (fabs(LRFRL44) < 1e-6) LRFRL44 = 0.0;

	printf(" $$ \\Lambda(\\beta) = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", L11, L12, L13, L14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L21, L22, L23, L24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L31, L32, L33, L34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L41, L42, L43, L44);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ \\Lambda(\\beta) R_2 F R_2^{-1} \\Lambda(\\beta)^{-1} = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", LRFRL11, LRFRL12, LRFRL13, LRFRL14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", LRFRL21, LRFRL22, LRFRL23, LRFRL24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", LRFRL31, LRFRL32, LRFRL33, LRFRL34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", LRFRL41, LRFRL42, LRFRL43, LRFRL44);
	printf(" } \\right ] $$ \n\n");
}

DeriveR()
{
	double d;

	e1 = F41;	e2 = F42;	e3 = F43;

	b1 = F23;	b2 = F31;	b3 = F12;

	/* eliminate e1 */

	d = sqrt(e1 * e1 + e2 * e2);

	r1_11 = e2 / d;		r1_12 = -e1 / d;	r1_13 = 0.0;
	r1_21 = e1 / d;		r1_22 = e2 / d;		r1_23 = 0.0;
	r1_31 = 0.0;		r1_32 = 0.0;		r1_33 = 1.0;

	t1 = r1_11 * e1 + r1_12 * e2 + r1_13 * e3;
	t2 = r1_21 * e1 + r1_22 * e2 + r1_23 * e3;
	t3 = r1_31 * e1 + r1_32 * e2 + r1_33 * e3;

	e1 = t1;
	e2 = t2;
	e3 = t3;

	t1 = r1_11 * b1 + r1_12 * b2 + r1_13 * b3;
	t2 = r1_21 * b1 + r1_22 * b2 + r1_23 * b3;
	t3 = r1_31 * b1 + r1_32 * b2 + r1_33 * b3;

	b1 = t1;
	b2 = t2;
	b3 = t3;

	/* eliminate e2 */

	d = sqrt(e2 * e2 + e3 * e3);

	r2_11 = 1.0;		r2_12 = 0.0;		r2_13 = 0.0;
	r2_21 = 0.0;		r2_22 = e3 / d;		r2_23 = -e2 / d;
	r2_31 = 0.0;		r2_32 = e2 / d;		r2_33 = e3 / d;

	t1 = r2_11 * e1 + r2_12 * e2 + r2_13 * e3;
	t2 = r2_21 * e1 + r2_22 * e2 + r2_23 * e3;
	t3 = r2_31 * e1 + r2_32 * e2 + r2_33 * e3;

	e1 = t1;
	e2 = t2;
	e3 = t3;

	t1 = r2_11 * b1 + r2_12 * b2 + r2_13 * b3;
	t2 = r2_21 * b1 + r2_22 * b2 + r2_23 * b3;
	t3 = r2_31 * b1 + r2_32 * b2 + r2_33 * b3;

	b1 = t1;
	b2 = t2;
	b3 = t3;

	/* eliminate b1 */

	d = sqrt(b1 * b1 + b2 * b2);

	r3_11 = b2 / d;		r3_12 = -b1 / d;	r3_13 = 0.0;
	r3_21 = b1 / d;		r3_22 = b2 / d;		r3_23 = 0.0;
	r3_31 = 0.0;		r3_32 = 0.0;		r3_33 = 1.0;

	/* put them all together */

	t_11 = r3_11 * r2_11 + r3_12 * r2_21 + r3_13 * r2_31;
	t_12 = r3_11 * r2_12 + r3_12 * r2_22 + r3_13 * r2_32;
	t_13 = r3_11 * r2_13 + r3_12 * r2_23 + r3_13 * r2_33;

	t_21 = r3_21 * r2_11 + r3_22 * r2_21 + r3_23 * r2_31;
	t_22 = r3_21 * r2_12 + r3_22 * r2_22 + r3_23 * r2_32;
	t_23 = r3_21 * r2_13 + r3_22 * r2_23 + r3_23 * r2_33;

	t_31 = r3_31 * r2_11 + r3_32 * r2_21 + r3_33 * r2_31;
	t_32 = r3_31 * r2_12 + r3_32 * r2_22 + r3_33 * r2_32;
	t_33 = r3_31 * r2_13 + r3_32 * r2_23 + r3_33 * r2_33;

	R11 = t_11 * r1_11 + t_12 * r1_21 + t_13 * r1_31;
	R12 = t_11 * r1_12 + t_12 * r1_22 + t_13 * r1_32;
	R13 = t_11 * r1_13 + t_12 * r1_23 + t_13 * r1_33;
	R14 = 0.0;

	R21 = t_21 * r1_11 + t_22 * r1_21 + t_23 * r1_31;
	R22 = t_21 * r1_12 + t_22 * r1_22 + t_23 * r1_32;
	R23 = t_21 * r1_13 + t_22 * r1_23 + t_23 * r1_33;
	R24 = 0.0;

	R31 = t_31 * r1_11 + t_32 * r1_21 + t_33 * r1_31;
	R32 = t_31 * r1_12 + t_32 * r1_22 + t_33 * r1_32;
	R33 = t_31 * r1_13 + t_32 * r1_23 + t_33 * r1_33;
	R34 = 0.0;

	R41 = 0.0;
	R42 = 0.0;
	R43 = 0.0;
	R44 = 1.0;

	/* R F */

	a11 = R11 * F11 + R12 * F21 + R13 * F31 + R14 * F41;
	a12 = R11 * F12 + R12 * F22 + R13 * F32 + R14 * F42;
	a13 = R11 * F13 + R12 * F23 + R13 * F33 + R14 * F43;
	a14 = R11 * F14 + R12 * F24 + R13 * F34 + R14 * F44;

	a21 = R21 * F11 + R22 * F21 + R23 * F31 + R24 * F41;
	a22 = R21 * F12 + R22 * F22 + R23 * F32 + R24 * F42;
	a23 = R21 * F13 + R22 * F23 + R23 * F33 + R24 * F43;
	a24 = R21 * F14 + R22 * F24 + R23 * F34 + R24 * F44;

	a31 = R31 * F11 + R32 * F21 + R33 * F31 + R34 * F41;
	a32 = R31 * F12 + R32 * F22 + R33 * F32 + R34 * F42;
	a33 = R31 * F13 + R32 * F23 + R33 * F33 + R34 * F43;
	a34 = R31 * F14 + R32 * F24 + R33 * F34 + R34 * F44;

	a41 = R41 * F11 + R42 * F21 + R43 * F31 + R44 * F41;
	a42 = R41 * F12 + R42 * F22 + R43 * F32 + R44 * F42;
	a43 = R41 * F13 + R42 * F23 + R43 * F33 + R44 * F43;
	a44 = R41 * F14 + R42 * F24 + R43 * F34 + R44 * F44;

	/* R F R^-1 */

	RFR11 = a11 * R11 + a12 * R12 + a13 * R13 - a14 * R14;
	RFR12 = a11 * R21 + a12 * R22 + a13 * R23 - a14 * R24;
	RFR13 = a11 * R31 + a12 * R32 + a13 * R33 - a14 * R34;
	RFR14 = -a11 * R41 - a12 * R42 - a13 * R43 + a14 * R44;

	RFR21 = a21 * R11 + a22 * R12 + a23 * R13 - a24 * R14;
	RFR22 = a21 * R21 + a22 * R22 + a23 * R23 - a24 * R24;
	RFR23 = a21 * R31 + a22 * R32 + a23 * R33 - a24 * R34;
	RFR24 = -a21 * R41 - a22 * R42 - a23 * R43 + a24 * R44;

	RFR31 = a31 * R11 + a32 * R12 + a33 * R13 - a34 * R14;
	RFR32 = a31 * R21 + a32 * R22 + a33 * R23 - a34 * R24;
	RFR33 = a31 * R31 + a32 * R32 + a33 * R33 - a34 * R34;
	RFR34 = -a31 * R41 - a32 * R42 - a33 * R43 + a34 * R44;

	RFR41 = a41 * R11 + a42 * R12 + a43 * R13 - a44 * R14;
	RFR42 = a41 * R21 + a42 * R22 + a43 * R23 - a44 * R24;
	RFR43 = a41 * R31 + a42 * R32 + a43 * R33 - a44 * R34;
	RFR44 = -a41 * R41 - a42 * R42 - a43 * R43 + a44 * R44;

	if (fabs(RFR11) < 1e-6) RFR11 = 0.0;
	if (fabs(RFR12) < 1e-6) RFR12 = 0.0;
	if (fabs(RFR13) < 1e-6) RFR13 = 0.0;
	if (fabs(RFR14) < 1e-6) RFR14 = 0.0;

	if (fabs(RFR21) < 1e-6) RFR21 = 0.0;
	if (fabs(RFR22) < 1e-6) RFR22 = 0.0;
	if (fabs(RFR23) < 1e-6) RFR23 = 0.0;
	if (fabs(RFR24) < 1e-6) RFR24 = 0.0;

	if (fabs(RFR31) < 1e-6) RFR31 = 0.0;
	if (fabs(RFR32) < 1e-6) RFR32 = 0.0;
	if (fabs(RFR33) < 1e-6) RFR33 = 0.0;
	if (fabs(RFR34) < 1e-6) RFR34 = 0.0;

	if (fabs(RFR41) < 1e-6) RFR41 = 0.0;
	if (fabs(RFR42) < 1e-6) RFR42 = 0.0;
	if (fabs(RFR43) < 1e-6) RFR43 = 0.0;
	if (fabs(RFR44) < 1e-6) RFR44 = 0.0;

	printf(" $$ R_2 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", R11, R12, R13, R14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R21, R22, R23, R24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R31, R32, R33, R34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", R41, R42, R43, R44);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_2 F R_2^{-1} = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", RFR11, RFR12, RFR13, RFR14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", RFR21, RFR22, RFR23, RFR24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", RFR31, RFR32, RFR33, RFR34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", RFR41, RFR42, RFR43, RFR44);
	printf(" } \\right ] $$ \n\n");
}

ComputeLambda()
{
	/* R1 L */

	a11 = R1_11 * L11 + R1_12 * L21 + R1_13 * L31 + R1_14 * L41;
	a12 = R1_11 * L12 + R1_12 * L22 + R1_13 * L32 + R1_14 * L42;
	a13 = R1_11 * L13 + R1_12 * L23 + R1_13 * L33 + R1_14 * L43;
	a14 = R1_11 * L14 + R1_12 * L24 + R1_13 * L34 + R1_14 * L44;

	a21 = R1_21 * L11 + R1_22 * L21 + R1_23 * L31 + R1_24 * L41;
	a22 = R1_21 * L12 + R1_22 * L22 + R1_23 * L32 + R1_24 * L42;
	a23 = R1_21 * L13 + R1_22 * L23 + R1_23 * L33 + R1_24 * L43;
	a24 = R1_21 * L14 + R1_22 * L24 + R1_23 * L34 + R1_24 * L44;

	a31 = R1_31 * L11 + R1_32 * L21 + R1_33 * L31 + R1_34 * L41;
	a32 = R1_31 * L12 + R1_32 * L22 + R1_33 * L32 + R1_34 * L42;
	a33 = R1_31 * L13 + R1_32 * L23 + R1_33 * L33 + R1_34 * L43;
	a34 = R1_31 * L14 + R1_32 * L24 + R1_33 * L34 + R1_34 * L44;

	a41 = R1_41 * L11 + R1_42 * L21 + R1_43 * L31 + R1_44 * L41;
	a42 = R1_41 * L12 + R1_42 * L22 + R1_43 * L32 + R1_44 * L42;
	a43 = R1_41 * L13 + R1_42 * L23 + R1_43 * L33 + R1_44 * L43;
	a44 = R1_41 * L14 + R1_42 * L24 + R1_43 * L34 + R1_44 * L44;

	/* R1 L R2 */

	Lambda11 = a11 * R11 + a12 * R21 + a13 * R31 + a14 * R41;
	Lambda12 = a11 * R12 + a12 * R22 + a13 * R32 + a14 * R42;
	Lambda13 = a11 * R13 + a12 * R23 + a13 * R33 + a14 * R43;
	Lambda14 = a11 * R14 + a12 * R24 + a13 * R34 + a14 * R44;

	Lambda21 = a21 * R11 + a22 * R21 + a23 * R31 + a24 * R41;
	Lambda22 = a21 * R12 + a22 * R22 + a23 * R32 + a24 * R42;
	Lambda23 = a21 * R13 + a22 * R23 + a23 * R33 + a24 * R43;
	Lambda24 = a21 * R14 + a22 * R24 + a23 * R34 + a24 * R44;

	Lambda31 = a31 * R11 + a32 * R21 + a33 * R31 + a34 * R41;
	Lambda32 = a31 * R12 + a32 * R22 + a33 * R32 + a34 * R42;
	Lambda33 = a31 * R13 + a32 * R23 + a33 * R33 + a34 * R43;
	Lambda34 = a31 * R14 + a32 * R24 + a33 * R34 + a34 * R44;

	Lambda41 = a41 * R11 + a42 * R21 + a43 * R31 + a44 * R41;
	Lambda42 = a41 * R12 + a42 * R22 + a43 * R32 + a44 * R42;
	Lambda43 = a41 * R13 + a42 * R23 + a43 * R33 + a44 * R43;
	Lambda44 = a41 * R14 + a42 * R24 + a43 * R34 + a44 * R44;

	printf(" $$ \\Lambda = R_1 \\Lambda(\\beta) R_2 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lambda11, Lambda12, Lambda13, Lambda14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lambda21, Lambda22, Lambda23, Lambda24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lambda31, Lambda32, Lambda33, Lambda34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", Lambda41, Lambda42, Lambda43, Lambda44);
	printf(" } \\right ] $$ \n\n");
}
