// demo program that prints all decodings of a binary sequence

char result[100];

main()
{
        f("111110101", 0);
}

f(char *s, int n)
{
        if (*s == 0) {
                printf("%s\n", result);
                return;
        }

        if (strncmp(s, "1", 1) == 0) {
                strcpy(result + n, "1");
                f(s + 1, n + 1);
        }

        if (strncmp(s, "111", 3) == 0) {
                strcpy(result + n, "3");
                f(s + 3, n + 1);
        }

        if (strncmp(s, "101", 3) == 0) {
                strcpy(result + n, "5");
                f(s + 3, n + 1);
        }

        if (strncmp(s, "10", 2) == 0) {
                strcpy(result + n, "7");
                f(s + 2, n + 1);
        }

        if (strncmp(s, "10101", 5) == 0) {
                strcpy(result + n, "9");
                f(s + 5, n + 1);
        }
}
