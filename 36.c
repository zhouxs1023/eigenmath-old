/* Find a rotation that converts $\vec E$ to $(0, E^2, E^3)$
and $\vec B$ to $(0, B^2, B^3)$. */

#include <stdio.h>
#include <math.h>

double E1 = 1.0;
double E2 = 2.0;
double E3 = 3.0;

double B1 = 4.0;
double B2 = 5.0;
double B3 = 6.0;

double e1, e2, e3;

double b1, b2, b3;

double t1, t2, t3;

double t_11, t_12, t_13;
double t_21, t_22, t_23;
double t_31, t_32, t_33;

double r_11, r_12, r_13;
double r_21, r_22, r_23;
double r_31, r_32, r_33;

double r1_11, r1_12, r1_13;
double r1_21, r1_22, r1_23;
double r1_31, r1_32, r1_33;

double r2_11, r2_12, r2_13;
double r2_21, r2_22, r2_23;
double r2_31, r2_32, r2_33;

double r3_11, r3_12, r3_13;
double r3_21, r3_22, r3_23;
double r3_31, r3_32, r3_33;

double r4_11, r4_12, r4_13;
double r4_21, r4_22, r4_23;
double r4_31, r4_32, r4_33;

main()
{
	double d;

	e1 = E1;	e2 = E2;	e3 = E3;

	b1 = B1;	b2 = B2;	b3 = B3;

	printf(" $$ E = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e1);
	printf(" %15g \\cr \n", e2);
	printf(" %15g \\cr \n", e3);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ B = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", b1);
	printf(" %15g \\cr \n", b2);
	printf(" %15g \\cr \n", b3);
	printf(" } \\right ] $$ \n\n");

	/* eliminate e1 */

	d = sqrt(e1 * e1 + e2 * e2);

	r1_11 = e2 / d;		r1_12 = -e1 / d;	r1_13 = 0.0;

	r1_21 = e1 / d;		r1_22 = e2 / d;		r1_23 = 0.0;

	r1_31 = 0.0;		r1_32 = 0.0;		r1_33 = 1.0;

	t1 = r1_11 * e1 + r1_12 * e2 + r1_13 * e3;
	t2 = r1_21 * e1 + r1_22 * e2 + r1_23 * e3;
	t3 = r1_31 * e1 + r1_32 * e2 + r1_33 * e3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	e1 = t1;
	e2 = t2;
	e3 = t3;

	t1 = r1_11 * b1 + r1_12 * b2 + r1_13 * b3;
	t2 = r1_21 * b1 + r1_22 * b2 + r1_23 * b3;
	t3 = r1_31 * b1 + r1_32 * b2 + r1_33 * b3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	b1 = t1;
	b2 = t2;
	b3 = t3;

	printf(" $$ R_1 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g \\cr \n", r1_11, r1_12, r1_13);
	printf(" %15g & %15g & %15g \\cr \n", r1_21, r1_22, r1_23);
	printf(" %15g & %15g & %15g \\cr \n", r1_31, r1_32, r1_33);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_1 E = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e1);
	printf(" %15g \\cr \n", e2);
	printf(" %15g \\cr \n", e3);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_1 B = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", b1);
	printf(" %15g \\cr \n", b2);
	printf(" %15g \\cr \n", b3);
	printf(" } \\right ] $$ \n\n");

	/* eliminate e2 */

	d = sqrt(e2 * e2 + e3 * e3);

	r2_11 = 1.0;		r2_12 = 0.0;		r2_13 = 0.0;

	r2_21 = 0.0;		r2_22 = e3 / d;		r2_23 = -e2 / d;

	r2_31 = 0.0;		r2_32 = e2 / d;		r2_33 = e3 / d;

	t1 = r2_11 * e1 + r2_12 * e2 + r2_13 * e3;
	t2 = r2_21 * e1 + r2_22 * e2 + r2_23 * e3;
	t3 = r2_31 * e1 + r2_32 * e2 + r2_33 * e3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	e1 = t1;
	e2 = t2;
	e3 = t3;

	t1 = r2_11 * b1 + r2_12 * b2 + r2_13 * b3;
	t2 = r2_21 * b1 + r2_22 * b2 + r2_23 * b3;
	t3 = r2_31 * b1 + r2_32 * b2 + r2_33 * b3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	b1 = t1;
	b2 = t2;
	b3 = t3;

	printf(" $$ R_2 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g \\cr \n", r2_11, r2_12, r2_13);
	printf(" %15g & %15g & %15g \\cr \n", r2_21, r2_22, r2_23);
	printf(" %15g & %15g & %15g \\cr \n", r2_31, r2_32, r2_33);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_2 R_1 E = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e1);
	printf(" %15g \\cr \n", e2);
	printf(" %15g \\cr \n", e3);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_2 R_1 B = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", b1);
	printf(" %15g \\cr \n", b2);
	printf(" %15g \\cr \n", b3);
	printf(" } \\right ] $$ \n\n");

	/* eliminate b1 */

	d = sqrt(b1 * b1 + b2 * b2);

	r3_11 = b2 / d;		r3_12 = -b1 / d;	r3_13 = 0.0;

	r3_21 = b1 / d;		r3_22 = b2 / d;		r3_23 = 0.0;

	r3_31 = 0.0;		r3_32 = 0.0;		r3_33 = 1.0;

	t1 = r3_11 * e1 + r3_12 * e2 + r3_13 * e3;
	t2 = r3_21 * e1 + r3_22 * e2 + r3_23 * e3;
	t3 = r3_31 * e1 + r3_32 * e2 + r3_33 * e3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	e1 = t1;
	e2 = t2;
	e3 = t3;

	t1 = r3_11 * b1 + r3_12 * b2 + r3_13 * b3;
	t2 = r3_21 * b1 + r3_22 * b2 + r3_23 * b3;
	t3 = r3_31 * b1 + r3_32 * b2 + r3_33 * b3;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;

	b1 = t1;
	b2 = t2;
	b3 = t3;

	printf(" $$ R_3 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g \\cr \n", r3_11, r3_12, r3_13);
	printf(" %15g & %15g & %15g \\cr \n", r3_21, r3_22, r3_23);
	printf(" %15g & %15g & %15g \\cr \n", r3_31, r3_32, r3_33);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_3 R_2 R_1 E = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e1);
	printf(" %15g \\cr \n", e2);
	printf(" %15g \\cr \n", e3);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R_3 R_2 R_1 B = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", b1);
	printf(" %15g \\cr \n", b2);
	printf(" %15g \\cr \n", b3);
	printf(" } \\right ] $$ \n\n");

	/* put back r2 */

	r4_11 = 1.0;	r4_12 = 0.0;	r4_13 = 0.0;
	r4_21 = 1.0;	r4_22 = r2_22;	r4_23 = r2_32;
	r4_31 = 1.0;	r4_32 = r2_23;	r4_33 = r2_33;

	/* r2^1 r3 r2 r1 */

	t_11 = r4_11 * r3_11 + r4_12 * r3_21 + r4_13 * r3_31;
	t_12 = r4_11 * r3_12 + r4_12 * r3_22 + r4_13 * r3_32;
	t_13 = r4_11 * r3_13 + r4_12 * r3_23 + r4_13 * r3_33;

	t_21 = r4_21 * r3_11 + r4_22 * r3_21 + r4_23 * r3_31;
	t_22 = r4_21 * r3_12 + r4_22 * r3_22 + r4_23 * r3_32;
	t_23 = r4_21 * r3_13 + r4_22 * r3_23 + r4_23 * r3_33;

	t_31 = r4_31 * r3_11 + r4_32 * r3_21 + r4_33 * r3_31;
	t_32 = r4_31 * r3_12 + r4_32 * r3_22 + r4_33 * r3_32;
	t_33 = r4_31 * r3_13 + r4_32 * r3_23 + r4_33 * r3_33;

	r3_11 = t_11;	r3_12 = t_12;	r3_13 = t_13;
	r3_21 = t_21;	r3_22 = t_22;	r3_23 = t_23;
	r3_31 = t_31;	r3_32 = t_32;	r3_33 = t_33;

	t_11 = r3_11 * r2_11 + r3_12 * r2_21 + r3_13 * r2_31;
	t_12 = r3_11 * r2_12 + r3_12 * r2_22 + r3_13 * r2_32;
	t_13 = r3_11 * r2_13 + r3_12 * r2_23 + r3_13 * r2_33;

	t_21 = r3_21 * r2_11 + r3_22 * r2_21 + r3_23 * r2_31;
	t_22 = r3_21 * r2_12 + r3_22 * r2_22 + r3_23 * r2_32;
	t_23 = r3_21 * r2_13 + r3_22 * r2_23 + r3_23 * r2_33;

	t_31 = r3_31 * r2_11 + r3_32 * r2_21 + r3_33 * r2_31;
	t_32 = r3_31 * r2_12 + r3_32 * r2_22 + r3_33 * r2_32;
	t_33 = r3_31 * r2_13 + r3_32 * r2_23 + r3_33 * r2_33;

	r_11 = t_11 * r1_11 + t_12 * r1_21 + t_13 * r1_31;
	r_12 = t_11 * r1_12 + t_12 * r1_22 + t_13 * r1_32;
	r_13 = t_11 * r1_13 + t_12 * r1_23 + t_13 * r1_33;

	r_21 = t_21 * r1_11 + t_22 * r1_21 + t_23 * r1_31;
	r_22 = t_21 * r1_12 + t_22 * r1_22 + t_23 * r1_32;
	r_23 = t_21 * r1_13 + t_22 * r1_23 + t_23 * r1_33;

	r_31 = t_31 * r1_11 + t_32 * r1_21 + t_33 * r1_31;
	r_32 = t_31 * r1_12 + t_32 * r1_22 + t_33 * r1_32;
	r_33 = t_31 * r1_13 + t_32 * r1_23 + t_33 * r1_33;

	e1 = r_11 * E1 + r_12 * E2 + r_13 * E3;
	e2 = r_21 * E1 + r_22 * E2 + r_23 * E3;
	e3 = r_31 * E1 + r_32 * E2 + r_33 * E3;

	if (fabs(e1) < 1e-6)
		e1 = 0.0;
	if (fabs(e2) < 1e-6)
		e2 = 0.0;
	if (fabs(e3) < 1e-6)
		e3 = 0.0;

	b1 = r_11 * B1 + r_12 * B2 + r_13 * B3;
	b2 = r_21 * B1 + r_22 * B2 + r_23 * B3;
	b3 = r_31 * B1 + r_32 * B2 + r_33 * B3;

	if (fabs(b1) < 1e-6)
		b1 = 0.0;
	if (fabs(b2) < 1e-6)
		b2 = 0.0;
	if (fabs(b3) < 1e-6)
		b3 = 0.0;

	printf(" $$ R = R_2^{-1} R_3 R_2 R_1 = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g \\cr \n", r_11, r_12, r_13);
	printf(" %15g & %15g & %15g \\cr \n", r_21, r_22, r_23);
	printf(" %15g & %15g & %15g \\cr \n", r_31, r_32, r_33);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R E = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e1);
	printf(" %15g \\cr \n", e2);
	printf(" %15g \\cr \n", e3);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ R B = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", b1);
	printf(" %15g \\cr \n", b2);
	printf(" %15g \\cr \n", b3);
	printf(" } \\right ] $$ \n\n");

	printf(" \\end\n");
}
