/* Compute checksum for Intel Hex 86 files. */

/* caveats */

/* 1  adds all data bytes, even if they overlap or are outside eprom */

/* 2  assumes that unused eprom bytes contain zero */

#include <stdio.h>

char str[256];

main()
{
	int i;
	char *s;
	unsigned int addr, bc, cs, d, seg, type, sum = 0;

	while (fgets(str, 256, stdin)) {

		if (*str != ':')
			continue;

		sscanf(str + 1, "%02x%04x%02x", &bc, &addr, &type);

		if (type == 2)
			sscanf(str + 9, "%04x", &seg);

		if (type == 0) {
			printf(".");
			s = str + 9;
			cs = bc + (addr >> 8) + (addr & 0xff) + type;
			for (i = 0; i < bc; i++) {
				sscanf(s, "%02x", &d);
				cs += d;
				sum += d;
				s += 2;
			}
			sscanf(s, "%02x", &d);
			cs += d;
			if (cs & 0xff) {
				printf("bad record checksum\n");
				printf("%s", str);
				exit();
			}
		}
	}

	printf("\n%04x\n", sum);
}
