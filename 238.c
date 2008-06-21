// Scan source files and emit selftest

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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

main()
{
	int i, n, x;
	struct dirent **p;
	n = scandir(".", &p, filter, alphasort);
	for (i = 0; i < n; i++)
		//printf("<a href=\"%s\">%s</a><br>\n", p[i]->d_name, p[i]->d_name);
		g(p[i]->d_name);
}

char buf[1000], *a, *b, c;
FILE *f;

g(char *s)
{
	if (strcmp(s, "MainOSX.cpp") == 0 || strcmp(s, "MainXP.cpp") == 0)
		return;
	if (strcmp(s, "display.cpp") == 0)
		return;
	if (strcmp(s, "cmdisplay.cpp") == 0)
		return;
	if (strcmp(s, "scan.cpp") == 0)
		return;
	f = fopen(s, "r");
	if (f == NULL) {
		printf("cannot open %s\n", s);
		exit(1);
	}
	printf("clear\n");
	printf("trace=1\n");
	while (fgets(buf, sizeof buf, f)) {
		if (strstr(buf, "static char *s[] = {"))
			break;
	}
	while (fgets(buf, sizeof buf, f)) {
		if (*buf == '/')
			continue;
		a = strchr(buf, '"');
		if (a == NULL)
			continue;
		c = a[1];

		emit_string();
		printf("\n");
		if (c == '#')
			continue;

		fgets(buf, sizeof buf, f);
		a = strchr(buf, '"');

		// empty string?

		if (a[1] == '"')
			continue;

		printf("check(last = ");
		emit_string();
		printf(")\n");

		printf("-- %s\n", s);
	}
	fclose(f);
}

emit_string()
{
	b = strchr(a + 1, '"');
	*b = 0;
	printf("%s", a + 1);
	while (b[1] != ',') {
		fgets(buf, sizeof buf, f);
		a = strchr(buf, '"');
		b = strchr(a + 1, '"');
		*b = 0;
		printf("%s", a + 1);
	}
}

