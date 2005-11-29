/* $\vec E$, $\vec B$ and $\vec O$ define a plane.
This plane intersects the $x^2$-$x^3$ plane in a line.
Find a rotation $R_2$ that rotates the $\vec E$-$\vec B$ plane
about the line of intersection with the $x^2$-$x^3$ plane
until $E^1$ and $B^1$ are eliminated. */

#include <stdio.h>
#include <math.h>

double cosphi, sinphi;
double costheta, sintheta;

double E1 = 1.0;
double E2 = 2.0;
double E3 = 3.0;

double B1 = 4.0;
double B2 = 5.0;
double B3 = 6.0;

double x1, x2, x3;

double R11, R12, R13;
double R21, R22, R23;
double R31, R32, R33;

main()
{
	double h, p1, p2, p3;

	double E1hat, E2hat, E3hat;
	double B1hat, B2hat, B3hat;

	p1 = 0.0;
	p2 = E2 * B1 - E1 * B2;
	p3 = E3 * B1 - E1 * B3;

	h = sqrt(p2 * p2 + p3 * p3);

	cosphi = p3 / h;
	sinphi = p2 / h;

	E1hat = E1;
	E2hat = cosphi * E2 - sinphi * E3;
	E3hat = sinphi * E2 + cosphi * E3;

	B1hat = B1;
	B2hat = cosphi * B2 - sinphi * B3;
	B3hat = sinphi * B2 + cosphi * B3;

	costheta = E2hat / sqrt(E1hat * E1hat + E2hat * E2hat);
	sintheta = E1hat / sqrt(E1hat * E1hat + E2hat * E2hat);

	R11 = costheta;
	R12 = -cosphi * sintheta;
	R13 = sinphi * sintheta;

	R21 = cosphi * sintheta;
	R22 = cosphi * cosphi * costheta + sinphi * sinphi;
	R23 = -cosphi * sinphi * costheta + cosphi * sinphi;

	R31 = -sinphi * sintheta;
	R32 = -cosphi * sinphi * costheta + cosphi * sinphi;
	R33 = sinphi * sinphi * costheta + cosphi * cosphi;

	printf("$$ \\vec E = \\left [ \\matrix { \n");
	printf("%g \\cr \n", E1);
	printf("%g \\cr \n", E2);
	printf("%g \\cr \n", E3);
	printf("} \\right ] $$ \n");

	printf("$$ \\vec B = \\left [ \\matrix { \n");
	printf("%g \\cr \n", B1);
	printf("%g \\cr \n", B2);
	printf("%g \\cr \n", B3);
	printf("} \\right ] $$ \n");

	printf("$$ \\vec P = B^1 \\vec E - E^1 \\vec B = \n");
	printf("\\left [ \\matrix { \n");
	printf("%g \\cr \n", p1);
	printf("%g \\cr \n", p2);
	printf("%g \\cr \n", p3);
	printf("} \\right ] $$ \n");

	printf("$$ R_\\phi = \\left [ \\matrix { \n");
	printf("1 & 0 & 0 \\cr \\cr \n");
	printf("0 & {P^3 \\over \\sqrt{(P^2)^2+(P^3)^2}} \n");
	printf("& {-P^2 \\over \\sqrt{(P^2)^2+(P^3)^2}} \\cr \n");
	printf("0 & {P^2 \\over \\sqrt{(P^2)^2+(P^3)^2}} \n");
	printf("& {P^3 \\over \\sqrt{(P^2)^2+(P^3)^2}} \\cr \n");
	printf("} \\right ] = \\left [ \\matrix { \n");
	printf("%g & %g & %g \\cr \n", 1.0, 0.0, 0.0);
	printf("%g & %g & %g \\cr \n", 0.0, cosphi, -sinphi);
	printf("%g & %g & %g \\cr \n", 0.0, sinphi, cosphi);
	printf("} \\right ] $$ \n");

	printf("$$ \\vec E_\\phi = R_\\phi \\vec E = \\left [ \\matrix { \n");
	printf("%g \\cr \n", E1hat);
	printf("%g \\cr \n", E2hat);
	printf("%g \\cr \n", E3hat);
	printf("} \\right ] $$ \n");

	printf("$$ \\vec B_\\phi = R_\\phi \\vec B = \\left [ \\matrix { \n");
	printf("%g \\cr \n", B1hat);
	printf("%g \\cr \n", B2hat);
	printf("%g \\cr \n", B3hat);
	printf("} \\right ] $$ \n");

	printf("$$ R_\\theta = \\left [ \\matrix { \n");
	printf("{E^2_\\phi \\over \\sqrt{(E^1_\\phi)^2+(E^2_\\phi)^2}} \n");
	printf("& {-E^1_\\phi \\over \\sqrt{(E^1_\\phi)^2+(E^2_\\phi)^2}} & 0 \\cr \n");
	printf("{E^1_\\phi \\over \\sqrt{(E^1_\\phi)^2+(E^2_\\phi)^2}} \n");
	printf("& {E^2_\\phi \\over \\sqrt{(E^1_\\phi)^2+(E^2_\\phi)^2}} & 0 \\cr \n");
	printf("\\cr 0 & 0 & 1 \\cr \n");
	printf("} \\right ] = \\left [ \\matrix { \n");
	printf("%g & %g & %g \\cr \n", costheta, -sintheta, 0.0);
	printf("%g & %g & %g \\cr \n", sintheta, costheta, 0.0);
	printf("%g & %g & %g \\cr \n", 0.0, 0.0, 1.0);
	printf("} \\right ] $$ \n");

	printf("$$ R = R_\\phi^{-1} R_\\theta R_\\phi = \\left [ \\matrix { \n");
	printf("%g & %g & %g \\cr \n", R11, R12, R13);
	printf("%g & %g & %g \\cr \n", R21, R22, R23);
	printf("%g & %g & %g \\cr \n", R31, R32, R33);
	printf("} \\right ] $$ \n");

	x1 = R11 * E1 + R12 * E2 + R13 * E3;
	x2 = R21 * E1 + R22 * E2 + R23 * E3;
	x3 = R31 * E1 + R32 * E2 + R33 * E3;

	if (fabs(x1) < 1e-6) x1 = 0.0;
	if (fabs(x2) < 1e-6) x2 = 0.0;
	if (fabs(x3) < 1e-6) x3 = 0.0;

	printf("$$ R \\vec E = \\left [ \\matrix { \n");
	printf("%g \\cr \n", x1);
	printf("%g \\cr \n", x2);
	printf("%g \\cr \n", x3);
	printf("} \\right ] $$ \n");

	x1 = R11 * B1 + R12 * B2 + R13 * B3;
	x2 = R21 * B1 + R22 * B2 + R23 * B3;
	x3 = R31 * B1 + R32 * B2 + R33 * B3;

	if (fabs(x1) < 1e-6) x1 = 0.0;
	if (fabs(x2) < 1e-6) x2 = 0.0;
	if (fabs(x3) < 1e-6) x3 = 0.0;

	printf("$$ R \\vec B = \\left [ \\matrix { \n");
	printf("%g \\cr \n", x1);
	printf("%g \\cr \n", x2);
	printf("%g \\cr \n", x3);
	printf("} \\right ] $$ \n");
}
