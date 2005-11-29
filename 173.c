/* Generate a file for initializing the DSP user to network queue. */

main()

{

	f("This is a short packet.\n");

	f("This is a long packet that tests the data blocking capabilities of the transmitter and receiver.\n");

	f("Short packet with odd length\n");

	f("Pad to 100 words..............\n");

}

f(s)

char *s;

{

	int i, n;

	while (strlen(s) > 30) {

		printf("001e\n");

		for (i = 0; i < 15; i++) {

			printf("%02x%02x\n", s[1], s[0]);

			s += 2;

		}

	}

	n = strlen(s);

	printf("%04x\n", n | 0x8000);

	n = (n + 1) / 2;

	for (i = 0; i < n; i++) {

		printf("%02x%02x\n", s[1], s[0]);

		s += 2;

	}

}
