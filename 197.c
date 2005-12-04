/* Generate LISP code for $4\times4$ determinant. */

main()
{
	int a, b, c, d, sgn;
	printf("\ttmp = 0;\n");
	for (a = 0; a < 4; a++)
	for (b = 0; b < 4; b++)
	for (c = 0; c < 4; c++)
	for (d = 0; d < 4; d++) {
		if (a == b || a == c || a == d || b == c || b == d || c == d)
			continue;
		/* sign of the permutation */
		sgn = 1;
		if (a > b) sgn *= -1;
		if (a > c) sgn *= -1;
		if (a > d) sgn *= -1;
		if (b > c) sgn *= -1;
		if (b > d) sgn *= -1;
		if (c > d) sgn *= -1;
		printf("\ttmp += %+d * m[%d][0] * m[%d][1] * m[%d][2] * m[%d][3];\n",
			sgn, a, b, c, d);
	}
}
