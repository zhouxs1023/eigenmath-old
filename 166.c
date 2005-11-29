/* Build operation code list from SDLC receive state table. */
/* Least significant bit is the first bit received. */
/* This program is superceded by number 167. */

main()

{

	build_table();

	get_op_codes();

}

/*---------------------------------------------------------------------------*/

struct y {

	int opening_flag;

	int data;

	int fieldwidth;

	int closing_flag;

	int abort;

	int format_error;

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

	printf("%d abort states\n", n);

	/* count format error states */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].format_error)

				n++;

	printf("%d format error states\n", n);

	/* maximum field width */

	n = 0;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			if (table[i][j].fieldwidth > n)

				n = table[i][j].fieldwidth;

	printf("maximum field width = %d bits\n", n);

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

		if (fieldwidth >= 7 && (data & 0x7f) == 0x7f) {

			table[state][j].abort = 1;

			data = (data >> 7) & 0x01ff;

			fieldwidth -= 7;

			continue;

		}

		/* A format error occurs when 01111110111110 is received. */

		/* The middle zero is a stuffed bit that is removed. */

		/* That leaves a string of six ones but it is not a flag. */

		/* format error? */

		if (fieldwidth >= 6 && (data & 0x3f) == 0x3f) {

			table[state][j].format_error = 1;

			data = (data >> 6) & 0x03ff;

			fieldwidth -= 6;

			continue;

		}

		/* stuffed bit? */

		if (fieldwidth >= 6 && (data & 0x3f) == 0x1f) {

			table[state][j].data = (table[state][j].data >> 5) | 0xf800;

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

	table[state][j].data = table[state][j].data >> (16 - table[state][j].fieldwidth);

	/* next state */

	table[state][j].next_state = next_state;

	/* clean up if abort or format error */

	if (table[state][j].abort || table[state][j].format_error) {

		table[state][j].data = 0;

		table[state][j].fieldwidth = 0;

	}

}

/*---------------------------------------------------------------------------*/

int code[5][13];

int ncode;

get_op_codes()

{

	int i, j;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			g(i, j);

	for (i = 0; i < 5; i++)

		for (j = 0; j < 13; j++)

			if (code[i][j])

				print_op_code(i, j);

}

g(i, j)

int i, j;

{

	int op, x = 0;

	if (table[i][j].opening_flag)

		x |= 0x01;

	if (table[i][j].closing_flag)

		x |= 0x02;

	if (table[i][j].abort)

		x |= 0x04;

	if (table[i][j].format_error)

		x |= 0x08;

	if (x == 0x00)

		op = 0;

	else if (x == 0x01)

		op = 1;

	else if (x == 0x02)

		op = 2;

	else if (x == 0x04)

		op = 3;

	else if (x == 0x08)

		op = 4;

	else if (x == 0x09)

		op = 1;		/* flag followed by format error */

	else if (x == 0x05)

		op = 1;		/* flag followed by abort */

	else if (x == 0x0c)

		op = 3;		/* format error and abort */

	else {

		printf("bad operation code\n");

		printf("receive state = %d\n", i);

		printf("receive data = 0x%x\n", j);

		printf("opening flag = %d\n", table[i][j].opening_flag);

		printf("closing flag = %d\n", table[i][j].closing_flag);

		printf("abort = %d\n", table[i][j].abort);

		printf("format_error = %d\n", table[i][j].format_error);

		exit(1);

	}

	x = table[i][j].fieldwidth;

	if (x > 12) {

		printf("bad field width\n");

		printf("receive state = %d\n", i);

		printf("receive data = 0x%x\n", j);

		printf("field width = %d\n", x);

		exit(1);

	}

	code[op][x] = 1;

}

print_op_code(i, j)

int i, j;

{

	switch (i) {

	case 0:

		printf("%d data bits\n", j);

		break;

	case 1:

		printf("opening flag followed by %d data bits\n", j);

		break;

	case 2:

		printf("%d data bits followed by closing flag\n", j);

		break;

	case 3:

		printf("abort, field width = %d\n", j);

		break;

	case 4:

		printf("format error, field width = %d\n", j);

		break;

	}

}
