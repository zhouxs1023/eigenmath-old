/* Print a directory of C source files. The directory includes
the beginning lines of the file up to the first blank line.
Comment delimiters are removed.
This program was originally 39.C for DOS.
This version was modified to run under Linux. */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int flag[10000];
char str[256], filename[100];

main()
{
	int i, n;

/*	printf("\\magnification=500\n");
	printf("\\tolerance=10000\n");
	printf("\\hsize=4.25in\n");
	printf("\\vsize=2.85in\n");
	printf("\\raggedbottom\n");
	printf("\\def\\makefootline{\\line{\\hss\\sevenrm\\folio}}\n");
	printf("\\hyphenpenalty=1000\n");
	printf("\\scriptscriptfont0=\\scriptfont0\n");
	printf("\\scriptscriptfont1=\\scriptfont1\n");
	printf("\\scriptscriptfont2=\\scriptfont2\n");
*/
	// printf("\\hsize=6in\n");

	for (i = 0; i < 1000; i++)
		process(i);

	printf("\\end\n");
}

process(n)
int n;
{
	FILE *f;

	sprintf(filename, "%d.c", n);

	f = fopen(filename, "r");

	if (f == NULL)
		return;

	printf("\n\\beginsection %d.\n\n", n);

	while (fgets(str, 256, f) != NULL && !yyblank(str)) {
		remove_comment_delimiters();
		printf("%s", str);
	}

	fclose(f);
}

yyblank(s)
char *s;
{
	while (*s)
		if (!isspace(*s++))
			return 0;
	return 1;
}

remove_comment_delimiters()
{
	int i, len;

	len = strlen(str);

	for (i = 0; i < len - 1; i++)
		if (str[i] == '/' && str[i + 1] == '*' || str[i] == '*' && str[i + 1] == '/' || str[i] == '/' && str[i + 1] == '/') {
			str[i] = ' ';
			str[i + 1] = ' ';
		}
}
