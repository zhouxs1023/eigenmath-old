/* Convert Analog Devices DSP object file to C header file.
Reorder bytes so they can be read sequentially during boot. */

#include <stdio.h>

int mem[8192];
char str[500];

main()
{
	read_intel_hex_file();

	write_include_file();
}

read_intel_hex_file()
{
	int chk, d, i, n;
	unsigned int addr;
	char *s;

	while (fgets(str, 500, stdin) != NULL) {

		sscanf(str + 1, "%02x%04x%02x", &n, &addr, &d);

		chk = (n + (addr & 0xff) + (addr >> 8) + d) & 0xff;

		s = str + 9;

		for (i = 0; i < n; i++) {
			sscanf(s, "%02x", &d);
			mem[addr++] = d;
			s += 2;
			chk = (chk + d) & 0xff;
		}

		sscanf(s, "%02x", &d);

		chk = (chk + d) & 0xff;

		if (chk != 0) {
			printf("checksum failed\n");
			exit();
		}
	}
}

write_include_file()
{
	int a;

	printf("const unsigned char dsp[] = {\n");

	/* number of pages */

	printf("\t0x%02x,\n", mem[3]);

	/* thirty-two bytes per page */

	a = 32 * (mem[3] + 1);

	while (a > 0) {

		printf("\t0x%02x,0x%02x,0x%02x,\n", mem[a - 4], mem[a - 2], mem[a - 3]);

		a -= 4;
	}

	printf("};\n");

}
