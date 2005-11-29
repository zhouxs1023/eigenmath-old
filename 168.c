/* This is an include file that builds an SDLC receive state table. */
/* Least significant bit is the first bit received. The format */
/* error pattern 01111110111110 is treated as five one bits followed */
/* by a stuffed bit followed by an abort sequence. */
/* (It can also be interpreted as a format error followed by a flag sequence. */
/* The format error is due to the lack of a stuffed zero after the five ones.) */

struct {

	int opening_flag;

	int data;

	int fieldwidth;

	int closing_flag;

	int abort;

	int next_state;

} table[13][256];

build_table()

{

	int i, j, n;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			f(i, j);

	/* count abort states */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].abort)

				n++;

/*	printf("%d abort states\n", n); */

	/* maximum field width */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].fieldwidth > n)

				n = table[i][j].fieldwidth;

/*	printf("maximum field width = %d bits\n", n); */

}

#define FLAG 0x7e

#define DATA1 0x00	/* 0 */

#define DATA2 0x02	/* 10 */

#define DATA3 0x06	/* 110 */

#define DATA4 0x0e	/* 1110 */

#define DATA5 0x1e	/* 1 1110 */

#define DATA6 0x3e	/* 11 1110 */

#define DATA7 0x7e	/* 111 1110 */

#define DATA8 0x01	/* 1 */

#define DATA9 0x03	/* 11 */

#define DATA10 0x07	/* 111 */

#define DATA11 0x0f	/* 1111 */

#define DATA12 0x1f	/* 1 1111 */

f(state, j)

int state, j;

{

	int data, fieldwidth, next_state;

	data = j;

	j = reverse(j);

	/* extend bit field */

	switch (state) {

	case 0:

		fieldwidth = 8;

		break;

	case 1:

		data = (data << 1) | DATA1;

		fieldwidth = 9;

		break;

	case 2:

		data = (data << 2) | DATA2;

		fieldwidth = 10;

		break;

	case 3:

		data = (data << 3) | DATA3;

		fieldwidth = 11;

		break;

	case 4:

		data = (data << 4) | DATA4;

		fieldwidth = 12;

		break;

	case 5:

		data = (data << 5) | DATA5;

		fieldwidth = 13;

		break;

	case 6:

		data = (data << 6) | DATA6;

		fieldwidth = 14;

		break;

	case 7:

		data = (data << 7) | DATA7;

		fieldwidth = 15;

		break;

	case 8:

		data = (data << 1) | DATA8;

		fieldwidth = 9;

		break;

	case 9:

		data = (data << 2) | DATA9;

		fieldwidth = 10;

		break;

	case 10:

		data = (data << 3) | DATA10;

		fieldwidth = 11;

		break;

	case 11:

		data = (data << 4) | DATA11;

		fieldwidth = 12;

		break;

	case 12:

		data = (data << 5) | DATA12;

		fieldwidth = 13;

		break;

	}

	/* opening flag? */

	if ((data & 0xff) == FLAG) {

		table[state][j].opening_flag = 1;

		data = (data >> 8) & 0xff;

		fieldwidth -= 8;

	}

	/* scan the bit field */

	for (;;) {

		/* need more bits to make a decision? */

		if (fieldwidth == 0) {

			next_state = 0;

			break;

		}

		if (fieldwidth == 1 && data == DATA1) {

			next_state = 1;

			break;

		}

		if (fieldwidth == 2 && data == DATA2) {

			next_state = 2;

			break;

		}

		if (fieldwidth == 3 && data == DATA3) {

			next_state = 3;

			break;

		}

		if (fieldwidth == 4 && data == DATA4) {

			next_state = 4;

			break;

		}

		if (fieldwidth == 5 && data == DATA5) {

			next_state = 5;

			break;

		}

		if (fieldwidth == 6 && data == DATA6) {

			next_state = 6;

			break;

		}

		if (fieldwidth == 7 && data == DATA7) {

			next_state = 7;

			break;

		}

		if (fieldwidth == 1 && data == DATA8) {

			next_state = 8;

			break;

		}

		if (fieldwidth == 2 && data == DATA9) {

			next_state = 9;

			break;

		}

		if (fieldwidth == 3 && data == DATA10) {

			next_state = 10;

			break;

		}

		if (fieldwidth == 4 && data == DATA11) {

			next_state = 11;

			break;

		}

		if (fieldwidth == 5 && data == DATA12) {

			next_state = 12;

			break;

		}

		/* closing flag? */

		if (fieldwidth >= 8 && (data & 0xff) == FLAG) {

			if (fieldwidth > 8) {

				printf("error: bad remainder\n");

				exit(1);

			}

			table[state][j].closing_flag = 1;

			next_state = 0;

			break;

		}

		/* abort? */

		if (fieldwidth >= 6 && (data & 0x3f) == 0x3f) {

			table[state][j].abort = 1;

			do {

				data = (data >> 1) & 0x7fff;

				fieldwidth--;

			} while (data & 1);

			continue;

		}

		/* stuffed bit? */

		if (fieldwidth >= 6 && (data & 0x3f) == 0x1f) {

			table[state][j].data >>= 5;

			table[state][j].data |= 0xf800;

			table[state][j].fieldwidth += 5;

			data = (data >> 6) & 0x03ff;

			fieldwidth -= 6;

		} else {

			table[state][j].data = (table[state][j].data >> 1) & 0x7fff;

			if (data & 1)

				table[state][j].data |= 0x8000;

			table[state][j].fieldwidth++;

			data = (data >> 1) & 0x7fff;

			fieldwidth--;

		}

	}

	/* right justify data */

	table[state][j].data = (table[state][j].data >> (16 - table[state][j].fieldwidth)) & ((1 << table[state][j].fieldwidth) - 1);

	/* next state */

	table[state][j].next_state = next_state;

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
