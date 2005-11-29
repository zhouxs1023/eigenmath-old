/* First try at finding a canonical basis for sample $F$. */

#include <stdio.h>
#include <math.h>

double e1 = 1.0;
double e2 = 2.0;
double e3 = 3.0;

double b1 = 4.0;
double b2 = 5.0;
double b3 = 6.0;

double Lambda[4][4], Lambda1[4][4];
double f[4][4], fhat[4][4], fhat1[4][4];
double x[4];
double y[4];
double delta;
double epsilon;
double e11, e12, e13, e14;
double e21, e22, e23, e24;
double e31, e32, e33, e34;
double e41, e42, e43, e44;

double L11, L12, L13, L14;
double L21, L22, L23, L24;
double L31, L32, L33, L34;
double L41, L42, L43, L44;

double Lambda11, Lambda12, Lambda13, Lambda14;
double Lambda21, Lambda22, Lambda23, Lambda24;
double Lambda31, Lambda32, Lambda33, Lambda34;
double Lambda41, Lambda42, Lambda43, Lambda44;

#define F11 f[0][0]
#define F12 f[0][1]
#define F13 f[0][2]
#define F14 f[0][3]

#define F21 f[1][0]
#define F22 f[1][1]
#define F23 f[1][2]
#define F24 f[1][3]

#define F31 f[2][0]
#define F32 f[2][1]
#define F33 f[2][2]
#define F34 f[2][3]

#define F41 f[3][0]
#define F42 f[3][1]
#define F43 f[3][2]
#define F44 f[3][3]

#define x1 x[0]
#define x2 x[1]
#define x3 x[2]
#define x4 x[3]

#define y1 y[0]
#define y2 y[1]
#define y3 y[2]
#define y4 y[3]

main(argc, argv)
int argc;
char *argv[];
{
	int n;

/*	test1(); */

	F_from_E_and_B();

	canonical_basis();

	L_from_basis();

	Lambda_from_L();

	Lambda_times_F_times_inverse_of_Lambda();

	e1_dot_e1();
	e2_dot_e2();
	e3_dot_e3();
	e4_dot_e4();

	e2_dot_e4();

	printf(" \\end \n");

	exit();


	if (argc < 2) {
		printf("example number? ");
		scanf("%d", &n);
	} else
		sscanf(argv[1], "%d", &n);

	example();
}

example()
{
	int i, j;
	double A, alpha, e_dot_b, lambda[4];

	f[0][0] = 0.0;	f[0][1] = b3;	f[0][2] = -b2;	f[0][3] = e1;
	f[1][0] = -b3;	f[1][1] = 0.0;	f[1][2] = b1;	f[1][3] = e2;
	f[2][0] = b2;	f[2][1] = -b1;	f[2][2] = 0.0;	f[2][3] = e3;
	f[3][0] = e1;	f[3][1] = e2;	f[3][2] = e3;	f[3][3] = 0.0;

	/* E */

	printf("$$E=\\left[\\matrix{%g\\cr%g\\cr%g}\\right]$$\n", e1, e2, e3);

	/* B */

	printf("$$B=\\left[\\matrix{%g\\cr%g\\cr%g}\\right]$$\n", b1, b2, b3);

	/* F */

	printf("$$F=\\left[\\matrix{\n");
	printf("%g&%g&%g&%g\\cr\n", 0.0, b3, -b2, e1);
	printf("%g&%g&%g&%g\\cr\n", -b3, 0.0, b1, e2);
	printf("%g&%g&%g&%g\\cr\n", b2, -b1, 0.0, e3);
	printf("%g&%g&%g&%g\n", e1, e2, e3, 0.0);
	printf("}\\right]$$\n");

	/* |B| ^ 2 - |E| ^ 2 */

	A = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);
/*	printf("$$ |B|^2  - |E|^2 = %g $$\n", A); */

	/* E . B */

	e_dot_b = (e1 * b1 + e2 * b2 + e3 * b3);
	printf("$$\\vec E\\cdot\\vec B=%g$$\n", e_dot_b);

	/* eigenvalues */

	printf("eigenvalues\n");

	lambda[0] = sqrt(-A + sqrt(A * A + 4.0 * e_dot_b * e_dot_b)) / sqrt(2.0);
	lambda[1] = -lambda[0];
	lambda[2] = sqrt(A + sqrt(A * A + 4.0 * e_dot_b * e_dot_b)) / sqrt(2.0);
	lambda[3] = -lambda[2];

	/* canonical form */

	if (lambda[0] != 0.0) {

		epsilon = lambda[0];
		printf("$$\\epsilon=\\lambda_1=%g$$\n", epsilon);

		delta = e_dot_b / epsilon;
		printf("$$\\delta={\\vec E\\cdot\\vec B\\over\\epsilon}=%g$$\n", delta);

		printf("$$\\hat F=\\left[\\matrix{\n");

		printf("0&\\delta&0&0\\cr\n");
		printf("-\\delta&0&0&0\\cr\n");
		printf("0&0&0&\\epsilon\\cr\n");
		printf("0&0&\\epsilon&0\n");

		printf("}\\right]=\\left[\\matrix{\n");

		printf("%g&%g&%g&%g\\cr\n", 0.0, delta, 0.0, 0.0);
		printf("%g&%g&%g&%g\\cr\n", -delta, 0.0, 0.0, 0.0);
		printf("%g&%g&%g&%g\\cr\n", 0.0, 0.0, 0.0, epsilon);
		printf("%g&%g&%g&%g\n", 0.0, 0.0, epsilon, 0.0);

		printf("}\\right]$$\n");

		/* Lambda */

		Lambda[0][0] = delta * e3 / e_dot_b;
		Lambda[0][1] = 0.0;
		Lambda[0][2] = -delta * e1 / e_dot_b;
		Lambda[0][3] = delta * b2 / e_dot_b;

		Lambda[1][0] = 0.0;
		Lambda[1][1] = delta * e3 / e_dot_b;
		Lambda[1][2] = -delta * e2 / e_dot_b;
		Lambda[1][3] = -delta * b1 / e_dot_b;

		Lambda[2][0] = epsilon * b1 / e_dot_b;
		Lambda[2][1] = epsilon * b2 / e_dot_b;
		Lambda[2][2] = epsilon * b3 / e_dot_b;
		Lambda[2][3] = 0.0;

		Lambda[3][0] = -epsilon * e2 / e_dot_b;
		Lambda[3][1] = epsilon * e1 / e_dot_b;
		Lambda[3][2] = 0.0;
		Lambda[3][3] = epsilon * b3 / e_dot_b;

		printf("$$\\Lambda=\\hat F F^{-1}={1\\over\\vec E\\cdot\\vec B}\\left[\\matrix{\n");

		printf("\\delta E^3 & 0 & -\\delta E^1 & \\delta B^2 \\cr\n");
		printf("0 & \\delta E^3 & -\\delta E^2 & -\\delta B^1 \\cr\n");
		printf("\\epsilon B^1 & \\epsilon B^2 & \\epsilon B^3 & 0 \\cr\n");
		printf("-\\epsilon E^2 & \\epsilon E^1 & 0 & \\epsilon B^3 \n");

		printf("}\\right]=\\left[\\matrix{\n");

		printf("%g&%g&%g&%g\\cr\n", Lambda[0][0], Lambda[0][1], Lambda[0][2], Lambda[0][3]);
		printf("%g&%g&%g&%g\\cr\n", Lambda[1][0], Lambda[1][1], Lambda[1][2], Lambda[1][3]);
		printf("%g&%g&%g&%g\\cr\n", Lambda[2][0], Lambda[2][1], Lambda[2][2], Lambda[2][3]);
		printf("%g&%g&%g&%g\n",     Lambda[3][0], Lambda[3][1], Lambda[3][2], Lambda[3][3]);

		printf("}\\right]$$\n");

		lambda_times_f();

		Lambda_from_inverse_of_Lambda();

		lambda_times_f();

		eigenvector_x_from_E_and_B();

		F_times_x();

		epsilon_from_E_and_B();

		epsilon_times_x();

	} else if (lambda[2] != 0.0)
		;
	else
		;

	printf("\\end\n");
}

lambda_times_f()
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++) {
			fhat[i][j] =
			Lambda[i][0] * f[0][j] +
			Lambda[i][1] * f[1][j] +
			Lambda[i][2] * f[2][j] +
			Lambda[i][3] * f[3][j];
			if (fabs(fhat[i][j]) < 1e-6)
				fhat[i][j] = 0.0;
		}

	printf("$$\\Lambda F=\\left[\\matrix{\n");

	printf("%g&%g&%g&%g\\cr\n", fhat[0][0], fhat[0][1], fhat[0][2], fhat[0][3]);
	printf("%g&%g&%g&%g\\cr\n", fhat[1][0], fhat[1][1], fhat[1][2], fhat[1][3]);
	printf("%g&%g&%g&%g\\cr\n", fhat[2][0], fhat[2][1], fhat[2][2], fhat[2][3]);
	printf("%g&%g&%g&%g\n",     fhat[3][0], fhat[3][1], fhat[3][2], fhat[3][3]);

	printf("}\\right]$$\n");
}

yy3()
{

	Lambda1[0][0] = b3 / delta;
	Lambda1[0][1] = 0.0;
	Lambda1[0][2] = e1 / epsilon;
	Lambda1[0][3] = -b2 / epsilon;

	Lambda1[1][0] = 0.0;
	Lambda1[1][1] = b3 / delta;
	Lambda1[1][2] = e2 / epsilon;
	Lambda1[1][3] = b1 / epsilon;

	Lambda1[2][0] = -b1 / delta;
	Lambda1[2][1] = -b2 / delta;
	Lambda1[2][2] = e3 / epsilon;
	Lambda1[2][3] = 0.0;

	Lambda1[3][0] = e2 / delta;
	Lambda1[3][1] = -e1 / delta;
	Lambda1[3][2] = 0.0;
	Lambda1[3][3] = e3 / epsilon;

	printf("$$\\Lambda^{-1}=F{\\hat F}^{-1}=\\left[\\matrix{\n");

	printf("%g&%g&%g&%g\\cr\n", Lambda1[0][0], Lambda1[0][1], Lambda1[0][2], Lambda1[0][3]);
	printf("%g&%g&%g&%g\\cr\n", Lambda1[1][0], Lambda1[1][1], Lambda1[1][2], Lambda1[1][3]);
	printf("%g&%g&%g&%g\\cr\n", Lambda1[2][0], Lambda1[2][1], Lambda1[2][2], Lambda1[2][3]);
	printf("%g&%g&%g&%g\n",     Lambda1[3][0], Lambda1[3][1], Lambda1[3][2], Lambda1[3][3]);

	printf("}\\right]$$\n");
}

Lambda_from_inverse_of_Lambda()
{
	Lambda[0][0] = Lambda1[0][0];
	Lambda[0][1] = Lambda1[1][0];
	Lambda[0][2] = Lambda1[2][0];
	Lambda[0][3] = -Lambda1[3][0];

	Lambda[1][0] = Lambda1[0][1];
	Lambda[1][1] = Lambda1[1][1];
	Lambda[1][2] = Lambda1[2][1];
	Lambda[1][3] = -Lambda1[3][1];

	Lambda[2][0] = Lambda1[0][2];
	Lambda[2][1] = Lambda1[1][2];
	Lambda[2][2] = Lambda1[2][2];
	Lambda[2][3] = -Lambda1[3][2];

	Lambda[3][0] = -Lambda1[0][3];
	Lambda[3][1] = -Lambda1[1][3];
	Lambda[3][2] = -Lambda1[2][3];
	Lambda[3][3] = Lambda1[3][3];

	printf("$$ \\Lambda = \\eta ( \\Lambda ^ {-1} ) ^ T \\eta = \n");

	printf(" \\left [ \\matrix { \n");

	printf("%g&%g&%g&%g\\cr\n", Lambda[0][0], Lambda[0][1], Lambda[0][2], Lambda[0][3]);
	printf("%g&%g&%g&%g\\cr\n", Lambda[1][0], Lambda[1][1], Lambda[1][2], Lambda[1][3]);
	printf("%g&%g&%g&%g\\cr\n", Lambda[2][0], Lambda[2][1], Lambda[2][2], Lambda[2][3]);
	printf("%g&%g&%g&%g\n",     Lambda[3][0], Lambda[3][1], Lambda[3][2], Lambda[3][3]);

	printf("} \\right ] $$ \n\n");
}

eigenvector_x_from_E_and_B()
{
	double a, e_dot_b, l, z;

	a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);
	e_dot_b = e1 * b1 + e2 * b2 + e3 * b3;
	l = sqrt((-a + sqrt(a * a + 4.0 * e_dot_b * e_dot_b)) / 2.0);
	z = l * l * l + l * (b1 * b1 + b2 * b2 + b3 * b3);

	x1 = (e1 * l * l + e2 * b3 * l - e3 * b2 * l + b1 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	x2 = (e2 * l * l - e1 * b3 * l + e3 * b1 * l + b2 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	x3 = (e3 * l * l + e1 * b2 * l - e2 * b1 * l + b3 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	x4 = 1.0;

	printf(" $$ x = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", x1);
	printf(" %15g \\cr \n", x2);
	printf(" %15g \\cr \n", x3);
	printf(" %15g \\cr \n", x4);
	printf(" } \\right ] $$ \n\n");
}

eigenvector_y_from_E_and_B()
{
	double a, e_dot_b, l, z;

	a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);
	e_dot_b = e1 * b1 + e2 * b2 + e3 * b3;
	l = -sqrt((-a + sqrt(a * a + 4.0 * e_dot_b * e_dot_b)) / 2.0);
	z = l * l * l + l * (b1 * b1 + b2 * b2 + b3 * b3);

	y1 = (e1 * l * l + e2 * b3 * l - e3 * b2 * l + b1 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y2 = (e2 * l * l - e1 * b3 * l + e3 * b1 * l + b2 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y3 = (e3 * l * l + e1 * b2 * l - e2 * b1 * l + b3 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y4 = 1.0;

	printf(" $$ y = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", y1);
	printf(" %15g \\cr \n", y2);
	printf(" %15g \\cr \n", y3);
	printf(" %15g \\cr \n", y4);
	printf(" } \\right ] $$ \n\n");
}

eigenvector_x_from_delta()
{
	x1 = 0.0;
	x2 = 0.0;
	x3 = epsilon + delta;
	x4 = epsilon - delta;

	printf(" $$ x = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", x1);
	printf(" %15g \\cr \n", x2);
	printf(" %15g \\cr \n", x3);
	printf(" %15g \\cr \n", x4);
	printf(" } \\right ] $$ \n\n");
}

eigenvector_y_from_delta()
{
	double l, z;

	l = -delta;
	z = l * l * l + l * (b1 * b1 + b2 * b2 + b3 * b3);

	y1 = (-e1 * l * l + e2 * b3 * l - e3 * b2 * l + b1 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y2 = (-e2 * l * l - e1 * b3 * l + e3 * b1 * l + b2 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y3 = (-e3 * l * l + e1 * b2 * l - e2 * b1 * l + b3 * (e1 * b1 + e2 * b2 + e3 * b3)) / z;
	y4 = 0.0;

	printf(" $$ y = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", y1);
	printf(" %15g \\cr \n", y2);
	printf(" %15g \\cr \n", y3);
	printf(" %15g \\cr \n", y4);
	printf(" } \\right ] $$ \n\n");
}

F_times_x()
{
	double y[4];

	y[0] = f[0][0] * x[0] + f[0][1] * x[1] + f[0][2] * x[2] + f[0][3] * x[3];
	y[1] = f[1][0] * x[0] + f[1][1] * x[1] + f[1][2] * x[2] + f[1][3] * x[3];
	y[2] = f[2][0] * x[0] + f[2][1] * x[1] + f[2][2] * x[2] + f[2][3] * x[3];
	y[3] = f[3][0] * x[0] + f[3][1] * x[1] + f[3][2] * x[2] + f[3][3] * x[3];

	printf(" $$ Fx = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", y[0]);
	printf(" %15g \\cr \n", y[1]);
	printf(" %15g \\cr \n", y[2]);
	printf(" %15g \\cr \n", y[3]);
	printf(" } \\right ] $$ \n\n");
}

epsilon_from_E_and_B()
{
	double a, e_dot_b;
	a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);
	e_dot_b = e1 * b1 + e2 * b2 + e3 * b3;
	epsilon = sqrt((-a + sqrt(a * a + 4.0 * e_dot_b * e_dot_b)) / 2.0);

	printf(" $$ \\epsilon = %g $$ \n\n", epsilon);
}

delta_from_E_and_B()
{
	double a, e_dot_b;
	a = (b1 * b1 + b2 * b2 + b3 * b3) - (e1 * e1 + e2 * e2 + e3 * e3);
	e_dot_b = e1 * b1 + e2 * b2 + e3 * b3;
	delta = sqrt((a + sqrt(a * a + 4.0 * e_dot_b * e_dot_b)) / 2.0);

	printf(" $$ \\delta = %g $$ \n\n", delta);
}

epsilon_times_x()
{
	printf(" $$ \\epsilon x = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", epsilon * x[0]);
	printf(" %15g \\cr \n", epsilon * x[1]);
	printf(" %15g \\cr \n", epsilon * x[2]);
	printf(" %15g \\cr \n", epsilon * x[3]);
	printf(" } \\right ] $$ \n\n");
}

F_from_E_and_B()
{
	f[0][0] = 0.0;	f[0][1] = b3;	f[0][2] = -b2;	f[0][3] = e1;
	f[1][0] = -b3;	f[1][1] = 0.0;	f[1][2] = b1;	f[1][3] = e2;
	f[2][0] = b2;	f[2][1] = -b1;	f[2][2] = 0.0;	f[2][3] = e3;
	f[3][0] = e1;	f[3][1] = e2;	f[3][2] = e3;	f[3][3] = 0.0;

	printf(" $$ F = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", f[0][0], f[0][1], f[0][2], f[0][3]);
	printf(" %15g & %15g & %15g & %15g \\cr \n", f[1][0], f[1][1], f[1][2], f[1][3]);
	printf(" %15g & %15g & %15g & %15g \\cr \n", f[2][0], f[2][1], f[2][2], f[2][3]);
	printf(" %15g & %15g & %15g & %15g \\cr \n", f[3][0], f[3][1], f[3][2], f[3][3]);
	printf(" } \\right ] $$ \n\n");
}

e1_dot_e1()
{
	double t;
	t = e11 * e11 + e12 * e12 + e13 * e13 - e14 * e14;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_1 \\cdot e_1 = %g $$ \n\n", t);
}

e2_dot_e2()
{
	double t;
	t = e21 * e21 + e22 * e22 + e23 * e23 - e24 * 24;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_2 \\cdot e_2 = %g $$ \n\n", t);
}

e3_dot_e3()
{
	double t;
	t = e31 * e31 + e32 * e32 + e33 * e33 - e34 * e34;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_3 \\cdot e_3 = %g $$ \n\n", t);
}

e3_dot_e4()
{
	double t;
	t = e31 * e41 + e32 * e42 + e33 * e43 - e34 * e44;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_3 \\cdot e_4 = %g $$ \n\n", t);
}

e4_dot_e4()
{
	double t;
	t = e41 * e41 + e42 * e42 + e43 * e43 - e44 * e44;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_4 \\cdot e_4 = %g $$ \n\n", t);
}

e2_dot_e4()
{
	double t;
	t = e21 * e41 + e22 * e42 + e23 * e43 - e24 * e44;
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ e_2 \\cdot e_4 = %g $$ \n\n", t);
}

x_dot_x()
{
	double t;
	t = x[0] * x[0] + x[1] * x[1] + x[2] * x[2] - x[3] * x[3];
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ x \\cdot x = %g $$ \n\n", t);
}

y_dot_y()
{
	double t;
	t = y[0] * y[0] + y[1] * y[1] + y[2] * y[2] - y[3] * y[3];
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ y \\cdot y = %g $$ \n\n", t);
}

x_dot_y()
{
	double t;
	t = x[0] * y[0] + x[1] * y[1] + x[2] * y[2] - x[3] * y[3];
	if (fabs(t) < 1e-6)
		t = 0.0;
	printf(" $$ x \\cdot y = %g $$ \n\n", t);
}

F_times_x_minus_epsilon_times_x()
{
	double t1, t2, t3, t4;

	t1 = F11 * x1 + F12 * x2 + F13 * x3 + F14 * x4 - epsilon * x1;
	t2 = F21 * x1 + F22 * x2 + F23 * x3 + F24 * x4 - epsilon * x2;
	t3 = F31 * x1 + F32 * x2 + F33 * x3 + F34 * x4 - epsilon * x3;
	t4 = F41 * x1 + F42 * x2 + F43 * x3 + F44 * x4 - epsilon * x4;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;
	if (fabs(t4) < 1e-6)
		t4 = 0.0;

	printf(" $$ F x - \\epsilon x = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", t1);
	printf(" %15g \\cr \n", t2);
	printf(" %15g \\cr \n", t3);
	printf(" %15g \\cr \n", t4);
	printf(" } \\right ] $$ \n\n");
}

F_times_y_plus_epsilon_times_y()
{
	double t1, t2, t3, t4;

	t1 = F11 * y1 + F12 * y2 + F13 * y3 + F14 * y4 + epsilon * y1;
	t2 = F21 * y1 + F22 * y2 + F23 * y3 + F24 * y4 + epsilon * y2;
	t3 = F31 * y1 + F32 * y2 + F33 * y3 + F34 * y4 + epsilon * y3;
	t4 = F41 * y1 + F42 * y2 + F43 * y3 + F44 * y4 + epsilon * y4;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;
	if (fabs(t4) < 1e-6)
		t4 = 0.0;

	printf(" $$ F y + \\epsilon y = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", t1);
	printf(" %15g \\cr \n", t2);
	printf(" %15g \\cr \n", t3);
	printf(" %15g \\cr \n", t4);
	printf(" } \\right ] $$ \n\n");
}

F_times_x_minus_delta_times_x()
{
	double t1, t2, t3, t4;

	t1 = F11 * x1 + F12 * x2 + F13 * x3 + F14 * x4 - delta * x1;
	t2 = F21 * x1 + F22 * x2 + F23 * x3 + F24 * x4 - delta * x2;
	t3 = F31 * x1 + F32 * x2 + F33 * x3 + F34 * x4 - delta * x3;
	t4 = F41 * x1 + F42 * x2 + F43 * x3 + F44 * x4 - delta * x4;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;
	if (fabs(t4) < 1e-6)
		t4 = 0.0;

	printf(" $$ F x - \\delta x = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", t1);
	printf(" %15g \\cr \n", t2);
	printf(" %15g \\cr \n", t3);
	printf(" %15g \\cr \n", t4);
	printf(" } \\right ] $$ \n\n");
}

F_times_y_plus_delta_times_y()
{
	double t1, t2, t3, t4;

	t1 = F11 * y1 + F12 * y2 + F13 * y3 + F14 * y4 + delta * y1;
	t2 = F21 * y1 + F22 * y2 + F23 * y3 + F24 * y4 + delta * y2;
	t3 = F31 * y1 + F32 * y2 + F33 * y3 + F34 * y4 + delta * y3;
	t4 = F41 * y1 + F42 * y2 + F43 * y3 + F44 * y4 + delta * y4;

	if (fabs(t1) < 1e-6)
		t1 = 0.0;
	if (fabs(t2) < 1e-6)
		t2 = 0.0;
	if (fabs(t3) < 1e-6)
		t3 = 0.0;
	if (fabs(t4) < 1e-6)
		t4 = 0.0;

	printf(" $$ F y + \\delta y = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", t1);
	printf(" %15g \\cr \n", t2);
	printf(" %15g \\cr \n", t3);
	printf(" %15g \\cr \n", t4);
	printf(" } \\right ] $$ \n\n");
}

canonical_basis()
{
	double a, b;

	double u1, u2, u3, u4;
	double v1, v2, v3, v4;

	epsilon_from_E_and_B();
	delta_from_E_and_B();

	eigenvector_x_from_E_and_B();
	eigenvector_y_from_E_and_B();

	u1 = (x2 + y2) * (x3 + y3);
	u2 = (x1 + y1) * (x3 + y3);
	u3 = -(x1 + y1) * (x2 + y2);
	u4 = 0.0;

	v1 = (x2 - y2) * (x3 - y3);
	v2 = (x1 - y1) * (x3 - y3);
	v3 = -(x1 - y1) * (x2 - y2);
	v4 = 0.0;

	a = 0.5 / (u1 * v1 + u2 * v2 + u3 * v3 - u4 * v4);

	b = 1.0;

	e11 = a * u1 + b * v1;
	e12 = a * u2 + b * v2;
	e13 = a * u3 + b * v3;
	e14 = a * u4 + b * v4;

	e21 = -a * u1 + b * v1;
	e22 = -a * u2 + b * v2;
	e23 = -a * u3 + b * v3;
	e24 = -a * u4 + b * v4;

	a = 0.5 / (x1 * y1 + x2 * y2 + x3 * y3 - x4 * y4);

	b = 1.0;

	e31 = a * x1 + b * y1;
	e32 = a * x2 + b * y2;
	e33 = a * x3 + b * y3;
	e34 = a * x4 + b * y4;

	e41 = -a * x1 + b * y1;
	e42 = -a * x2 + b * y2;
	e43 = -a * x3 + b * y3;
	e44 = -a * x4 + b * y4;

	printf(" $$ e_1 = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e11);
	printf(" %15g \\cr \n", e12);
	printf(" %15g \\cr \n", e13);
	printf(" %15g \\cr \n", e14);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ e_2 = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e21);
	printf(" %15g \\cr \n", e22);
	printf(" %15g \\cr \n", e23);
	printf(" %15g \\cr \n", e24);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ e_3 = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e31);
	printf(" %15g \\cr \n", e32);
	printf(" %15g \\cr \n", e33);
	printf(" %15g \\cr \n", e34);
	printf(" } \\right ] $$ \n\n");

	printf(" $$ e_4 = \\left [ \\matrix { \n");
	printf(" %15g \\cr \n", e41);
	printf(" %15g \\cr \n", e42);
	printf(" %15g \\cr \n", e43);
	printf(" %15g \\cr \n", e44);
	printf(" } \\right ] $$ \n\n");
}

L_from_basis()
{
	L11 = e11;
	L21 = e12;
	L31 = e13;
	L41 = e14;

	L12 = e21;
	L22 = e22;
	L32 = e23;
	L42 = e24;

	L13 = e31;
	L23 = e32;
	L33 = e33;
	L43 = e34;

	L14 = e41;
	L24 = e42;
	L34 = e43;
	L44 = e44;

	printf(" $$ \\L = \\left [ \\matrix { \n");

	printf(" e_1^1 & e_2^1 & e_3^1 & e_4^1 \\cr \n");
	printf(" e_1^2 & e_2^2 & e_3^2 & e_4^2 \\cr \n");
	printf(" e_1^3 & e_2^3 & e_3^3 & e_4^3 \\cr \n");
	printf(" e_1^4 & e_2^4 & e_3^4 & e_4^4 \\cr \n");

	printf(" } \\right ] = \\left [ \\matrix { \n");

	printf(" %15g & %15g & %15g & %15g \\cr \n", L11, L12, L13, L14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L21, L22, L23, L24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L31, L32, L33, L34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", L41, L42, L43, L44);
	printf(" } \\right ] $$ \n\n");
}

Lambda_from_L()
{
	Lambda11 = L11;
	Lambda12 = L21;
	Lambda13 = L31;
	Lambda14 = -L41;

	Lambda21 = L12;
	Lambda22 = L22;
	Lambda23 = L32;
	Lambda24 = -L42;

	Lambda31 = L13;
	Lambda32 = L23;
	Lambda33 = L33;
	Lambda34 = -L43;

	Lambda41 = -L14;
	Lambda42 = -L24;
	Lambda43 = -L34;
	Lambda44 = L44;
}

Lambda_times_F_times_inverse_of_Lambda()
{
	double a11, a12, a13, a14;
	double a21, a22, a23, a24;
	double a31, a32, a33, a34;
	double a41, a42, a43, a44;

	double b11, b12, b13, b14;
	double b21, b22, b23, b24;
	double b31, b32, b33, b34;
	double b41, b42, b43, b44;

	a11 = Lambda11 * F11 + Lambda12 * F21 * Lambda13 * F31 + Lambda14 * F41;
	a12 = Lambda11 * F12 + Lambda12 * F22 * Lambda13 * F32 + Lambda14 * F42;
	a13 = Lambda11 * F13 + Lambda12 * F23 * Lambda13 * F33 + Lambda14 * F43;
	a14 = Lambda11 * F14 + Lambda12 * F24 * Lambda13 * F34 + Lambda14 * F44;

	a21 = Lambda21 * F11 + Lambda22 * F21 * Lambda23 * F31 + Lambda24 * F41;
	a22 = Lambda21 * F12 + Lambda22 * F22 * Lambda23 * F32 + Lambda24 * F42;
	a23 = Lambda21 * F13 + Lambda22 * F23 * Lambda23 * F33 + Lambda24 * F43;
	a24 = Lambda21 * F14 + Lambda22 * F24 * Lambda23 * F34 + Lambda24 * F44;

	a31 = Lambda31 * F11 + Lambda32 * F21 * Lambda33 * F31 + Lambda34 * F41;
	a32 = Lambda31 * F12 + Lambda32 * F22 * Lambda33 * F32 + Lambda34 * F42;
	a33 = Lambda31 * F13 + Lambda32 * F23 * Lambda33 * F33 + Lambda34 * F43;
	a34 = Lambda31 * F14 + Lambda32 * F24 * Lambda33 * F34 + Lambda34 * F44;

	a41 = Lambda41 * F11 + Lambda42 * F21 * Lambda43 * F31 + Lambda44 * F41;
	a42 = Lambda41 * F12 + Lambda42 * F22 * Lambda43 * F32 + Lambda44 * F42;
	a43 = Lambda41 * F13 + Lambda42 * F23 * Lambda43 * F33 + Lambda44 * F43;
	a44 = Lambda41 * F14 + Lambda42 * F24 * Lambda43 * F34 + Lambda44 * F44;

	b11 = a11 * Lambda11 + a12 * Lambda12 + a13 * Lambda13 - a14 * Lambda14;
	b12 = a11 * Lambda21 + a12 * Lambda22 + a13 * Lambda23 - a14 * Lambda24;
	b13 = a11 * Lambda31 + a12 * Lambda32 + a13 * Lambda33 - a14 * Lambda34;
	b14 = -a11 * Lambda41 - a12 * Lambda42 - a13 * Lambda43 + a14 * Lambda44;

	b21 = a21 * Lambda11 + a22 * Lambda12 + a23 * Lambda13 - a24 * Lambda14;
	b22 = a21 * Lambda21 + a22 * Lambda22 + a23 * Lambda23 - a24 * Lambda24;
	b23 = a21 * Lambda31 + a22 * Lambda32 + a23 * Lambda33 - a24 * Lambda34;
	b24 = -a21 * Lambda41 - a22 * Lambda42 - a23 * Lambda43 + a24 * Lambda44;

	b31 = a31 * Lambda11 + a32 * Lambda12 + a33 * Lambda13 - a34 * Lambda14;
	b32 = a31 * Lambda21 + a32 * Lambda22 + a33 * Lambda23 - a34 * Lambda24;
	b33 = a31 * Lambda31 + a32 * Lambda32 + a33 * Lambda33 - a34 * Lambda34;
	b34 = -a31 * Lambda41 - a32 * Lambda42 - a33 * Lambda43 + a34 * Lambda44;

	b41 = a41 * Lambda11 + a42 * Lambda12 + a43 * Lambda13 - a44 * Lambda14;
	b42 = a41 * Lambda21 + a42 * Lambda22 + a43 * Lambda23 - a44 * Lambda24;
	b43 = a41 * Lambda31 + a42 * Lambda32 + a43 * Lambda33 - a44 * Lambda34;
	b44 = -a41 * Lambda41 - a42 * Lambda42 - a43 * Lambda43 + a44 * Lambda44;

	printf(" $$ \\Lambda F \\Lambda ^ { -1 } = \\left [ \\matrix { \n");
	printf(" %15g & %15g & %15g & %15g \\cr \n", b11, b12, b13, b14);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b21, b22, b23, b24);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b31, b32, b33, b34);
	printf(" %15g & %15g & %15g & %15g \\cr \n", b41, b42, b43, b44);
	printf(" } \\right ] $$ \n\n");
}

test1()
{
	F_from_E_and_B();

	epsilon_from_E_and_B();
	delta_from_E_and_B();

	eigenvector_x_from_E_and_B();
	eigenvector_y_from_E_and_B();

	F_times_x_minus_epsilon_times_x();
	F_times_y_plus_epsilon_times_y();

	printf(" \\end \n");
}

/*

eigenvector(lambda)
double lambda;
{
	double m;

	double a11, a12, a13, a14;
	double a21, a22, a23, a24;
	double a31, a32, a33, a34;
	double a41, a42, a43, a44;

	a11 = F11;	a12 = F12;	a13 = F13;	a14 = F14;
	a21 = F21;	a22 = F22;	a23 = F23;	a24 = F24;
	a31 = F31;	a32 = F32;	a33 = F33;	a34 = F34;
	a41 = F41;	a42 = F42;	a43 = F43;	a44 = F44;

	a11 -= lambda;
	a22 -= lambda;
	a33 -= lambda;
	a44 -= lambda;

	if (a11 != 0.0) {

		m = a21 / a11;

		a21 -= m * a11;
		a22 -= m * a12;
		a23 -= m * a13;
		a24 -= m * a14;

		m = a31 / a11;

		a31 -= m * a11;
		a32 -= m * a12;
		a33 -= m * a13;
		a34 -= m * a14;

		m = a41 / a11;

		a41 -= m * a11;
		a42 -= m * a12;
		a43 -= m * a13;
		a44 -= m * a14;
	}

	if (a22 != 0.0) {

		m = a12 / a22;

		a11 -= m * a21;
		a12 -= m * a22;
		a13 -= m * a23;
		a14 -= m * a24;

		m = a32 / a22;

		a31 -= m * a21;
		a32 -= m * a22;
		a33 -= m * a23;
		a34 -= m * a24;

		m = a42 / a22;

		a41 -= m * a21;
		a42 -= m * a22;
		a43 -= m * a23;
		a44 -= m * a24;
	}

	if (a33 != 0.0) {

		m = a13 / a33;

		a11 -= m * a31;
		a12 -= m * a32;
		a13 -= m * a33;
		a14 -= m * a34;

		m = a23 / a33;

		a21 -= m * a31;
		a22 -= m * a32;
		a23 -= m * a33;
		a24 -= m * a34;

		m = a43 / a33;

		a41 -= m * a31;
		a42 -= m * a32;
		a43 -= m * a33;
		a44 -= m * a34;
	}

	if (fabs(a44) > 1e-6) {

		m = a14 / a44;

		a11 -= m * a41;
		a12 -= m * a42;
		a13 -= m * a43;
		a14 -= m * a44;

		m = a24 / a44;

		a21 -= m * a41;
		a22 -= m * a42;
		a23 -= m * a43;
		a24 -= m * a44;

		m = a34 / a44;

		a31 -= m * a41;
		a32 -= m * a42;
		a33 -= m * a43;
		a34 -= m * a44;
	}

	if (fabs(a11) < 1e-6)
		a11 = 0.0;
	if (fabs(a12) < 1e-6)
		a12 = 0.0;
	if (fabs(a13) < 1e-6)
		a13 = 0.0;
	if (fabs(a14) < 1e-6)
		a14 = 0.0;

	if (fabs(a21) < 1e-6)
		a21 = 0.0;
	if (fabs(a22) < 1e-6)
		a22 = 0.0;
	if (fabs(a23) < 1e-6)
		a23 = 0.0;
	if (fabs(a24) < 1e-6)
		a24 = 0.0;

	if (fabs(a31) < 1e-6)
		a31 = 0.0;
	if (fabs(a32) < 1e-6)
		a32 = 0.0;
	if (fabs(a33) < 1e-6)
		a33 = 0.0;
	if (fabs(a34) < 1e-6)
		a34 = 0.0;

	if (fabs(a41) < 1e-6)
		a41 = 0.0;
	if (fabs(a42) < 1e-6)
		a42 = 0.0;
	if (fabs(a43) < 1e-6)
		a43 = 0.0;
	if (fabs(a44) < 1e-6)
		a44 = 0.0;

	printf("%15g %15g %15g %15g\n", a11, a12, a13, a14);
	printf("%15g %15g %15g %15g\n", a21, a22, a23, a24);
	printf("%15g %15g %15g %15g\n", a31, a32, a33, a34);
	printf("%15g %15g %15g %15g\n", a41, a42, a43, a44);
}

eigenvector(lambda)
double lambda;
{
	int d, i;
	double a[4][4], m;

	a[0][0] = F11 - lambda;
	a[0][1] = F12;
	a[0][2] = F13;
	a[0][3] = F14;

	a[1][0] = F21;
	a[1][1] = F22 - lambda;
	a[1][2] = F23;
	a[1][3] = F24;

	a[2][0] = F31;
	a[2][1] = F32;
	a[2][2] = F33 - lambda;
	a[2][3] = F14;

	a[3][0] = F41;
	a[3][1] = F42;
	a[3][2] = F43;
	a[3][3] = F44 - lambda;

	for (d = 0; d < 4; d++) {

		for (i = d + 1; i < 4; i++)
			if (fabs(a[d][d]) < fabs(a[i][d])) {
				a[d][0] = a[i][0];
				a[d][1] = a[i][1];
				a[d][2] = a[i][2];
				a[d][3] = a[i][3];
			}

		if (fabs(a[d][d]) > 1e-6)
			for (i = 0; i < 4; i++) {
				if (i == d)
					continue;
				m = a[i][d] / a[d][d];
				a[i][0] -= m * a[d][0];
				a[i][1] -= m * a[d][1];
				a[i][2] -= m * a[d][2];
				a[i][3] -= m * a[d][3];
			}
	}

	if (fabs(a[0][0]) < 1e-6)
		a[0][0] = 0.0;
	if (fabs(a[0][1]) < 1e-6)
		a[0][1] = 0.0;
	if (fabs(a[0][2]) < 1e-6)
		a[0][2] = 0.0;
	if (fabs(a[0][3]) < 1e-6)
		a[0][3] = 0.0;

	if (fabs(a[1][0]) < 1e-6)
		a[1][0] = 0.0;
	if (fabs(a[1][1]) < 1e-6)
		a[1][1] = 0.0;
	if (fabs(a[1][2]) < 1e-6)
		a[1][2] = 0.0;
	if (fabs(a[1][3]) < 1e-6)
		a[1][3] = 0.0;

	if (fabs(a[2][0]) < 1e-6)
		a[2][0] = 0.0;
	if (fabs(a[2][1]) < 1e-6)
		a[2][1] = 0.0;
	if (fabs(a[2][2]) < 1e-6)
		a[2][2] = 0.0;
	if (fabs(a[2][3]) < 1e-6)
		a[2][3] = 0.0;

	if (fabs(a[3][0]) < 1e-6)
		a[3][0] = 0.0;
	if (fabs(a[3][1]) < 1e-6)
		a[3][1] = 0.0;
	if (fabs(a[3][2]) < 1e-6)
		a[3][2] = 0.0;
	if (fabs(a[3][3]) < 1e-6)
		a[3][3] = 0.0;

	printf("%15g %15g %15g %15g\n", a[0][0], a[0][1], a[0][2], a[0][3]);
	printf("%15g %15g %15g %15g\n", a[1][0], a[1][1], a[1][2], a[1][3]);
	printf("%15g %15g %15g %15g\n", a[2][0], a[2][1], a[2][2], a[2][3]);
	printf("%15g %15g %15g %15g\n", a[3][0], a[3][1], a[3][2], a[3][3]);
}

*/
