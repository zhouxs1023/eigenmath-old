/* Experimental code for studying rings. */

int z;

#define N 8

int add[N][N], mul[N][N];

char sadd[] =
	"abcdefgh"
	"badcfehg"
	"cdabghef"
	"dcbahgfe"
	"efghabcd"
	"fehgbadc"
	"ghefcdab"
	"hgfedcba";

char smul[] =
	"aaaaaaaa"
	"abababab"
	"acacacac"
	"adadadad"
	"aeaeaeae"
	"afafafaf"
	"agagagag"
	"ahahahah";

main()
{
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			add[i][j] = sadd[8 * i + j] - 'a';
			mul[i][j] = smul[8 * i + j] - 'a';
		}
	}
	if (p1() == 0) printf("not associative (addition)\n");
	if (p2() == 0) printf("not commutative\n");
	if (p3() == 0) printf("no additive identity\n");
	else if (p4() == 0) printf("no additive inverse\n");
	if (p5() == 0) printf("not associative (multiplication)\n");
	if (p6() == 0) printf("not distributive\n");
}

p1()
{
	int a, b, c;
	for (a = 0; a < N; a++)
		for (b = 0; b < N; b++)
			for (c = 0; c < N; c++)
				if (add[add[a][b]][c] != add[a][add[b][c]]) return 0;
	return 1;
}

p2()
{
	int a, b;
	for (a = 0; a < N; a++)
		for (b = 0; b < N; b++)
			if (add[a][b] != add[b][a]) return 0;
	return 1;
}

p3()
{
	int a;
	for (z = 0; z < N; z++) {
		for (a = 0; a < N; a++)
			if (add[a][z] != a) break;
		if (a == N) return 1;
	}
	return 0;
}

p4()
{
	int a, b;
	for (a = 0; a < N; a++) {
		for (b = 0; b < N; b++)
			if (add[a][b] == z) break;
		if (b == N) return 0;
	}
	return 1;
}

p5()
{
	int a, b, c;
	for (a = 0; a < N; a++)
		for (b = 0; b < N; b++)
			for (c = 0; c < N; c++)
				if (mul[mul[a][b]][c] != mul[a][mul[b][c]]) return 0;
	return 1;
}

p6()
{
	int a, b, c;
	for (a = 0; a < N; a++)
		for (b = 0; b < N; b++)
			for (c = 0; c < N; c++) {
				if (mul[a][add[b][c]] != add[mul[a][b]][mul[a][c]]) return 0;
				if (mul[add[b][c]][a] != add[mul[b][a]][mul[c][a]]) return 0;
			}
	return 1;
}

