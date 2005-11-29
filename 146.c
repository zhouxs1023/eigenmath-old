/* Verify an algorithm for determining when a FIFO is emptied. */

main()

{

	int i, depth, y, yread, yread2, ywrite;

	for (yread = 0; yread < 127; yread++)

		for (depth = 0; depth < 63; depth++) {

			ywrite = (yread + depth) & 0x7f;

			for (i = 0; i <= depth; i++) {

				yread2 = (yread + i) & 0x7f;

				/* bit 6 is like a sign bit */

				/* bit 7 is like a carry bit */

				y = (ywrite - yread2) & 0xc0;

				if (y == 0x00 || y == 0xc0)

					fail(yread, depth, i);

			}

			for (i = depth + 1; i < 127; i++) {

				yread2 = (yread + i) & 0x7f;

				/* bit 6 is like a sign bit */

				y = (ywrite - yread2) & 0xc0;

				if (y == 0x40 || y == 0x80)

					fail(yread, depth, i);

			}

		}

}

fail(yread, depth, i)

int yread, depth, i;

{

	printf("fail\n");

	printf("yread = %d\n", yread);

	printf("depth = %d\n", depth);

	printf("i = %d\n", i);

	exit();

}
