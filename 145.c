/* Generate 8N mod 127 table. */

main()

{

	int i;

	for (i = 0; i < 33; i++)

		printf("\tdb\t%d\n", (8 * i) % 127);

}
