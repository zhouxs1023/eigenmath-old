/* Derive $[F_{ab}]$. */

#include <stdio.h>

#define E1 1

#define E2 2

#define E3 3

#define B1 4

#define B2 5

#define B3 6

int e[4][4] = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1,
};

int F[4][4] = {
	0,	B3,	-B2,	E1,
	-B3,	0,	B1,	E2,
	B2,	-B1,	0,	E3,
	E1,	E2,	E3,	0,
};

int ea[4];

int eb[4];

int Feb[4];

int FF[4][4];

main()

{

	int a, b, i, j;

	printf("\\magnification=1200\n");

	for (a = 0; a < 4; a++)

		for (b = 0; b < 4; b++) {

			calc(a, b);

			printf("$$F_{%d%d}=\n", a + 1, b + 1);

			printf("\\left [ \\matrix { %d \\cr %d \\cr %d \\cr %d } \\right ]\n", ea[0], ea[1], ea[2], ea[3]);

			printf("\\cdot\n");

			printf("\\left [ \\matrix {\n");

			for (i = 0; i < 4; i++) {

				for (j = 0; j < 4; j++) {

					printEB(F[i][j]);

					printf(" & ");

				}

				printf("\\cr\n");

			}

			printf("} \\right ] \n");

			printf("\\left [ \\matrix { %d \\cr %d \\cr %d \\cr %d } \\right ]\n", eb[0], eb[1], eb[2], eb[3]);

			printf("=\n");

			printf("\\left [ \\matrix { %d \\cr %d \\cr %d \\cr %d } \\right ]\n", ea[0], ea[1], ea[2], ea[3]);

			printf("\\cdot\n");

			printf("\\left [ \\matrix {\n");

			for (i = 0; i < 4; i++) {

				printEB(Feb[i]);

				printf("\\cr\n");

			}

			printf("} \\right ]\n");

			printf("=\n");

			printEB(FF[a][b]);

			printf("$$\n");

		}


	printf("$$[F_{ab}]=\\left [ \\matrix {\n");

	for (i = 0; i < 4; i++) {

		for (j = 0; j < 4; j++) {

			printEB(FF[i][j]);

			printf(" & ");

		}

		printf("\\cr\n");

	}

	printf("} \\right ] $$ \\end\n");

}

printEB(n)

int n;

{

	switch (n) {

	case 0:

		printf("0");

		break;

	case E1:

		printf("E^1");

		break;

	case -E1:

		printf("-E^1");

		break;

	case E2:

		printf("E^2");

		break;

	case -E2:

		printf("-E^2");

		break;

	case E3:

		printf("E^3");

		break;

	case -E3:

		printf("-E^3");

		break;

	case B1:

		printf("B^1");

		break;

	case -B1:

		printf("-B^1");

		break;

	case B2:

		printf("B^2");

		break;

	case -B2:

		printf("-B^2");

		break;

	case B3:

		printf("B^3");

		break;

	case -B3:

		printf("-B^3");

		break;

	default:

		printf("?");

		break;

	}

}

calc(a, b)

int a, b;

{

	ea[0] = e[a][0];

	ea[1] = e[a][1];

	ea[2] = e[a][2];

	ea[3] = e[a][3];

	eb[0] = e[b][0];

	eb[1] = e[b][1];

	eb[2] = e[b][2];

	eb[3] = e[b][3];

	Feb[0] = F[0][0] * eb[0] + F[0][1] * eb[1] + F[0][2] * eb[2] + F[0][3] * eb[3];

	Feb[1] = F[1][0] * eb[0] + F[1][1] * eb[1] + F[1][2] * eb[2] + F[1][3] * eb[3];

	Feb[2] = F[2][0] * eb[0] + F[2][1] * eb[1] + F[2][2] * eb[2] + F[2][3] * eb[3];

	Feb[3] = F[3][0] * eb[0] + F[3][1] * eb[1] + F[3][2] * eb[2] + F[3][3] * eb[3];

	FF[a][b] = ea[0] * Feb[0] + ea[1] * Feb[1] + ea[2] * Feb[2] - ea[3] * Feb[3];

}
