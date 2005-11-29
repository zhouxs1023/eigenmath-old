/* Scan a file dumped form the DSP simulator. The file contains
HDLC sent in a T1 time slot. Scan and decode the HDLC packets. */

main()

{

	int i, n = 0;

	int data, queue = 0;

	for (i = 0; i < 800; i++) {

		if (n < 8) {

			scanf("%x", &data);

			data = reverse(data);

			queue |= data << n;

			n += 8;

		}

		if ((queue & 0xff) == 0x7e) {

			queue = (queue >> 8) & 0xff;

			n -= 8;

		} else if ((queue & 0x3f) == 0x3f) {

			printf("abort\n");

			exit(1);

		} else if ((queue & 0x3f) == 0x1f) {

			queue = (queue >> 6) & 0x3ff;

			n -= 6;

			out(1);

			out(1);

			out(1);

			out(1);

			out(1);

		} else {

			out(queue & 1);

			queue = (queue >> 1) & 0x7fff;

			n--;

		}

	}

}

int n, q;

out(bit)

int bit;

{

	q |= bit << 8;

	q >>= 1;

	if (++n == 8) {

		n = 0;

		printf("%c", q);

	}

}

/* reverse the order of the bits */

reverse(x)

int x;

{

	int y = 0;

	if (x & 0x01)

		y |= 0x80;

	if (x & 0x02)

		y |= 0x40;

	if (x & 0x04)

		y |= 0x20;

	if (x & 0x08)

		y |= 0x10;

	if (x & 0x10)

		y |= 0x08;

	if (x & 0x20)

		y |= 0x04;

	if (x & 0x40)

		y |= 0x02;

	if (x & 0x80)

		y |= 0x01;

	return y;

}
