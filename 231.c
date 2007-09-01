//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

char buf[1000];

main()
{
	int book, chapter, verse;
	FILE *f;
	printf("\\documentclass{article}\n");
	printf("\\title{The Holy Bible}\n");
	printf("\\date{}\n");
	printf("\\begin{document}\n");
	printf("\\parindent=0pt\n");
	printf("\\maketitle\n");
	f = fopen("bible.txt", "r");
	while (fgets(buf, 1000, f)) {
		if (isdigit(*buf)) {
			sscanf(buf, "%d:%d", &chapter, &verse);
			if (verse == 1)
				printf("\n\\subsection*{Chapter %d}\n", chapter);
			printf("\\P%d~%s", verse, buf + 8);
		}
		if (*buf == ' ')
			printf("%s", buf + 8);
		if (*buf == 'B') {
			sscanf(buf, "Book %d", &book);
			buf[strlen(buf) - 1] = 0;
			printf("\n\\section*{Book %d %s}\n", book, buf + 8);
		}
	}
	fclose(f);
	printf("\\end{document}\n");
}

