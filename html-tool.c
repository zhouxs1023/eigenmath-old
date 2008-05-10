// Convert source code to hyperlinked html.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define N 1000
#define BUFLEN 1000
char buf[BUFLEN], token[BUFLEN];
int special;

struct {
	char *filename;
	char *symbol;
	int line;
} stab[N];

int count;

main()
{
	phase_1();
	phase_2();
	printf("%d symbols\n", count);
}

int
filter(struct dirent *p)
{
	int len;
	len = strlen(p->d_name);
	if (len > 2 && strcmp(p->d_name + len - 2, ".h") == 0)
		return 0;
	else if (len > 4 && strcmp(p->d_name + len - 4, ".cpp") == 0)
		return 1;
	else
		return 0;
}

phase_1()
{
	int i, n, x;
	struct dirent **p;
	n = scandir(".", &p, filter, alphasort);
	for (i = 0; i < n; i++)
		//printf("<a href=\"%s\">%s</a><br>\n", p[i]->d_name, p[i]->d_name);
		g(p[i]->d_name);
	scan_defs();
}

char str1[1000], str2[1000], str3[1000];

g(char *s)
{
	int line = 2;
	char *a, *b, *c, *t;
	FILE *f;
	if (strcmp(s, "MainOSX.cpp") == 0 || strcmp(s, "MainXP.cpp") == 0)
		return;
//	printf("\n// %s\n", s);
	f = fopen(s, "r");
	if (f == NULL) {
		printf("cannot open %s\n", s);
		exit(1);
	}
	a = fgets(str1, 1000, f);
	b = fgets(str2, 1000, f);
	c = fgets(str3, 1000, f);
	while (1) {
		if (c == NULL)
			break;
		if (*c == '{' && strncmp(a, "static", 5) != 0) {
			// string b has the name of the function
			strchr(b, '(')[0] = 0;
			if (count == N) {
				printf("stab full\n");
				exit(1);
			}
			stab[count].filename = strdup(s);
			stab[count].symbol = strdup(b);
			stab[count].line = line;
			count++;
			// printf("%s %d\n", b, line);
		}
		t = a;
		a = b;
		b = c;
		c = fgets(t, 1000, f);
		line++;
	}
	fclose(f);
}

scan_defs()
{
	char *s, *t;
	FILE *f;
	f = fopen("defs.h", "r");
	while (fgets(buf, BUFLEN, f)) {
		if (strncmp(buf, "#define", 7) == 0) {
			s = buf + 8;
			t = token;
			do {
				*t++ = *s++;
			} while (*s == '_' || isalnum(*s));
			*t = 0;
			stab[count].filename = "defs.h";
			stab[count].symbol = strdup(token);
			count++;
		}
	}
}

phase_2()
{
	int i, n, x;
	struct dirent **p;
	n = scandir(".", &p, filter, alphasort);
	for (i = 0; i < n; i++)
		//printf("<a href=\"%s\">%s</a><br>\n", p[i]->d_name, p[i]->d_name);
		emit_html(p[i]->d_name);
	special = 1;
	emit_html("defs.h");
}

char *preamble =
"<html>\n"
"<head>\n"
"<title>%s</title>\n"
//"<style type=\"text/css\">\n"
//"<!--\n"
//"A:link {color: black; text-decoration: none}\n"
//"A:visited {color: black; text-decoration: none}\n"
//"A:active {color: black; text-decoration: none}\n"
//"A:hover {color: blue; text-decoration: underline}\n"
//"-->\n"
//"</style>\n"
"</head>\n"
"<body>\n";

char newfilename[BUFLEN], buf[BUFLEN];
FILE *fout;
int ncol;

emit_html(char *filename, int special)
{
	FILE *f;
	if (strcmp(filename, "MainOSX.cpp") == 0
	|| strcmp(filename, "MainXP.cpp") == 0)
		return;
	f = fopen(filename, "r");
	if (f == NULL) {
		printf("cannot open %s\n", filename);
		exit(1);
	}
	strcpy(newfilename, "src/");
	strcat(newfilename, filename);
	strcat(newfilename, ".html");
	fout = fopen(newfilename, "w");
	if (fout == NULL) {
		printf("cannot open %s\n", newfilename);
		exit(1);
	}

	fprintf(fout, preamble, filename);

	while (fgets(buf, BUFLEN, f)) {
		fputs("<tt>", fout);
		process_one_line_of_source_code(buf);
		fputs("</tt><br>\n", fout);
	}

	fputs("</body>\n</html>\n", fout);

	fclose(f);
	fclose(fout);
}

process_one_line_of_source_code(char *s)
{
	char *p, *t;
	int d, i;
	static int comment_state;

	ncol = 0;

	// erase line feed

	t = s + strlen(s);
	if (t - s && t[-1] == '\n') {
		t--;
		*t = 0;
	}

	if (strncmp(s, "#define ", 8) == 0)
		d = 1;
	else
		d = 0;

	while (*s) {

		if (comment_state) {
			if (t - s > 1 && s[0] == '*' && s[1] == '/') {
				emit_char(*s++);
				emit_char(*s++);
				fputs("</i></b>", fout);
				comment_state = 0;
			} else
				emit_char(*s++);
			continue;
		}

		if (t - s > 1 && s[0] == '/' && s[1] == '*') {
			fputs("<b><i>", fout);
			emit_char(*s++);
			emit_char(*s++);
			comment_state = 1;
			continue;
		}

		if (t - s > 1 && s[0] == '/' && s[1] == '/') {
			fputs("<b><i>", fout);
			while (*s)
				emit_char(*s++);
			fputs("</i></b>", fout);
			continue;
		}

		// quoted string?

		if (*s == '"') {
			emit_char(*s++);
			while (*s && *s != '"')
				emit_char(*s++);
			if (*s)
				emit_char(*s++);
			continue;
		}

		// symbol?

		if (*s == '_' || isalpha(*s)) {

			p = token;
			while (*s == '_' || isalnum(*s))
				*p++ = *s++;
			*p = 0;
			p = token;

			// check symbol table

			for (i = 0; i < count; i++)
				if (strcmp(p, stab[i].symbol) == 0)
					break;

			// symbol not found?

			if (i == count) {
				while (*p)
					emit_char(*p++);
				continue;
			}

			// symbol defn?

			if (ncol == 0 || d && ncol == 8) {
				fprintf(fout, "<a name=\"%s\">", p);
				while (*p)
					emit_char(*p++);
				fputs("</a>", fout);
				continue;
			}

			// hyperlink

			fprintf(fout, "<a href=\"%s.html#%s\">", stab[i].filename, stab[i].symbol);
			while (*p)
				emit_char(*p++);
			fputs("</a>", fout);

			continue;
		}

		// default case

		emit_char(*s++);
	}
}

emit_char(int c)
{
	switch (c) {
	case '\t':
		do {
			fputs("&nbsp;", fout);
			ncol++;
		} while (ncol % 8);
		break;
	case ' ':
		fputs("&nbsp;", fout);
		ncol++;
		break;
	case '<':
		fputs("&lt;", fout);
		ncol++;
		break;
	case '>':
		fputs("&gt;", fout);
		ncol++;
		break;
	default:
		fputc(c, fout);
		ncol++;
		break;
	}
}
