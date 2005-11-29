/* Scan the file ``210x.map'' and emit the file ``dsp.equ.'' */

#include <stdio.h>

char str[1000];

char label[100], value[100];

main()

{

	int i;

	FILE *f, *g;

	f = fopen("210x.map", "r");

	g = fopen("dsp.equ", "w");

	/* skip first eight lines */

	for (i = 0; i < 8; i++)

		if (fgets(str, 1000, f) == NULL)

			error();

	for (;;) {

		if (fgets(str, 1000, f) == NULL)

			error();

		/* done if blank line */

		if (*str == '\n')

			break;

		/* scan label */

		sscanf(str + 5, "%s", label);

		/* skip if label begins with a Y */

		if (*label == 'Y')

			continue;

		/* scan value */

		sscanf(str + 40, "%s", value);

		/* emit assembler statement */

		fprintf(g, "dsp_%s equ 0%sh\n", label, value);

	}

}

error()

{

	fprintf(stderr, "unexpected end of file\n");

	exit();

}
