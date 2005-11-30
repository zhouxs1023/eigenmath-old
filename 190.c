/* Scan an HTML file and extract TEX code. */

#include <stdio.h>

int count;
FILE *fin, *fout, *flisp;
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

	strcpy(t, filename);
	strcpy(t + strlen(t), ".html");
	fout = fopen(t, "w");
	if (fout == NULL) {
		printf("error: %s\n", t);
		exit(1);
	}

	strcpy(lisp_file_name, argv[1]);
	strcpy(lisp_file_name + strlen(lisp_file_name), ".lisp");

	fprintf(fout, "<html>\n");

	for (;;) {
		if (fgets(s, 1000, fin) == NULL)
			break;
		switch (*s) {
		case '\n':
			break;
		case '<':
			fprintf(fout, "%s", s);
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
	fclose(fin);
	fclose(fout);
}

emit_paragraph()
{

	fprintf(fout, "<p>\n");
	for (;;) {
		fprintf(fout, "%s", s);
		if (fgets(s, 1000, fin) == NULL)
			break;
		if (*s == '\n')
			break;
	}
}

emit_tex(flag)
int flag;
{
	FILE *fout2;
	fprintf(fout, "<p><img src=\"%s%03d.gif\">\n", filename, ++count);
	sprintf(t, "%s%03d.tex", filename, count);
	fout2 = fopen(t, "w");
	if (fout2 == NULL) {
		printf("error: %s\n", t);
		exit(1);
	}
	fprintf(fout2, "\\magnification=500\\hsize=5in\\parindent=0pt\n");
	for (;;) {
		fprintf(fout2, "%s", s);
		if (fgets(s, 1000, fin) == NULL)
			break;
		if (*s == '\n')
			break;
	}
	fprintf(fout2, "\\end\n");
	fclose(fout2);
}

emit_lisp()
{
	if (flisp == NULL) {
		flisp = fopen(lisp_file_name, "w");
		if (flisp == NULL) {
			printf("cannot open %s for writing\n", lisp_file_name);
			exit(1);
		}
	}
	fprintf(fout, "<p><pre>\n");
	for (;;) {
		fprintf(fout, "%s", s);
		fprintf(flisp, "%s", s);
		if (fgets(s, 1000, fin) == NULL)
			break;
		if (*s == '\n')
			break;
	}
	fprintf(fout, "</pre>\n");
}
