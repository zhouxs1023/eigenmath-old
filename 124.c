/* Simulate bonding delay calculation algorithm. */

#include <stdio.h>
#include <stdlib.h>

char frame[16384];

int delay[8] = {
	0,
	1,
	2,
	3,
	8188,
	8189,
	8190,
	8191,
};

main()
{
	int i, n, d1, d2, d3, fc1, fc2, fc3, t1, t2, t3, x1, x2, x3;

	long k = 0L;

	randomize();

	/* initialize frame */

	for (i = 0; i < 16384; i++)
		frame[i] = -1;

	/* frame count */

	for (i = 0; i < 64; i++)
		frame[256 * i + 191] = i;

	for (;;) {

		if (kbhit())
			exit(0);

		if (k % 1000L == 0)
			printf("%ld laps\n", k);

		k++;

		/* random delay */

		d1 = random(8192);

		d2 = random(8192);

		d3 = random(8192);

/*		d1 = delay[random(8)];

		d2 = delay[random(8)];

		d3 = delay[random(8)];
*/
		/* random starting point */

		x1 = t1 = random(16384);

		x2 = t2 = random(16384);

		x3 = t3 = random(16384);

		/* find sync */

		for (;;)
			if (frame[(d1 + t1 - 1) & 0x3fff] > -1)
				break;
			else
				t1 = (t1 + 1) & 0x3fff;

		for (;;)
			if (frame[(d2 + t2 - 2) & 0x3fff] > -1)
				break;
			else
				t2 = (t2 + 1) & 0x3fff;

		for (;;)
			if (frame[(d3 + t3 - 3) & 0x3fff] > -1)
				break;
			else
				t3 = (t3 + 1) & 0x3fff;

		/* frame count */

		fc1 = frame[(d1 + t1 - 1) & 0x3fff];

		fc2 = frame[(d2 + t2 - 2) & 0x3fff];

		fc3 = frame[(d3 + t3 - 3) & 0x3fff];

		/* delay calculation */

		t1 = (t1 - 256 * fc1 - 1) & 0x3fff;

		t2 = (t2 - 256 * fc2 - 2) & 0x3fff;

		t3 = (t3 - 256 * fc3 - 3) & 0x3fff;

		/* normalize */

		n = t1;

		if (t2 < n)
			n = t2;

		if (t3 < n)
			n = t3;

		t1 -= n;

		t2 -= n;

		t3 -= n;

		/* extend sign */

		if (t1 & 0x2000)
			t1 |= 0xc000;

		if (t2 & 0x2000)
			t2 |= 0xc000;

		if (t3 & 0x2000)
			t3 |= 0xc000;

		/* normalize */

		n = t1;

		if (t2 > n)
			n = t2;

		if (t3 > n)
			n = t3;

		t1 -= n;

		t2 -= n;

		t3 -= n;

		/* check */

		if (d1 - d2 == t2 - t1 && d2 - d3 == t3 - t2 && d3 - d1 == t1 - t3)
			;
		else {
			printf("fail\n\n");

			printf("d1=%d\n", d1);
			printf("d2=%d\n", d2);
			printf("d3=%d\n\n", d3);

			printf("d1-d2=%d\n", d1 - d2);
			printf("d2-d3=%d\n", d2 - d3);
			printf("d3-d1=%d\n\n", d3 - d1);

			printf("x1=%d\n", x1);
			printf("x2=%d\n", x2);
			printf("x3=%d\n\n", x3);

			printf("t1=%d\n", t1);
			printf("t2=%d\n", t2);
			printf("t3=%d\n\n", t3);

			printf("t2-t1=%d\n", t2 - t1);
			printf("t3-t2=%d\n", t3 - t2);
			printf("t1-t3=%d\n\n", t1 - t3);

			printf("n=%d\n", n);

			exit(0);
		}
	}
}
