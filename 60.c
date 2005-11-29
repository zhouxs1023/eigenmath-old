/* Simulate the frame count alignment algorithm.
This algorithm is used to implement the Bonding protocol. */

#include <stdio.h>
#include <stdlib.h>

struct {
	int fc;
	int fc_align_count;
	int fc_align_flag;
	int fc_delta;
} dsp_status[32];

int dsp_data;

#define FC 0

main()
{
	int i;
	for (i = 0; i < 64; i++) {
		test(i);
		printf(".");
	}
}

test(delta)
int delta;
{
	int i, f;

	f = random(256);

	dsp_status[0].fc = 0;
	dsp_status[0].fc_delta = -1;
	dsp_status[0].fc_align_flag = 0;
	dsp_status[0].fc_align_count = 0;

	for (i = 0; i < 300; i++) {

		f++;

		dsp_data = (f << 8) | (((f - delta) & 0x3f) << 1) | 0x81;

		check_fc_align();

		if (i < 3 && dsp_status[0].fc_align_flag == 1) {
			printf("false alignment\n");
			exit(0);
		}

		if (i >= 3 && dsp_status[0].fc_align_flag == 0) {
			printf("alignment failure\n");
			exit(0);
		}

		if (i >= 3 && dsp_status[0].fc_delta != delta) {
			printf("delta failure\n");
			exit(0);
		}
	}
}

read_dsp()
{
	return dsp_data;
}

check_fc_align()
{
	int i;
	unsigned int d, n;

	for (i = 0; i < 32; i++) {

		n = read_dsp(FC + i);

		if (n == dsp_status[i].fc)
			continue;

		dsp_status[i].fc = n;

		d = ((n >> 8) - (n >> 1)) & 0x3f;

		if (d == dsp_status[i].fc_delta)
			++dsp_status[i].fc_align_count;
		else
			--dsp_status[i].fc_align_count;

		if (dsp_status[i].fc_align_count >= 3) {
			dsp_status[i].fc_align_count = 3;
			dsp_status[i].fc_align_flag = 1;
		}

		if (dsp_status[i].fc_align_count <= 0) {
			dsp_status[i].fc_align_count = 0;
			dsp_status[i].fc_align_flag = 0;
			dsp_status[i].fc_delta = d;
		}
	}
}
