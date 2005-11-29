/* Simulate FIFO depth. */

#include <stdio.h>

main()

{

	int depth, yread, ywrite;

	for (yread = 0; yread < 8; yread++)

		for (ywrite = 0; ywrite < 8; ywrite++) {

			printf("read = %d\n", yread);

			printf("write = %d\n", ywrite);

			depth = (ywrite - yread) & 7;

			printf("depth = %d\n", depth);

			if (graph(yread, ywrite) != depth) {

				printf("error\n");

				exit();

			}

		}

}

graph(yread, ywrite)

int yread, ywrite;

{

	int depth = 0, i;

	if (yread <= ywrite)

		for (i = 0; i < 8; i++)

			if (i >= yread && i < ywrite) {

				printf("[xxxxx]\n");

				depth++;

			} else

				printf("[     ]\n");

	else

		for (i = 0; i < 8; i++)

			if (i >= yread || i < ywrite) {

				printf("[xxxxx]\n");

				depth++;

			} else

				printf("[     ]\n");

	return depth;

}
