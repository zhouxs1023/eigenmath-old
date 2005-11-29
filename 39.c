/* Print a directory of C source files. The directory includes
the beginning lines of the file up to the first blank line.
Comment delimiters are removed. */

#include <stdio.h>
#include <ctype.h>
#include <dir.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

int flag[10000];
char str[256], filename[100];

main()
{
	int i, n;
	struct ffblk b;

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
	printf("\\hsize=6in\n");

	n = findfirst("*.c", &b, 0);

	while (n == 0) {
		i = 0;
		sscanf(b.ff_name, "%d.c", &i);
		if (i > 0 && i < 10000)
			flag[i] = 1;
		n = findnext(&b);
	}

	for (i = 0; i < 10000; i++)
		if (flag[i])
			process(i);

	printf("\\end\n");

}

process(n)
int n;
{
	FILE *f;

	printf("\n\\beginsection %d.\n\n", n);

	sprintf(filename, "%d.c", n);

	f = fopen(filename, "r");

	while (fgets(str, 256, f) != NULL && !isblank(str)) {
		remove_comment_delimiters();
		printf("%s", str);
	}

	fclose(f);
}

isblank(s)
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
		if (str[i] == '/' && str[i + 1] == '*' || str[i] == '*' && str[i + 1] == '/') {
			str[i] = ' ';
			str[i + 1] = ' ';
		}
}
