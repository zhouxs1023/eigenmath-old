/* A simple symbolic calculator based on the first sections of Naber. */

#include <stdio.h>

char *displayform[] = {

/* 0 */

	"empty",

/* 1 */

	"e\n 1\n",

/* 2 */

	"e\n 2\n",

/* 3 */

	"e\n 3\n",

/* 4 */

	"e\n 4\n",

/* 5 */

	"^\ne\n 1\n",

/* 6 */

	"^\ne\n 2\n",

/* 7 */

	"^\ne\n 3\n",

/* 8 */

	"^\ne\n 4\n",

/* 9 */

	"       1             2             3             4    \n"
	"Lambda   e  + Lambda   e  + Lambda   e  + Lambda   e  \n"
	"      1   1         1   2         1   3         1   4 \n",

/* 10 */

	"       1             2             3             4    \n"
	"Lambda   e  + Lambda   e  + Lambda   e  + Lambda   e  \n"
	"      2   1         2   2         2   3         2   4 \n",

/* 11 */

	"       1             2             3             4    \n"
	"Lambda   e  + Lambda   e  + Lambda   e  + Lambda   e  \n"
	"      3   1         3   2         3   3         3   4 \n",

/* 12 */

	"       1             2             3             4    \n"
	"Lambda   e  + Lambda   e  + Lambda   e  + Lambda   e  \n"
	"      4   1         4   2         4   3         4   4 \n",

/* 13 */

	"      1             1             1             1     \n"
	"Lambda   e  + Lambda   e  + Lambda   e  - Lambda   e  \n"
	"       1  1          2  2          3  3          4  4 \n",

/* 14 */

	"      2             2             2             2     \n"
	"Lambda   e  + Lambda   e  + Lambda   e  - Lambda   e  \n"
	"       1  1          2  2          3  3          4  4 \n",

/* 15 */

	"      3             3             3             3     \n"
	"Lambda   e  + Lambda   e  + Lambda   e  - Lambda   e  \n"
	"       1  1          2  2          3  3          4  4 \n",

/* 16 */

	"       4             4             4             4     \n"
	"-Lambda   e  - Lambda   e  - Lambda   e  + Lambda   e  \n"
	"        1  1          2  2          3  3          4  4 \n",

/* 17 */

	"                 1             2             3          \n"
	"          Lambda        Lambda        Lambda            \n"
	"       4        4             4             4           \n"
	"Lambda   (-------- e  + -------- e  + -------- e  + e ) \n"
	"      4          4  1          4  2          4  3    4  \n"
	"          Lambda        Lambda        Lambda            \n"
	"                4             4             4           \n",

/* 18 */

	"              1             2             3          \n"
	"       Lambda        Lambda        Lambda            \n"
	"             4             4             4           \n"
	"gamma (-------- e  + -------- e  + -------- e  + e ) \n"
	"              4  1          4  2          4  3    4  \n"
	"       Lambda        Lambda        Lambda            \n"
	"             4             4             4           \n",

/* 19 */

	"       ->       \n"
	"gamma (u  + e ) \n"
	"             4  \n",

/* 20 */

	"            ->       \n"
	"gamma (beta d  + e ) \n"
	"                  4  \n",

/* 21 */

	"       4  ->       \n"
	"Lambda   (u  + e ) \n"
	"      4         4  \n",

/* 22 */

	"       4       ->       \n"
	"Lambda   (beta d  + e ) \n"
	"      4              4  \n",

/* 23 */

	"\332                                                       \277\n"
	"\263       1              1              1              1  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       2              2              2              2  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       3              3              3              3  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       4              4              4              4  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\300                                                       \331\n",

/* 24 */

	"\332                 \277\n"
	"\263  1   0   0   0  \263\n"
	"\263                 \263\n"
	"\263  0   1   0   0  \263\n"
	"\263                 \263\n"
	"\263  0   0   1   0  \263\n"
	"\263                 \263\n"
	"\263  0   0   0  -1  \263\n"
	"\300                 \331\n",

/* 25 */

	"\332                                                            \277\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda          Lambda    \263\n"
	"\263         1               1               1               1  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda          Lambda    \263\n"
	"\263         2               2               2               2  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda          Lambda    \263\n"
	"\263         3               3               3               3  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263 -Lambda         -Lambda         -Lambda         -Lambda    \263\n"
	"\263         4               4               4               4  \263\n"
	"\300                                                            \331\n",

/* 26 */

	"\332                                                       \277\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        1              1              1              1 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        2              2              2              2 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        3              3              3              3 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263        4              4              4              4 \263\n"
	"\300                                                       \331\n",

/* 27 */

	"\332                                                       \277\n"
	"\263        1              1              1              1 \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263       1              2              3              4  \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263        2              2              2              2 \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263       1              2              3              4  \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263        3              3              3              3 \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263       1              2              3              4  \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263        4              4              4              4 \263\n"
	"\263 Lambda         Lambda         Lambda         Lambda   \263\n"
	"\263       1              2              3              4  \263\n"
	"\300                                                       \331\n",

/* 28 */

	"\332                                                            \277\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda         -Lambda    \263\n"
	"\263         1               1               1               1  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda         -Lambda    \263\n"
	"\263         2               2               2               2  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263  Lambda          Lambda          Lambda         -Lambda    \263\n"
	"\263         3               3               3               3  \263\n"
	"\263                                                            \263\n"
	"\263                                                            \263\n"
	"\263        1               2               3               4   \263\n"
	"\263 -Lambda         -Lambda         -Lambda          Lambda    \263\n"
	"\263         4               4               4               4  \263\n"
	"\300                                                            \331\n",

/* 29 */

	"\332                                                       \277\n"
	"\263       1              1              1              1  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       2              2              2              2  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       3              3              3              3  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       4              4              4              4  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        1              2              3              4 \263\n"
	"\300                                                       \331\n",

/* 30 */

	"\332                                                       \277\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        1              1              1              1 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        2              2              2              2 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        3              3              3              3 \263\n"
	"\263                                                       \263\n"
	"\263                                                       \263\n"
	"\263       1              2              3              4  \263\n"
	"\263 Lambda         Lambda         Lambda        -Lambda   \263\n"
	"\263        4              4              4              4 \263\n"
	"\300                                                       \331\n",

};

#define NEQU 17

struct {

	int f, x, y;

} equ[NEQU] = {

	/* 1.2.15 */

	{1, 5, 9},
	{1, 6, 10},
	{1, 7, 11},
	{1, 8, 12},

	/* 1.2.10 */

	{2, 9, 13},
	{2, 10, 14},
	{2, 11, 15},
	{2, 12, 16},
	{2, 27, 28},

	/* factor */

	{3, 12, 17},

	/* 1.3.13 */

	{4, 17, 18},
	{4, 21, 19},
	{4, 22, 20},

	/* 1.3.11 */

	{5, 18, 19},
	{5, 17, 21},

	/* 1.3.14 */

	{6, 19, 20},
	{6, 21, 22},

};

#define NCITATION 7

char *citation[NCITATION] = {
	"?",
	"1.2.15",
	"1.2.10",
	"factor",
	"1.3.13",
	"1.3.11",
	"1.3.14",
};

int ystack[4];

char instring[256];

main()
{

	for (;;) {

		printf("% ");

		gets(instring);

		cmd();

	}

}

cmd()

{

	int i, j;

	/* scan the table of citations */

	for (i = 1; i < NCITATION; i++)

		if (strcmp(instring, citation[i]) == 0) {

			/* citation found */

			/* scan the table of equivalences */

			for (j = 1; j < NEQU; j++) {

				if (equ[j].f == i && equ[j].x == *ystack) {

					*ystack = equ[j].y;

					print_tos();

					return;

				}

				if (equ[j].f == i && equ[j].y == *ystack) {

					*ystack = equ[j].x;

					print_tos();

					return;

				}

			}

			printf("citation does not apply\n");

			return;

		}

	if (strcmp(instring, "^e1") == 0)
		push(5);
	else if (strcmp(instring, "^e2") == 0)
		push(6);
	else if (strcmp(instring, "^e3") == 0)
		push(7);
	else if (strcmp(instring, "^e4") == 0)
		push(8);
	else if (strcmp(instring, "Lambda") == 0)
		push(23);
	else if (strcmp(instring, "eta") == 0)
		push(24);
	else if (strcmp(instring, "help") == 0)
		help();
	else if (strcmp(instring, "transpose") == 0 && *ystack == 23) {
		*ystack = 26;
		print_tos();
	} else if (strcmp(instring, "invert") == 0 && ystack[0] == 23) {
		ystack[0] = 27;
		print_tos();
	} else if (strcmp(instring, "invert") == 0 && ystack[0] == 27) {
		ystack[0] = 23;
		print_tos();
	} else if (strcmp(instring, "multiply") == 0 && ystack[0] == 26 && ystack[1] == 24) {
		ystack[0] = 25;
		ystack[1] = ystack[2];
		ystack[2] = ystack[3];
		ystack[3] = 0;
		print_tos();
	} else if (strcmp(instring, "multiply") == 0 && ystack[0] == 24 && ystack[1] == 25) {
		ystack[0] = 28;
		ystack[1] = ystack[2];
		ystack[2] = ystack[3];
		ystack[3] = 0;
		print_tos();
	} else if (strcmp(instring, "multiply") == 0 && ystack[0] == 24 && ystack[1] == 23) {
		ystack[0] = 29;
		ystack[1] = ystack[2];
		ystack[2] = ystack[3];
		ystack[3] = 0;
		print_tos();
	} else if (strcmp(instring, "multiply") == 0 && ystack[0] == 24 && ystack[1] == 26) {
		ystack[0] = 30;
		ystack[1] = ystack[2];
		ystack[2] = ystack[3];
		ystack[3] = 0;
		print_tos();
	} else if (strcmp(instring, "multiply") == 0 && ystack[0] == 30 && ystack[1] == 24) {
		ystack[0] = 28;
		ystack[1] = ystack[2];
		ystack[2] = ystack[3];
		ystack[3] = 0;
		print_tos();
	} else
		printf("?\n");
}

print()

{

	int i;

	for (i = 0; i < 4; i++)

		if (ystack[i])

			printf("%s", displayform[ystack[i]]);

}

/* print top of stack */

print_tos()

{

	printf("%s", displayform[*ystack]);

}

push(n)

int n;

{

	ystack[3] = ystack[2];

	ystack[2] = ystack[1];

	ystack[1] = ystack[0];

	ystack[0] = n;

	print_tos();

}

/* print applicable equivalence verbs */

help()

{

	int i;

	for (i = 1; i < NEQU; i++) {

		if (equ[i].x == *ystack) {

			printf("%s\n", citation[equ[i].f]);

			printf("%s", displayform[equ[i].y]);

		}

		if (equ[i].y == *ystack) {

			printf("%s\n", citation[equ[i].f]);

			printf("%s", displayform[equ[i].x]);

		}

	}

}
