/* Generate a receive packet state matrix for Analog Devices DSP. */

/* operation code format */

/* nnnn dddd dddd dddd xxxx yyyy */

/* n = four bit next state field */

/* d = twelve bit data field */

/* x = four bit primary function code field */

/* y = four bit secondary function code field */

/* function	meaning */

/* 	0	no operation */

/*	1	1 data bit */

/*	2	2 data bits */

/*	3	3 data bits */

/*	4	4 data bits */

/*	5	5 data bits */

/*	6	6 data bits */

/*	7	7 data bits */

/*	8	8 data bits */

/*	9	9 data bits */

/*	10	10 data bits */

/*	11	11 data bits */

/*	12	12 data bits */

/*	13	flag */

/*	14	abort */

#include "168.c"

main()

{

	int i, j;

	build_table();

	for (i = 0; i < 13; i++)

		for (j = 0; j < 256; j++)

			emit_opcode(i, j);

}

emit_opcode(i, j)

int i, j;

{

	int op = 0;

	int data, fieldwidth, next_state;

	fieldwidth = table[i][j].fieldwidth;

	data = table[i][j].data & ((1 << fieldwidth) - 1);

	next_state = table[i][j].next_state;

	if (next_state < 0 || next_state > 12)

		dump(i, j);

	if (table[i][j].opening_flag)

		op |= 0x01;

	if (table[i][j].closing_flag)

		op |= 0x02;

	if (table[i][j].abort)

		op |= 0x04;

	if (op == 0x00)

		/* data */

		if (fieldwidth < 1 || fieldwidth > 12)

			dump (i, j);

		else

			emit(next_state, data, fieldwidth, 0);

	else if (op == 0x01)

		/* opening flag followed by data */

		if (fieldwidth < 0 || fieldwidth > 6)

			dump(i, j);

		else

			emit(next_state, data, 13, fieldwidth);

	else if (op == 0x02)

		/* data follwed by closing flag */

		if (fieldwidth < 1 || fieldwidth > 5)

			dump(i, j);

		else

			emit(next_state, data, fieldwidth, 13);

	else if (op == 0x04)

		/* abort */

		emit(next_state, 0, 14, 0);

	else if (op == 0x05)

		/* flag followed by abort */

		if (fieldwidth)

			dump(i, j);

		else

			emit(next_state, 0, 13, 14);

	else

		dump (i, j);

}

dump(i, j)

int i, j;

{

	printf("receive state = %d\n", i);

	printf("receive data = 0x%x\n", j);

	printf("opening flag = %d\n", table[i][j].opening_flag);

	printf("field width = %d\n", table[i][j].fieldwidth);

	printf("data = 0x%x\n", table[i][j].data);

	printf("closing flag = %d\n", table[i][j].closing_flag);

	printf("abort = %d\n", table[i][j].abort);

	exit(1);

}

emit(n, d, x, y)

int n, d, x, y;

{

	printf("%x%03x%x%x\n", n, d, x, y);

}
