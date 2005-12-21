// DVT tool for 6415 DSP board.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <errno.h>
#include <asm/ioctl.h>

void eval(char *);
void help(void);
void copy_memory(unsigned long, unsigned long, unsigned long);
void fill_memory(unsigned long, unsigned long, int, int, int);
void inspect_memory(unsigned long, int, int);
void print_memory(unsigned long, unsigned long);
void read_memory(unsigned long, int);
void search_memory(unsigned long, unsigned long, unsigned int, unsigned int, int, int);
void test_memory(unsigned long, unsigned long, int);
void verify_memory(unsigned long, unsigned long, unsigned long);
void write_memory(unsigned long, unsigned int, unsigned int, int);
void test_address_lines(unsigned long, int);
void edit_pci(int, int, int, int);
void scan_pci(void);

#define BUF ((unsigned int *) buf)

unsigned char buf[8192];
unsigned long a1, a2, a3, a4, w2, w3, u;

// dsp stuff

#define IO_MAGIC 0xD5

#define READ_REG       _IO(IO_MAGIC, 12)
#define WRITE_REG      _IO(IO_MAGIC, 13)
#define SET_READ_MODE  _IO(IO_MAGIC, 14)
#define SET_WRITE_MODE _IO(IO_MAGIC, 15)

#define REG_HPIC          0
#define REG_HPIA          1
#define REG_HPIDI         2
#define REG_HPID          3
#define REG_CTRLSTAT      4
#define REG_MODULE_SEL    5
#define REG_CHIP_SEL      6
#define REG_EXT1_SEL      7
#define REG_EXT2_SEL      8
#define REG_RESET         9
#define REG_BM_HOST_ADDR 10
#define REG_BM_XFER_CTRL 11
#define REG_DMA_DSP      12
#define REG_BM_FLAGS     13
#define REG_INTR         14

#define MODE_NOWAIT 4

int dspnum, fd;

// read a board register

int
dsp_ioctl_get(int reg, int *data)
{
	int buf[2], t;
	buf[0] = reg;
	buf[1] = 0;
	t = ioctl(fd, READ_REG, buf);
	*data = buf[1];
	return t;
}

// write a board register

int
dsp_ioctl_set(int reg, int data)
{
	int buf[2], t;
	buf[0] = reg;
	buf[1] = data;
	t = ioctl(fd, WRITE_REG, buf);
	return t;
}

// read len bytes from DSP memory

int
read_dsp(unsigned char *buf, int len, unsigned long addr)
{
	int i, n, t;
	struct pollfd pollfd;

	// ensure DMA channel is available

	pollfd.fd = fd;
	pollfd.events = POLLIN;
	t = poll(&pollfd, 1, -1);
	if (t < 1)
		return -1;

	// number of blocks

	n = len / 4096;

	for (i = 0; i < n; i++) {

		// start the DMA transfer

		t = pread(fd, buf + 4096 * i, 4096, addr + 4096 * i);
		if (t)
			return -1; // t = 0 expected

		// wait for the DMA to complete

		t = poll(&pollfd, 1, -1);
		if (t < 1)
			return -1;

		// copy data from the DMA buffer

		t = pread(fd, buf + 4096 * i, 4096, addr + 4096 * i);
		if (t < 4096)
			return -1;
	}

	// bytes remaining

	n = len - 4096 * n;

	if (n == 0)
		return 0;

	// start the DMA transfer

	t = pread(fd, buf + len - n, n, addr + len - n);
	if (t)
		return -1; // t = 0 expected

	// wait for the DMA to complete

	t = poll(&pollfd, 1, -1);
	if (t < 1)
		return -1;

	// copy data from the DMA buffer

	t = pread(fd, buf + len - n, n, addr + len - n);
	if (t < n)
		return -1;

	return 0;
}

// write len bytes to DSP memory

int
write_dsp(unsigned char *buf, int len, unsigned long addr)
{
	int i, n, t;
	struct pollfd pollfd;

	// ensure DMA channel is available

	pollfd.fd = fd;
	pollfd.events = POLLOUT;
	t = poll(&pollfd, 1, 1000);
	if (t < 1)
		return -1;

	// number of blocks

	n = len / 4096;

	for (i = 0; i < n; i++) {

		// start the DMA transfer

		t = pwrite(fd, buf + 4096 * i, 4096, addr + 4096 * i);
		if (t < 4096)
			return -1;

		// wait for the DMA to complete

		t = poll(&pollfd, 1, 1000);
		if (t < 1)
			return -1;
	}

	// bytes remaining

	n = len - 4096 * n;

	if (n == 0)
		return 0;

	// start the DMA transfer

	t = pwrite(fd, buf + len - n, n, addr + len - n);
	if (t < n)
		return -1;

	// wait for the DMA to complete

	t = poll(&pollfd, 1, 1000);
	if (t < 1)
		return -1;

	return 0;
}

// bounce DSP reset lines

int
dsp_reset(void)
{
	int t;
	t = dsp_ioctl_set(REG_RESET, 0x00);
	if (t)
		return -1;
	usleep(100000);
	t = dsp_ioctl_set(REG_RESET, 0xff);
	if (t)
		return -1;
	usleep(100000);
	return 0;
}

void
dsp_select(int n)
{
	dspnum = n;
	dsp_ioctl_set(REG_CHIP_SEL, 1 << n);
}

void
dsp_open(void)
{
	int tmp;
	fd = open("/dev/sigc641x", O_RDWR);
	if (fd < 0) {
		perror(NULL);
		exit(1);
	}
	tmp = MODE_NOWAIT;
	ioctl(fd, SET_READ_MODE, &tmp);
	ioctl(fd, SET_WRITE_MODE, &tmp);
	dsp_reset();
	dsp_select(0);
}

/*

EMIFA_GCTL       0x01800000
EMIFA_CE0        0x01800008
EMIFA_SDRAMCTL   0x01800018
EMIFA_SDRAMREF   0x0180001c
EMIFA_SDRAMEXT   0x01800020

Please set the value for those five registers as below.

    *(int *)EMIFA_GCTL     = 0x0001200C;
    *(int *)EMIFA_CE0      = 0xFFFFFF30;
    *(int *)EMIFA_SDRAMCTL = 0x53116000;
    *(int *)EMIFA_SDRAMREF = 0x00000735;
    *(int *)EMIFA_SDRAMEXT = 0x00054549;
*/

void
init_sdram(void)
{
	unsigned int x;
	x = 0x0001200c; write_dsp((void *) &x, 4, 0x01800000);
	x = 0xffffff30; write_dsp((void *) &x, 4, 0x01800008);
	x = 0x53116000; write_dsp((void *) &x, 4, 0x01800018);
	x = 0x00000735; write_dsp((void *) &x, 4, 0x0180001c);
	x = 0x00054549; write_dsp((void *) &x, 4, 0x01800020);
}

main(int argc, char *argv[])
{
	dsp_open();

	init_sdram();

	if (argc == 2) {
		eval(argv[1]);
		exit(0);
	}

	help();

	while (1) {
		printf("DSP %d> ", dspnum);
		fgets(buf, sizeof buf, stdin);
		eval(buf);
	}
}

int
user(void)
{
	struct pollfd x;
	x.fd = 0; // stdin
	x.events = POLLIN | POLLHUP;
	if (poll(&x, 1, 0) == 1) {
		fgets(buf, sizeof buf, stdin);
		return 1;
	} else
		return 0;
}

void
help(void)
{
	printf("           Aa[,b]             ADDRESS LINE TEST\n");
//	printf("           Ba[,b[,c]]         TEST BLOCK OF MEMORY (16 BIT)\n");
	printf("           Ca,b,c             COPY MEMORY BLOCK\n");
	printf("           E[a,b,c,d]         EXAMINE/ALTER PCI CONFIGURATION SPACE\n");
	printf("           Fa,b,c[,d]         FILL MEMORY\n");
//	printf("           Ia[,b,c]           INSPECT/ALTER MEMORY BY BYTE\n");
//	printf("           L[s]               LOAD PROGRAM\n");
	printf("           Ma[,b,c]           INSPECT/ALTER MEMORY BY LONG WORDS\n");
	printf("           Na,b,c[,d]         SEARCH MEMORY FOR NON-PATTERN\n");
	printf("           Pa[,b]             PRINT MEMORY CONTENTS\n");
	printf("           Ra[,b]             CONTINUALLY READ MEMORY\n");
	printf("           Sa,b,c[,d]         SEARCH MEMORY FOR PATTERN\n");
	printf("           Ta[,b[,c]]         TEST BLOCK OF MEMORY (32 BIT)\n");
	printf("           U[a]               INSPECT/SET U REGISTER\n");
	printf("           Va,b,c             VERIFY MEMORY AGAINST MEMORY\n");
	printf("           Wa,b[,c]           WRITE/TOGGLE BITS IN MEMORY\n");
	printf("           Za                 CHANGE DSP NUMBER\n");
	printf("           ?                  HELP\n");
}

char *
next(char *s)
{
	while (isxdigit(*s))
		s++;
	while (*s && !isxdigit(*s))
		s++;
	return s;
}

int
get_args(char *s)
{
	int n;
	char *t;

	while (*s && *s != 'U' && *s != 'u' && !isxdigit(*s))
		s++;

	if (*s == 'U' || *s == 'u') {
		a1 = u;
		s++;
		while (*s && !isxdigit(*s))
			s++;
	} else {
		n = sscanf(s, "%lx", &a1);
		if (n < 1)
			return 0;
		s = next(s);
	}

	// width of 2nd arg

	t = s;
	w2 = 0;
	while (isxdigit(*t)) {
		t++;
		w2++;
	}

	// scan 2nd arg

	n = sscanf(s, "%lx", &a2);
	if (n < 1)
		return 1;
	s = next(s);

	// width of 3rd arg

	t = s;
	w3 = 0;
	while (isxdigit(*t)) {
		t++;
		w3++;
	}

	// scan 3rd arg

	n = sscanf(s, "%lx", &a3);
	if (n < 1)
		return 2;
	s = next(s);

	// scan 4th arg

	n = sscanf(s, "%lx", &a4);
	if (n < 1)
		return 3;

	return 4;
}

void
eval(char *s)
{
	int c, n;
	while (isspace(*s))
		s++;
	if (*s == 0)
		return;
	c = toupper(*s);
	n = get_args(s + 1);
	switch (c) {
	case '?':
		help();
		break;
	case 'A':
		if (n == 1)
			test_address_lines(a1, 32);
		else if (n == 2)
			test_address_lines(a1, a2);
		else
			printf("ERROR\n");
		break;
	case 'C':
		if (n == 3)
			copy_memory(a1, a2, a3);
		else
			printf("ERROR\n");
		break;
	case 'E':
		if (n == 0)
			scan_pci();
		else if (n == 3)
			edit_pci(a1, a2, a3, 0);
		else if (n == 4)
			edit_pci(a1, a2, a3, a4);
		break;
	case 'F':
		if (n == 3)
			fill_memory(a1, a2, a3, 0, w3);
		else if (n == 4)
			fill_memory(a1, a2, a3, a4, w3);
		else
			printf("ERROR\n");
		break;
	case 'I':
		if (n == 1)
			inspect_memory(a1, 4, 4);
		else if (n == 2)
			inspect_memory(a1, a2, a2);
		else if (n == 3)
			inspect_memory(a1, a2, a3);
		else
			printf("ERROR\n");
		break;
	case 'M':
		if (n == 1)
			inspect_memory(a1, 4, 4);
		else if (n == 2)
			inspect_memory(a1, a2, a2);
		else if (n == 3)
			inspect_memory(a1, a2, a3);
		else
			printf("ERROR\n");
		break;
	case 'N':
		if (n == 3)
			search_memory(a1, a2, a3, 0xffffffff, w3, 0);
		else if (n == 4)
			search_memory(a1, a2, a3, a4, w3, 0);
		else
			printf("ERROR\n");
		break;
	case 'P':
		if (n == 1 || (n == 0 && a1))
			print_memory(a1, a1 + 256);
		else if (n == 2)
			print_memory(a1, a2);
		else
			printf("ERROR\n");
		break;
	case 'R':
		if (n == 1)
			read_memory(a1, 1);
		else if (n == 2)
			read_memory(a1, a2);
		else
			printf("ERROR\n");
		break;
	case 'S':
		if (n == 3)
			search_memory(a1, a2, a3, 0xffffffff, w3, 1);
		else if (n == 4)
			search_memory(a1, a2, a3, a4, w3, 1);
		else
			printf("ERROR\n");
		break;
	case 'T':
		if (n == 1)
			test_memory(a1, 0, 0);
		else if (n == 2)
			test_memory(a1, a2, 0);
		else if (n == 3)
			test_memory(a1, a2, a3);
		else
			printf("ERROR\n");
		break;
	case 'U':
		if (n == 0)
			printf("%lx\n", u);
		else if (n == 1)
			u = a1;
		else
			printf("ERROR\n");
		break;
	case 'V':
		if (n == 3)
			verify_memory(a1, a2, a3);
		else
			printf("ERROR\n");
		break;
	case 'W':
		if (n == 2)
			write_memory(a1, a2, 0, w2);
		else if (n == 3)
			write_memory(a1, a2, a3, w2);
		else
			printf("ERROR\n");
		break;
	case 'Z':
		if (n == 1)
			dsp_select(a1);
		else
			printf("ERROR\n");
		break;
	default:
		printf("ERROR\n");
		break;
	}
}

// test from address a to b

void
test_memory(unsigned long a, unsigned long b, int maxerr)
{
	int i, j, n, r;
	int len, k = 0, nerr = 0;
	unsigned int pat, tmp;

	// word-align

	a &= ~3;
	b &= ~3;

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	do {
		k++;

		// write walking 0

		pat = 1 << (k % 33);

		// write n full blocks

		for (i = 0; i < n; i++) {
			for (j = 0; j < 1024; j++) {
				BUF[j] = ~pat;
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
			write_dsp(buf, 4096, a + 4096 * i);
			if (user())
				goto stop;
		}

		// write the runt block if there is one

		if (r) {
			for (j = 0; j < r / 4; j++) {
				BUF[j] = ~pat;
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
			write_dsp(buf, r, a + 4096 * n);
		}

		// verify walking 0

		pat = 1 << (k % 33);

		// read and verify n full blocks

		for (i = 0; i < n; i++) {
			read_dsp(buf, 4096, a + 4096 * i);
			if (user())
				goto stop;
			for (j = 0; j < 1024; j++) {
				tmp = BUF[j];
				if (tmp != ~pat) {
					printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", a + 4096 * i + 4 * j, ~pat, tmp, k);
					if (++nerr >= maxerr)
						goto done;
				}
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
		}

		// read and verify the runt block if there is one

		if (r) {
			read_dsp(buf, r, a + 4096 * n);
			for (j = 0; j < r / 4; j++) {
				tmp = BUF[j];
				if (tmp != ~pat) {
					printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", a + 4096 * n + 4 * j, ~pat, tmp, k);
					if (++nerr >= maxerr)
						goto done;
				}
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
		}

		// write walking 1

		pat = 1 << (k % 33);

		// write n full blocks

		for (i = 0; i < n; i++) {
			for (j = 0; j < 1024; j++) {
				BUF[j] = pat;
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
			write_dsp(buf, 4096, a + 4096 * i);
			if (user())
				goto stop;
		}

		// write the runt block if there is one

		if (r) {
			for (j = 0; j < r / 4; j++) {
				BUF[j] = ~pat;
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
			write_dsp(buf, r, a + 4096 * n);
		}

		// verify walking 1

		pat = 1 << (k % 33);

		// read and verify n full blocks

		for (i = 0; i < n; i++) {
			read_dsp(buf, 4096, a + 4096 * i);
			if (user())
				goto stop;
			for (j = 0; j < 1024; j++) {
				tmp = BUF[j];
				if (tmp != pat) {
					printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", a + 4096 * i + 4 * j, pat, tmp, k);
					if (++nerr >= maxerr)
						goto done;
				}
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
		}

		// read and verify the runt block if there is one

		if (r) {
			read_dsp(buf, r, a + 4096 * n);
			for (j = 0; j < r / 4; j++) {
				tmp = BUF[j];
				if (tmp != pat) {
					printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", a + 4096 * n + 4 * j, pat, tmp, k);
					if (++nerr >= maxerr)
						goto done;
				}
				if (pat)
					pat <<= 1;
				else
					pat = 1;
			}
		}

	} while (maxerr); // just a single pass when maxerr == 0

done:
	if (nerr == 0)
		printf("MEMORY OK\n");

	return;

stop:
	printf("TEST STOPPED ON PASS %08x\n", k);
}

// copy a through b into c

void
copy_memory(unsigned long a, unsigned long b, unsigned long c)
{
	int i, j, len, n, r;

	// word-align

	a &= ~3;
	b &= ~3;
	c &= ~3;

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	// copy n full blocks

	for (i = 0; i < n; i++) {
		read_dsp(buf, 4096, a + 4096 * i);
		write_dsp(buf, 4096, c + 4096 * i);
	}

	// copy the runt block if there is one

	if (r) {
		read_dsp(buf, r, a + 4096 * n);
		write_dsp(buf, r, c + 4096 * n);
	}
}

// verify from a to b with c

void
verify_memory(unsigned long a, unsigned long b, unsigned long c)
{
	int i, j, len, n, r;
	unsigned int x, y;
	unsigned long d;

	// word-align

	a &= ~3;
	b &= ~3;
	c &= ~3;

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	// verify n full blocks

	for (i = 0; i < n; i++) {
		read_dsp(buf, 4096, a + 4096 * i);
		read_dsp(buf + 4096, 4096, c + 4096 * i);
		if (user())
			return;
		for (j = 0; j < 1024; j++) {
			x = BUF[j];
			y = BUF[j + 1024];
			if (x != y)
				printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x\n", c + 4096 * i + 4 * j, x, y);
		}
	}

	// verify the runt block if there is one

	if (r) {
		read_dsp(buf, r, a + 4096 * n);
		read_dsp(buf + 4096, r, c + 4096 * n);
		for (j = 0; j < r / 4; j++) {
			x = BUF[j];
			y = BUF[j + 1024];
			if (x != y)
				printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x\n", c + 4096 * n + 4 * j, x, y);
		}
	}
}

// print from a to b

void
print_memory(unsigned long a, unsigned long b)
{
	int i, j, k, len, n, r;
	char *s;

	// word-align

	a &= ~3;
	b &= ~3;

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	// print n full blocks

	for (i = 0; i < n; i++) {
		read_dsp(buf, 4096, a + 4096 * i);
		if (user())
			return;
		for (j = 0; j < 1024; j += 4) {
			printf("%08x: %08x %08x %08x %08x ", a + 4096 * i + 4 * j, BUF[j], BUF[j + 1], BUF[j + 2], BUF[j + 3]);
			s = (char *) buf;
			for (k = 0; k < 16; k++) {
				if (isprint(s[k]))
					printf("%c", s[k]);
				else
					printf("_");
			}
			printf("\n");
		}
	}

	// print the runt block if there is one

	if (r) {
		read_dsp(buf, r, a + 4096 * n);
		for (j = 0; j < r / 4; j += 4) {
			printf("%08x: %08x %08x %08x %08x ", a + 4096 * n + 4 * j, BUF[j], BUF[j + 1], BUF[j + 2], BUF[j + 3]);
			s = (char *) buf;
			for (k = 0; k < 16; k++) {
				if (isprint(s[k]))
					printf("%c", s[k]);
				else
					printf("_");
			}
			printf("\n");
		}
	}
}

// fill from a to b

void
fill_memory(unsigned long a, unsigned long b, int val, int incr, int nchar)
{
	int i, j, len, n, r;

	if (nchar < 5) {
		printf("sorry, the hardware only supports 32-bit memory access\n");
		return;
	}

	// word-align

	a &= ~3;
	b &= ~3;

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	// fill n full blocks

	for (i = 0; i < n; i++) {
		for (j = 0; j < 1024; j++) {
			BUF[j] = val;
			val += incr;
		}
		write_dsp(buf, 4096, a + 4096 * i);
	}

	// fill the runt block if there is one

	if (r) {
		for (j = 0; j < 1024; j++) {
			BUF[j] = val;
			val += incr;
		}
		write_dsp(buf, r, a + 4096 * n);
	}
}

void
inspect_memory(unsigned long a, int width, int incr)
{
	int n;
	unsigned int val;

	if (width != 4 || incr % 4) {
		printf("sorry, the hardware only supports 32-bit memory access\n");
		return;
	}

	a &= ~3;

	while (1) {

		read_dsp(buf, 4, a);

		printf("%08x: %08x ", a, *BUF);

		fgets(buf, sizeof buf, stdin);

		if (*buf == ',') {
			a -= incr;
			continue;
		}

		if (*buf == '\n') {
			a += incr;
			continue;
		}

		n = sscanf(buf, "%x", &val);

		if (n < 1)
			break;

		*BUF = val;

		write_dsp(buf, 4, a);
	}
}

// search from a to b

void
search_memory(unsigned long a, unsigned long b, unsigned int pattern, unsigned int mask, int nchar, int mode)
{
	int i, j, len, match, n, r;
	unsigned int val;

	if (nchar < 5) {
		printf("sorry, the hardware only supports 32-bit memory access\n");
		return;
	}

	len = b - a;

	// n is the number of full blocks

	n = len / 4096;

	// r is the length of the runt block

	r = len % 4096;

	// search n full blocks

	for (i = 0; i < n; i++) {
		read_dsp(buf, 4096, a + 4096 * i);
		for (j = 0; j < 1024; j++) {
			val = BUF[j];
			if (((val ^ pattern) & mask) == 0)
				match = 1;
			else
				match = 0;
			if (match == mode)
				printf("%08x: %08x\n", a + 4096 * i + 4 * j, val);
		}
	}

	// search the runt block if there is one

	if (r) {
		read_dsp(buf, r, a + 4096 * n);
		for (j = 0; j < 1024; j++) {
			val = BUF[j];
			if (((val ^ pattern) & mask) == 0)
				match = 1;
			else
				match = 0;
			if (match == mode)
				printf("%08x: %08x\n", a + 4096 * i + 4 * j, val);
		}
	}
}

// write the same memory location over and over

void
write_memory(unsigned long a, unsigned int val, unsigned int inv, int nchar)
{
	if (nchar < 5) {
		printf("sorry, the hardware only supports 32-bit memory access\n");
		return;
	}
	a &= ~3;
	while (1) {
		*BUF = val;
		write_dsp(buf, 4, a);
		val ^= inv;
		if (user())
			break;
	}
}

// read the same memory location over and over, print if it changes

void
read_memory(unsigned long a, int width)
{
	unsigned int val, tmp;
	if (width != 4) {
		printf("sorry, the hardware only supports 32-bit memory access\n");
		return;
	}
	a &= ~3;
	read_dsp(buf, 4, a);
	val = *BUF;
	printf("%08lx: %08x\n", a, val);
	while (1) {
		read_dsp(buf, 4, a);
		tmp = *BUF;
		if (tmp != val) {
			val = tmp;
			printf("%08lx: %08x\n", a, val);
		}
		if (user())
			break;
	}
}

void
test_address_lines(unsigned long a, int n)
{
	int i;
	unsigned int addr_chk, addr_err, data_err;
	unsigned int mask, pat, tmp;

	// test data lines

	data_err = 0;
	mask = 1;
	for (i = 0; i < 32; i++) {
		write_dsp((unsigned char *) &mask, 4, a);
		read_dsp((unsigned char *) &tmp, 4, a);
		if (tmp != mask) {
			printf("EXPECTED %08x RECEIVED %08x\n", mask, tmp);
			data_err |= (tmp ^ mask); // accumulate bad bits
		}
		mask <<= 1;
	}

	// see if there are any aliased address lines

	addr_err = 0;
	addr_chk = 0;
	pat = ~data_err;	// use good data bits for this test
	if (pat) {
		mask = 4; // start with a2
		for (i = 2; i < n; i++) {
			write_dsp((unsigned char *) &pat, 4, a); // write to base addr
			tmp = 0;
			write_dsp((unsigned char *) &tmp, 4, a ^ mask); // write to alias addr
			read_dsp((unsigned char *) &tmp, 4, a); // read from base addr
			if (tmp != pat)
				addr_err |= mask; // accumulate errors
			addr_chk |= mask;
			mask <<= 1;
			if (mask == 0 || mask >= n)
				break;
		}
	}

	// print results

	printf("    3          2          1                 3          2          1           \n");
	printf("   10987654 32109876 54321098 76543210     10987654 32109876 54321098 76543210\n");

	printf("D)");

	mask = 0x80000000;
	for (i = 0; i < 32; i++) {
		if (i % 8 == 0)
			printf(" ");
		if (data_err & mask)
			printf("B");
		else
			printf("g");
		mask >>= 1;
	}

	printf("  A)");

	mask = 0x80000000;
	for (i = 0; i < 32; i++) {
		if (i % 8 == 0)
			printf(" ");
		if (addr_chk & mask) {
			if (addr_err & mask)
				printf("B");
			else
				printf("g");
		} else
			printf("-");
		mask >>= 1;
	}

	printf("\n");
}

int bus, device, function, irq;
unsigned long base_addr[6], size[6];
unsigned long rom_base_addr, rom_size;

int
find_device(int vendor_id, int device_id)
{
	int i, n;
	unsigned int a, b;
	FILE *f;
	f = fopen("/proc/bus/pci/devices", "r");
	if (f == NULL) {
		printf("cannot open /proc/bus/pci/devices\n");
		return -1;
	}
	while (fgets(buf, sizeof buf, f)) {
		n = sscanf(buf, "%x %x %x %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx",
			&a,
			&b,
			&irq,
			&base_addr[0],
			&base_addr[1],
			&base_addr[2],
			&base_addr[3],
			&base_addr[4],
			&base_addr[5],
			&rom_base_addr,
			&size[0],
			&size[1],
			&size[2],
			&size[3],
			&size[4],
			&size[5],
			&rom_size);
		if (n != 17) {
			printf("error parsing device file\n");
			fclose(f);
			return -1;
		}
		bus = a >> 8;
		device = a >> 3 & 0x1f;
		function = a & 7;
		//vendor_id = b >> 16;
		//device_id = b & 0xffff;
		if (b == (vendor_id << 16) + device_id)
			return 0;
	}
	return -1;
}

void
edit_pci(int reg, int func, int slot, int bus)
{
	int fd, n;
	unsigned int val;
	sprintf(buf, "/proc/bus/pci/%02x/%02x.%d", bus, slot, func);
	fd = open(buf, O_RDWR);
	if (fd == -1) {
		printf("cannot open %s\n", buf);
		return;
	}
	while (1) {
		if (pread(fd, &val, 4, reg) != 4) {
			printf("pread() error\n");
			break;
		}
		printf("%02x: %08x ", reg, val);
		fgets(buf, sizeof buf, stdin);
		if (*buf == ',') {
			if (reg >= 4)
				reg -= 4;
			continue;
		}
		if (*buf == '\n') {
			if (reg <= 252)
				reg += 4;
			continue;
		}
		n = sscanf(buf, "%x", &val);
		if (n < 1)
			break;
		if (pwrite(fd, &val, 4, reg) != 4) {
			printf("pwrite() error\n");
			break;
		}
		// fsync(fd) --special files don't support fsync()
	}
	close(fd);
}

void
scan_pci(void)
{
	int i, n;
	int bus, device, function, vendor_id, device_id;
	unsigned int pci_addr, id, irq;

	unsigned long addr[6], size[6];
	unsigned long rom_addr, rom_size;

	FILE *f;

	f = fopen("/proc/bus/pci/devices", "r");

	if (f == NULL) {
		printf("cannot open /proc/bus/pci/devices\n");
		return;
	}

	while (fgets(buf, sizeof buf, f)) {
		n = sscanf(buf, "%x %x %x %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx %lx",
			&pci_addr,
			&id,
			&irq,
			&addr[0],
			&addr[1],
			&addr[2],
			&addr[3],
			&addr[4],
			&addr[5],
			&rom_addr,
			&size[0],
			&size[1],
			&size[2],
			&size[3],
			&size[4],
			&size[5],
			&rom_size);
		if (n != 17) {
			printf("error parsing /proc/bus/pci/devices\n");
			return;
		}
		bus = pci_addr >> 8;
		device = pci_addr >> 3 & 0x1f;
		function = pci_addr & 7;
		vendor_id = id >> 16;
		device_id = id & 0xffff;
		printf("%02x:%02x.%d %04x %04x %08x %08x %08x %08x %08x %08x\n",
			bus,
			device,
			function,
			vendor_id,
			device_id,
			addr[0], addr[1], addr[2],
			addr[3], addr[4], addr[5]);
		printf("                  %08x %08x %08x %08x %08x %08x\n",
			size[0], size[1], size[2],
			size[3], size[4], size[5]);
	}
}
