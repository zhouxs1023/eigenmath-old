// This is a test program that reads and writes large files.
// I used it to debug an NFS problem on a PPC board.

#include <stdio.h>

main()
{
	int i, n = 0, nn;
	FILE *f;
	while (1) {
		n++;
		f = fopen("hello", "w");
		for (i = 0; i < 1000000; i++)
			fprintf(f, "%d\n", n);
		fclose(f);
		f = fopen("hello", "r");
		for (i = 0; i < 1000000; i++) {
			fscanf(f, "%d", &nn);
			if (n == nn)
				continue;
			printf("failed\n");
			exit(1);
		}
		fclose(f);
		printf("%d\n", n);
	}
}
