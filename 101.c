/* Scan a Pascal source program and emit tokens. */

#include <stdio.h>

#include <ctype.h>

#define YYAND 128
#define YYARRAY 129
#define YYBEGIN 130
#define YYCASE 131
#define YYCONST 132
#define YYDIV 133
#define YYDO 134
#define YYDOWNTO 135
#define YYELSE 136
#define YYEND 137
#define YYFILE 138
#define YYFOR 139
#define YYFUNCTION 140
#define YYGOTO 141
#define YYIF 142
#define YYIN 143
#define YYLABEL 144
#define YYMOD 145
#define YYNIL 146
#define YYNOT 147
#define YYOF 148
#define YYOR 149
#define YYPACKED 150
#define YYPROCEDURE 151
#define YYPROGRAM 152
#define YYRECORD 153
#define YYREPEAT 154
#define YYSET 155
#define YYTHEN 156
#define YYTO 157
#define YYTYPE 158
#define YYUNTIL 159
#define YYVAR 160
#define YYWHILE 161
#define YYWITH 162
#define YYIDENTIFIER 163
#define YYDIGITSTRING 164
#define YYSTRING 165
#define YYCOLONEQUAL 166
#define YYPERIODPERIOD 167
#define YYNOTEQUAL 168
#define YYGREATEREQUAL 169
#define YYLESSEQUAL 170
#define YYREAD 171
#define YYREADLN 172
#define YYWRITE 173
#define YYWRITELN 174

int c;

char buf[2000];

main()

{

	c = fgetc(stdin);

	while (c != EOF)

		gettoken();

}

gettoken()

{

	int n, token;

	/* skip spaces */

	while (isspace(c))

		c = fgetc(stdin);

	/* comment */

	if (c == '{') {

		while (c != '}')

			c = fgetc(stdin);

		c = fgetc(stdin);

	/* identifier or key word */

	} else if (isalpha(c)) {

		n = 0;

		while (isalpha(c)) {

			buf[n++] = c;

			c = fgetc(stdin);

		}

		buf[n] = 0;

		if (strcmp(buf, "and") == 0)

			token = YYAND;

		else if (strcmp(buf, "array") == 0)

			token = YYARRAY;

		else if (strcmp(buf, "begin") == 0)

			token = YYBEGIN;

		else if (strcmp(buf, "case") == 0)

			token = YYCASE;

		else if (strcmp(buf, "const") == 0)

			token = YYCONST;

		else if (strcmp(buf, "div") == 0)

			token = YYDIV;

		else if (strcmp(buf, "do") == 0)

			token = YYDO;

		else if (strcmp(buf, "downto") == 0)

			token = YYDOWNTO;

		else if (strcmp(buf, "else") == 0)

			token = YYELSE;

		else if (strcmp(buf, "end") == 0)

			token = YYEND;

		else if (strcmp(buf, "file") == 0)

			token = YYFILE;

		else if (strcmp(buf, "for") == 0)

			token = YYFOR;

		else if (strcmp(buf, "function") == 0)

			token = YYFUNCTION;

		else if (strcmp(buf, "goto") == 0)

			token = YYGOTO;

		else if (strcmp(buf, "if") == 0)

			token = YYIF;

		else if (strcmp(buf, "in") == 0)

			token = YYIN;

		else if (strcmp(buf, "label") == 0)

			token = YYLABEL;

		else if (strcmp(buf, "mod") == 0)

			token = YYMOD;

		else if (strcmp(buf, "nil") == 0)

			token = YYNIL;

		else if (strcmp(buf, "not") == 0)

			token = YYNOT;

		else if (strcmp(buf, "of") == 0)

			token = YYOF;

		else if (strcmp(buf, "or") == 0)

			token = YYOR;

		else if (strcmp(buf, "packed") == 0)

			token = YYPACKED;

		else if (strcmp(buf, "procedure") == 0)

			token = YYPROCEDURE;

		else if (strcmp(buf, "program") == 0)

			token = YYPROGRAM;

		else if (strcmp(buf, "record") == 0)

			token = YYRECORD;

		else if (strcmp(buf, "repeat") == 0)

			token = YYREPEAT;

		else if (strcmp(buf, "set") == 0)

			token = YYSET;

		else if (strcmp(buf, "then") == 0)

			token = YYTHEN;

		else if (strcmp(buf, "to") == 0)

			token = YYTO;

		else if (strcmp(buf, "type") == 0)

			token = YYTYPE;

		else if (strcmp(buf, "until") == 0)

			token = YYUNTIL;

		else if (strcmp(buf, "var") == 0)

			token = YYVAR;

		else if (strcmp(buf, "while") == 0)

			token = YYWHILE;

		else if (strcmp(buf, "with") == 0)

			token = YYWITH;

		else if (strcmp(buf, "read") == 0)

			token = YYREAD;

		else if (strcmp(buf, "readln") == 0)

			token = YYREADLN;

		else if (strcmp(buf, "write") == 0)

			token = YYWRITE;

		else if (strcmp(buf, "writeln") == 0)

			token = YYWRITELN;

		else

			token = YYIDENTIFIER;

		printf("%4d %s\n", token, buf);

	} else if (isdigit(c)) {

		printf("%4d ", YYDIGITSTRING);

		while (isdigit(c)) {

			printf("%c", c);

			c = fgetc(stdin);

		}

		if (c == '.') {

			c = fgetc(stdin);

			if (c == '.') {

				c = fgetc(stdin);

				printf("\n");

				printf("%4d ..\n", YYPERIODPERIOD);

			} else {

				printf(".");

				while (isdigit(c)) {

					printf("%c", c);

					c = fgetc(stdin);

				}

				printf("\n");

			}

		} else

			printf("\n");

	} else if (c == '\'') {

		printf("%4d '", YYSTRING);

		c = fgetc(stdin);

		for (;;) {

			if (c == '\'') {

				c = fgetc(stdin);

				if (c == '\'')

					;

				else

					break;

			}

			printf("%c", c);

			c = fgetc(stdin);

		}

		printf("'\n", c);

	} else if (c == '.') {

		c = fgetc(stdin);

		if (c == '.') {

			printf("%4d ..\n", YYPERIODPERIOD);

			c = fgetc(stdin);

		} else

			/* period */

			printf("%4d .\n", '.');

	} else if (c == ':') {

		c = fgetc(stdin);

		if (c == '=') {

			printf("%4d :=\n", YYCOLONEQUAL);

			c = fgetc(stdin);

		} else

			printf("%4d :\n", ':');

	} else if (c == '<') {

		c = fgetc(stdin);

		if (c == '>') {

			printf("%4d <>\n", YYNOTEQUAL);

			c = fgetc(stdin);

		} else if (c == '=') {

			printf("%4d <=\n", YYLESSEQUAL);

			c = fgetc(stdin);

		} else

			printf("%4d <\n", '<');

	} else if (c == '>') {

		c = fgetc(stdin);

		if (c == '=') {

			printf("%4d >=\n", YYGREATEREQUAL);

			c = fgetc(stdin);

		} else

			printf("%4d >\n", '>');

	} else if (c != EOF) {

		printf("%4d %c\n", c, c);

		c = fgetc(stdin);

	}

}
