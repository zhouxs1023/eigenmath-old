/* Scan a source file and emit TEX. */

#include <stdio.h>

int count;
FILE *fin;
char filename[100], lisp_file_name[100];
char s[1000];
char t[1000];

main(argc, argv)
int argc;
char *argv[];
{
	if (argc != 2) {
		printf("usage: mk filename\n");
		exit(1);
	}

	strcpy(filename, argv[1]);

	fin = fopen(filename, "r");
	if (fin == NULL) {
		printf("error: %s\n", filename);
		exit(1);
	}

	printf("\\parindent=0pt\n");

	for (;;) {
		if (fgets(s, 1000, fin) == NULL)
			break;
		switch (*s) {
		case '\n':
			break;
		case '<':
			break;
		case '$':
			emit_tex(1);
			break;
		case ';':
		case '(':
			emit_lisp();
			break;
		default:
			emit_tex(0);
			break;
		}
	}
	printf("\\end\n");
}

emit_tex(flag)
int flag;
{
	for (;;) {
		printf("%s", s);
		if (fgets(s, 1000, fin) == NULL)
			break;
		if (*s == '\n')
			break;
	}
}

emit_lisp()
{
	int c, i, n;
	for (;;) {
		printf("{\\tt ");
		n = strlen(s) - 1;
		for (i = 0; i < n; i++) {
			c = s[i];
			switch (c) {
			case ' ':
			case '_':
			case '{':
			case '}':
				printf("\\%c", c);
				break;
			default:
				printf("%c", c);
				break;
			}
		}
		printf("}\n\n");
		if (fgets(s, 1000, fin) == NULL)
			break;
		if (*s == '\n')
			break;
	}
}
