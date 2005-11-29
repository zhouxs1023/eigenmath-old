/* verify bonding equations for deriving rmult and submult */

#include <stdio.h>

int mode;

long a, bcr, rmult, submult;

main()

{

	for (mode = 0; mode < 4; mode++) {

		bcr = 56000;

		test();

		bcr = 64000;

		test();

	}

}

test()

{

	long i, min, max, step;

	switch (mode) {

	case 0:

	case 1:

		min = bcr;

		max = 32 * bcr;

		step = bcr;

		break;

	case 2:

		min = bcr * 63 / 64;

		max = 32 * bcr * 63 / 64;

		step = bcr * 63 / 64;

		break;

	case 3:

		min = 8000;

		max = 32 * bcr;

		step = 8000;

		break;

	}

	for (i = min; i <= max; i += step) {

		a = i;

		to_rmult();

		to_a();

		printf("mode=%d bcr=%ld i=%ld a=%ld rmult=%ld submult=%ld\n", mode, bcr, i, a, rmult, submult);

		if (a != i) {

			printf("fail\n");

			exit();

		}

	}

}

/* convert bearer channel rate and application rate to rmult and submult */

to_rmult()

{

	switch (mode) {

	case 0:

	case 1:

		rmult = a / bcr;

		submult = 7;

		break;

	case 2:

		rmult = a * 64 / bcr / 63;

		submult = 7;

		break;

	case 3:

		rmult = a / bcr;

		submult = (a - bcr * rmult) / 8000;

		break;

	}

}

/* convert bcr, rmult and submult to application rate */

to_a()

{

	switch (mode) {

	case 0:

	case 1:

		a = bcr * rmult;

		break;

	case 2:

		a = bcr * rmult * 63 / 64;

		break;

	case 3:

		a = bcr * rmult + 8000 * submult;

		break;

	}

}
