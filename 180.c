/* Sum shell probabilities. */

#include <stdio.h>

main()

{

	sum("sh10");

	sum("sh20");

	sum("sh30");

}

sum(s)

char *s;

{

	int n;

	FILE *f;

	float p, t = 0.0;

	f = fopen(s, "r");

	if (f == NULL) {

		printf("cannot open %s\n", s);

		exit(1);

	}

	for (n = 0; n < 1000; n++) {

		fscanf(f, "%g", &p);

		t += p;

	}

	printf("sum of %s = %g\n", s, t);

	fclose(f);

}
