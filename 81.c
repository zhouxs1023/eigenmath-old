/* Check to see if any Intel Hex records overlap. */

#include <stdio.h>

#define MAX 20000

char buf[500], mem[MAX];

main()

{

	int addr, len, i;

	while (fgets(buf, 500, stdin) != NULL) {

		sscanf(buf + 1, "%02x%04x", &len, &addr);

		printf("len = %d addr = %d\n", len, addr);

		for (i = 0; i < len; i++) {

			if (addr + i >= MAX || mem[addr + i]) {

				printf("error\n");

				exit(1);

			}

			mem[addr + i] = 1;

		}

	}

}
