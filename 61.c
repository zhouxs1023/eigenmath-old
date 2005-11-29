/* Simulate FAW hunt algorithm used to implement Bonding protocol. */

#include <stdio.h>
#include <stdlib.h>

int t;
int min = 1000;
int max = 0;

int frame[256];
unsigned int offset;
unsigned int delay;
int dsp_faw;
int dsp_hunt;
unsigned int dsp_frame_count;

struct {
	int faw;
	int faw_align_count;
	int faw_align_flag;
	int hunt;
	unsigned int delay;
} dsp_status[32];

#define HUNT 1000
#define FAW 2000

main()
{
	int i;

	randomize();

	for (i = 0; i < 256; i++)
		frame[i] = 0xff;

	frame[64] = 0x1b;

	for (i = 0; i < 10000; i++) {
		printf("%d ", i + 1);
		test();
		if (kbhit())
			break;
	}

	printf("min=%d ms\n", 10 * min);
	printf("max=%d ms\n", 10 * max);
}

test()
{
	int i;

	dsp_status[0].hunt = 0;
	dsp_status[0].faw = 0;
	dsp_status[0].faw_align_count = 0;
	dsp_status[0].faw_align_flag = 0;
	dsp_status[0].delay = 0;

	delay = 0;

	dsp_faw = 0;
	dsp_hunt = 0;
	dsp_frame_count = rand();

	offset = rand();

	for (t = 0; t < 100; t++) {

		for (i = 0; i < 80; i++)

			simulate_dsp();

		simulate_68000();

		if (dsp_status[0].faw_align_flag) {
			printf("alignment found in %d ms\n", 10 * t);
			if (t < min)
				min = t;
			if (t > max)
				max = t;
			return;
		}
	}

	printf("alignment failed\n");
}

simulate_dsp()
{
	int d;

	dsp_frame_count = dsp_frame_count + 1;

	d = frame[(dsp_frame_count + offset - delay) & 0xff];

	if (d == 0x1b)

		dsp_hunt = dsp_frame_count;

	if ((dsp_frame_count & 0xff) == 0)

		dsp_faw = d | (dsp_frame_count & 0xff00);
}

read_dsp(n)
int n;
{
	switch (n) {

	case HUNT:
		return dsp_hunt;

	case FAW:
		return dsp_faw;

	default:
		return 0;
	}
}

set_delay(n)
int n;
{
	if (n == 0) {
		delay = dsp_status[n].delay;
		printf("d");
	}
}

simulate_68000()
{
	printf(".");

	check_faw_align();

	check_align_delay();
}

check_faw_align()
{
	int i, n;

	for (i = 0; i < 32; i++) {

		n = read_dsp(FAW + i) | 0x01;

		if (n == dsp_status[i].faw)
			continue;

		dsp_status[i].faw = n;

		if ((n & 0xff) == 0x1b)
			++dsp_status[i].faw_align_count;
		else
			--dsp_status[i].faw_align_count;

		if (dsp_status[i].faw_align_count >= 3) {
			dsp_status[i].faw_align_count = 3;
			dsp_status[i].faw_align_flag = 1;
		}

		if (dsp_status[i].faw_align_count <= 0) {
			dsp_status[i].faw_align_count = 0;
			dsp_status[i].faw_align_flag = 0;
		}
	}
}

/* example: if hunt delay is 10 then add enough delay to make it 256 */

check_align_delay()
{
	int i, n;
	unsigned int d;

	for (i = 0; i < 32; i++) {

		n = read_dsp(HUNT + i);

		if (n == dsp_status[i].hunt)
			continue;

		dsp_status[i].hunt = n;

		d = n & 0xff;

		if (d == 0)
			continue;

		if (dsp_status[i].faw_align_count == 0) {
			dsp_status[i].delay = (dsp_status[i].delay - d) & 0xff;
			set_delay(i);
		}
	}
}
