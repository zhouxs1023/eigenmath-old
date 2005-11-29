/* Test table driven SDLC receiver. */

/* Least significant bit is the first bit sent and received. */

#include <stdio.h>

#include <stdlib.h>

int out_length, in_length;

#define MAX 2000

char stream[MAX], outbuf[MAX], inbuf[MAX];

int send_index, recv_index;

int n_ones;

char test[16][256];

#include "168.c"

main()

{

	int i, j, m = 0, n;

	build_table();

	sendflag();

	for (;;) {

		for (i = 0; i < 100; i++)

			check();

		n = 0;

		for (i = 0; i < 13; i++)

			for (j = 0; j < 256; j++)

				if (test[i][j] == 0 && table[i][j].abort == 0)

					n++;

		if (n == 0 || kbhit())

			break;

		else

			printf("pass %d: %d untested states\n", ++m, n);

	}

	if (n == 0)

		printf("receive state table 100% tested\n");

	else

		for (i = 0; i < 13; i++)

			for (j = 0; j < 256; j++)

				if (test[i][j] == 0 && table[i][j].abort == 0)

					printf("untested state at i = %d, j = 0x%x\n", i, j);

}

check()

{

	int i;

	/* random length */

	out_length = 8 * (random(100) + 4);

	/* random data */

	for (i = 0; i < out_length; i++)

		/* ones bias to improve test coverage */

		if (random(127) < 53)

			outbuf[i] = 0;

		else

			outbuf[i] = 1;

/*	printf("%d", out_length); */

	sendbuf();

	recvbuf();

	if (in_length == out_length) {

		for (i = 0; i < out_length; i++)

			if (outbuf[i] == inbuf[i])

				continue;

			else {

				printf("packet compare error at index = %d\n", i);

				dump();

			}

	} else {

		printf("packet length error\n");

		dump();

	}

}

sendbuf()

{

	int i, n;

	/* opening flag is already in stream */

	for (i = 0; i < out_length; i++) {

		sendbit(outbuf[i]);

		if (n_ones == 5)

		sendbit(0);

	}

	/* closing flag */

	sendflag();

	/* may have to send another flag to prevent queue underrun */

	/* since receiver takes eight bits at a time */

	if (send_index % 8)

		sendflag();

}

sendflag()

{

/*	printf("*"); */

	sendbit(0);

	sendbit(1);

	sendbit(1);

	sendbit(1);

	sendbit(1);

	sendbit(1);

	sendbit(1);

	sendbit(0);

}

sendbit(d)

int d;

{

	stream[send_index++] = d;

	if (send_index == MAX)

		send_index = 0;

	if (d == 1)

		n_ones++;

	else

		n_ones = 0;

}

/*---------------------------------------------------------------------------*/

int recv_state, recv_data;

recvbuf()

{

	int d, i, n;

	/* wait for flag */

	for (;;) {

		if (table[recv_state][recv_data].opening_flag)

			break;

		if (table[recv_state][recv_data].closing_flag) {

			next_state();

			break;

		}

		next_state();

	}

	/* wait for data */

	for (;;) {

		if (table[recv_state][recv_data].fieldwidth)

			break;

		next_state();

	}

	/* shift in data until closing flag */

	in_length = 0;

	for (;;) {

/*		printf("state = %d data = 0x%02x\n", recv_state, recv_data); */

		d = table[recv_state][recv_data].data;

		n = table[recv_state][recv_data].fieldwidth;

		for (i = 0; i < n; i++) {

			inbuf[in_length++] = d & 1;

			d >>= 1;

			if (in_length == MAX) {

				printf("error: in length exceeded\n");

				exit(1);

			}

		}

		if (table[recv_state][recv_data].closing_flag)

			break;

		next_state();

		if (table[recv_state][recv_data].opening_flag)

			break;

	}

}

next_state()

{

	int d, i, n;

	recv_state = table[recv_state][recv_data].next_state;

	recv_data = recv8();

	/* set test coverage flag */

	test[recv_state][recv_data] = 1;
/*
	if (table[recv_state][recv_data].opening_flag)

		printf("<");

	d = table[recv_state][recv_data].data;

	n = table[recv_state][recv_data].fieldwidth;

	for (i = 0; i < n; i++) {

		printf("%d", d & 1);

		d >>= 1;

	}

	if (table[recv_state][recv_data].closing_flag)

		printf(">");

	printf("\n");
*/
}

recv8()

{

	int i, d = 0;

	for (i = 0; i < 8; i++) {

		d >>= 1;

		if (recvbit())

			d |= 0x80;

	}

	return d;

}

recvbit()

{

	int d;

	if (recv_index == send_index) {

		printf("error: queue underrun\n");

		dump();

	}

	d = stream[recv_index++];

	if (recv_index == MAX)

		recv_index = 0;

	return d;

}

dump()

{

	int i;

	printf("send packet length = %d bits\n", out_length);

	printf("receive packet length = %d bits\n", in_length);

	printf("outbuf =");

	for (i = 0; i < out_length; i++) {

		if (i % 72 == 0)

			printf("\n");

		printf("%d", outbuf[i]);

	}

	printf("\n");

	printf("inbuf =");

	for (i = 0; i < in_length; i++) {

		if (i % 72 == 0)

			printf("\n");

		printf("%d", inbuf[i]);

	}

	printf("\n");

	printf("outbuf ^ inbuf =");

	for (i = 0; i < out_length; i++) {

		if (i % 72 == 0)

			printf("\n");

		printf("%d", outbuf[i] ^ inbuf[i]);

	}

	printf("\n");

	printf("stream =");

	for (i = 0; i < MAX; i++) {

		if (i % 72 == 0)

			printf("\n");

		printf("%d", stream[i]);

	}

	printf("\n");

	exit(1);

}
