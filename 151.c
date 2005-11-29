/* This program tests reduction of FXS signalling to combinatorial logic. */

#include <stdio.h>

typedef struct {

	int ring, tg, a, b;	/* a and b are transmit signalling bits */

} OUT[2][2][2][2];

OUT f, g;

#define F f[l][rg][a][b]

#define G g[l][rg][a][b]

#define P (*p)[l][rg][a][b]

/*

ring	ringing voltage

tg	tip ground

a,b	signalling bits

l=1	loop closure

*/

main()

{

	printf("from pub 43801\n");

	function_f();

	printout(f);

	printf("combinatorial logic\n");

	function_g();

	printout(g);

}

function_f()

{

	int l, rg, a, b;	/* a and b are receive signalling bits */

	for (l = 0; l < 2; l++)

	for (rg = 0; rg < 2; rg++)

	for (a = 0; a < 2; a++)

	for (b = 0; b < 2; b++) {

		/* 1 */

		if (l == 0 && rg == 0) {

			assign(&F.a, -1);

			assign(&F.b, 1);

		}

		/* 4 */

		if (a == 1) {

			assign(&F.tg, -1);

			assign(&F.ring, -1);

		}

		/* 5 */

		if (a == 0 && b == 1) {

			assign(&F.tg, 1);

			assign(&F.ring, -1);

		}

		/* 6 */

		if (l == 0 && rg == 0 && a == 0 && b == 0) {

			assign(&F.tg, 1);

			assign(&F.ring, 1);

			assign(&F.a, -1);

			assign(&F.b, 1);

		}

		/* 7 */

		if (l == 1 && a == 0 && b == 0) {

			assign(&F.tg, 1);

			assign(&F.ring, -1);

			assign(&F.a, 1);

			assign(&F.b, 1);

		}

		/* 2 */

		if (rg == 1 && a == 1 && F.tg == -1) {

			assign(&F.a, -1);

			assign(&F.b, -1);

		}

		/* 3 */

		if ((l == 1 || rg == 1) && a == 0 && F.tg == 1) {

			assign(&F.a, 1);

			assign(&F.b, 1);

		}

	}

}

function_g()

{

	int l, rg, a, b;

	for (l = 0; l < 2; l++)

	for (rg = 0; rg < 2; rg++)

	for (a = 0; a < 2; a++)

	for (b = 0; b < 2; b++) {

		if (l == 0 && rg == 0 && a == 0 && b == 0)

			assign(&G.ring, 1);

		else

			assign(&G.ring, -1);

		if (a == 0)

			assign(&G.tg, 1);

		else

			assign(&G.tg, -1);

		if (a == 0 && rg == 1 || a == 0 && l == 1)

			assign(&G.a, 1);

		else

			assign(&G.a, -1);

		if (a == 1 && rg == 1)

			assign(&G.b, -1);

		else

			assign(&G.b, 1);

	}

}

assign(p, v)

int *p, v;

{

	if (*p == 0)

		*p = v;

	else if (*p != v) {

		printf("error");

		exit();

	}

}

printout(p)

OUT *p;

{

	int l, rg, a, b;

	for (l = 0; l < 2; l++)

	for (rg = 0; rg < 2; rg++)

	for (a = 0; a < 2; a++)

	for (b = 0; b < 2; b++) {

		if (l == 0)

			printf("loop=open   ");

		else

			printf("loop=closed ");

		if (rg == 0)

			printf("ringground=no  ");

		else

			printf("ringground=yes ");

		printf("a=%d b=%d ", a, b);

		switch (P.ring) {

		case -1:

			printf("ringing=no  ");

			break;

		case 0:

			printf("ringing=?   ");

			break;

		case 1:

			printf("ringing=yes ");

			break;

		}

		switch (P.tg) {

		case -1:

			printf("tipground=no  ");

			break;

		case 0:

			printf("tipground=?   ");

			break;

		case 1:

			printf("tipground=yes ");

			break;

		}

		switch (P.a) {

		case -1:

			printf("a=0 ");

			break;

		case 0:

			printf("a=? ");

			break;

		case 1:

			printf("a=1 ");

			break;

		}

		switch (P.b) {

		case -1:

			printf("b=0 ");

			break;

		case 0:

			printf("b=? ");

			break;

		case 1:

			printf("b=1 ");

			break;

		}

		printf("\n");

	}

}
