/* Build operation code list from SDLC receive state table. */
/* Least significant bit is the first bit received. The format */
/* error pattern 01111110111110 is treated as five one bits followed */
/* by a stuffed bit followed by an abort sequence. */
/* (It can also be interpreted as a format error followed by a flag sequence. */
/* The format error is due to the lack of a stuffed zero after the five ones.) */

main()

{

	build_table();

	get_op_codes();

}

#include "168.c"

/*---------------------------------------------------------------------------*/

int code[6][13];

int ncode;

get_op_codes()

{

	int i, j;

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			g(i, j);

	for (i = 0; i < 6; i++)

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

	if (x == 0x00)

		op = 0;		/* data */

	else if (x == 0x01)

		op = 1;		/* opening flag followed by data */

	else if (x == 0x02)

		op = 2;		/* data follwed by closing flag */

	else if (x == 0x04)

		op = 3;		/* abort */

	else if (x == 0x05)

		op = 4;		/* flag followed by abort */

	else {

		printf("bad operation code\n");

		printf("receive state = %d\n", i);

		printf("receive data = 0x%x\n", j);

		printf("opening flag = %d\n", table[i][j].opening_flag);

		printf("closing flag = %d\n", table[i][j].closing_flag);

		printf("abort = %d\n", table[i][j].abort);

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

		if (j == 0)

			printf("flag\n");

		else

			printf("flag followed by %d data bits\n", j);

		break;

	case 2:

		printf("%d data bits followed by flag\n", j);

		break;

	case 3:

		printf("abort, field width = %d\n", j);

		break;

	case 4:

		printf("flag followed by abort, field width = %d\n", j);

		break;

	}

}
