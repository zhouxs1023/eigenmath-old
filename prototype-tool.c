/* Progress in computer science does not always go forward.

Using Pascal, people had to write their programs upside down.

Then wonderful C came along. K & R created a perfect programming language.

And with C, people no longer had to write their programs upside down.

They could put main at the top.

Then a committee came along and broke C with their prototype rule.

After that, people had to start writing their programs upside down again.

Technically, prototypes should not be necessary.

In the mid-1980's Borland had a beautiful C compiler that did type checking
without prototypes.

This tool generates a prototype file so I can avoid prototype declarations
and still keep the code rightside up.

*/

#include <stdio.h>
#include <dirent.h>

int
filter(const struct dirent *p)
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

char str1[1000], str2[1000], str3[1000];

g(char *s)
{
	char *a, *b, *c, *t;
	FILE *f;
	if (strcmp(s, "MainOSX.cpp") == 0 || strcmp(s, "MainXP.cpp") == 0)
		return;
	printf("// %s\n", s);
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
			a[strlen(a) - 1] = 0; // erase newline
			b[strlen(b) - 1] = 0;
			printf("%s %s;\n", a, b);
		}
		t = a;
		a = b;
		b = c;
		c = fgets(t, 1000, f);
	}
	fclose(f);
}
