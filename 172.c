/* Generate a state table for a packet sender. */

struct {

	int data;

	int fieldwidth;

	int next_state;

} table[5][256];

main()

{

	int i, j;

	for (i = 0; i < 5; i++)

		for (j = 0; j < 256; j++)

			f(i, j);

/*	print_table(); */

	print_dsp_table();

}

f(state, j)

int state, j;

{

	int data, encoded_data, fieldwidth, i, n_ones;

	n_ones = state;

	data = j;

	encoded_data = 0;

	fieldwidth = 8;

	for (i = 0; i < 8; i++) {

		encoded_data <<= 1;

		if (data & 1) {

			encoded_data |= 1;

			if (++n_ones == 5) {

				n_ones = 0;

				encoded_data <<= 1;

				fieldwidth++;

			}

		} else

			n_ones = 0;

		data >>= 1;

	}

	/* left justify data */

	table[state][j].data = encoded_data << (16 - fieldwidth);

	table[state][j].fieldwidth = fieldwidth;

	table[state][j].next_state = n_ones;

}

print_table()

{

	int i, j;

	for (i = 0; i < 5; i++)

		for (j = 0; j < 256; j++)

			printf("state = %d, data = %x, encoded data = %x, field width = %d, next state = %d\n", i, j, table[i][j].data, table[i][j].fieldwidth, table[i][j].next_state);

}

print_dsp_table()

{

	int i, j;

	for (i = 0; i < 5; i++)

		for (j = 0; j < 256; j++)

			printf("%04x%02x\n", table[i][j].data | table[i][j].next_state, table[i][j].fieldwidth);

}
