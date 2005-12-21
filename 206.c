// Demo code for accessing PCI space in a user application.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>

void eval(char *);
void help(void);
void copy_memory(unsigned long, unsigned long, unsigned long);
void fill_memory(unsigned long, unsigned long, int, int, int);
void inspect_memory(unsigned long, int, int);
void print_memory(unsigned long, unsigned long);
void read_memory(unsigned long, int);
void search_memory(unsigned long, unsigned long, unsigned int, unsigned int, int, int);
void test_memory(unsigned long, unsigned long, int);
void test_memory_16(unsigned long, unsigned long, int);
void verify_memory(unsigned long, unsigned long, unsigned long);
void write_memory(unsigned long, unsigned int, unsigned int, int);
void test_address_lines(unsigned long, int);
void find_bar(unsigned long);
void edit_pci(int, int, int, int);
void scan_pci(void);
void toe(void);
void toe_print_eeprom(unsigned long);
void toe_edit_eeprom(unsigned long);

int ndev;
char buf[1000];
unsigned long a1, a2, a3, a4, w2, w3, u;

main(int argc, char *argv[])
{
	if (argc == 2) {
		eval(argv[1]);
		exit(0);
	}

	help();

	while (1) {
		printf("> ");
		fgets(buf, sizeof buf, stdin);
		eval(buf);
	}
}

int fd;
unsigned char *mem;
unsigned long mem_addr, mem_size;

void
open_mem(unsigned long addr)
{
	mem = 0;
	find_bar(addr);
	if (mem_addr == 0)
		return;
	fd = open("/dev/mem", O_RDWR);
	if (fd == -1) {
		printf("error opening /dev/mem\n");
		return;
	}
	mem = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mem_addr);
	if (mem == 0) {
		printf("mmap failure\n");
		close(fd);
	}
}

void
close_mem(void)
{
	munmap(mem, mem_size);
	close(fd);
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
	printf("           Aa[,b].             ADDRESS LINE TEST\n");
	printf("           Ba[,b[,c]].         TEST BLOCK OF MEMORY (16 BIT)\n");
	printf("           Ca,b,c.             COPY MEMORY BLOCK\n");
	printf("           E[a,b,c,d].         EXAMINE/ALTER PCI CONFIGURATION SPACE\n");
	printf("           Fa,b,c[,d].         FILL MEMORY\n");
	//printf("           H.                  ACCESS HID0 AND MSR REGISTERS\n");
	printf("           Ia[,b,c].           INSPECT/ALTER MEMORY BY BYTE\n");
	//printf("           Ja.                 JUMP TO LOCATION\n");
	//printf("           L[s].               LOAD PROGRAM\n");
	printf("           Ma[,b,c].           INSPECT MEMORY BY LONG WORDS\n");
	printf("           Na,b,c[,d].         SEARCH MEMORY FOR NON-PATTERN\n");
	printf("           Pa[,b].             PRINT MEMORY CONTENTS\n");
	printf("           Ra[,b].             CONTINUALLY READ MEMORY\n");
	printf("           Sa,b,c[,d].         SEARCH MEMORY FOR PATTERN\n");
	printf("           Ta[,b[,c]].         TEST BLOCK OF MEMORY (32 BIT)\n");
	printf("           U[a].               INSPECT/SET U REGISTER\n");
	//printf("           V.                  INITIALIZE EXCEPTION VECTORS\n");
	printf("           Va,b,c.             VERIFY MEMORY AGAINST MEMORY\n");
	printf("           Wa,b[,c].           WRITE/TOGGLE BITS IN MEMORY\n");
//	printf("           Za,b.               SCAN FOR VENDOR/DEVICE ID\n");
	printf("           ?                   HELP\n");
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
	if (strncmp(s, "toe", 3) == 0) {
		toe();
		return;
	}
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
	case 'B':
		if (n == 1)
			test_memory_16(a1, 0, 0);
		else if (n == 2)
			test_memory_16(a1, a2, 0);
		else if (n == 3)
			test_memory_16(a1, a2, a3);
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
			inspect_memory(a1, 1, 1);
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
	default:
		printf("ERROR\n");
		break;
	}
}

#define MEM ((unsigned int volatile *) mem)

// test from a to b

void
test_memory(unsigned long a, unsigned long b, int maxerr)
{
	int i, k = 0, nerr = 0;
	unsigned int pat, tmp;

	open_mem(a);

	if (mem == 0)
		return;

	if (b == 0)
		b = a + mem_size;

	a -= mem_addr;
	b -= mem_addr;

	if (b > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	a /= 4;
	b /= 4;

	do {
		k++;

		if (user())
			goto stop;

		// write walking 0

		pat = 1 << (k % 33);
		for (i = a; i < b; i++) {
			MEM[i] = ~pat;
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// read walking 0

		pat = 1 << (k % 33);
		for (i = a; i < b; i++) {
			tmp = MEM[i];
			if (tmp != ~pat) {
				printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", mem_addr + 4 * i, ~pat, tmp, k);
				if (++nerr >= maxerr)
					goto done;
			}
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// write walking 1

		pat = 1 << (k % 33);
		for (i = a; i < b; i++) {
			MEM[i] = pat;
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// read walking 1

		pat = 1 << (k % 33);
		for (i = a; i < b; i++) {
			tmp = MEM[i];
			if (tmp != pat) {
				printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x ON PASS %08x\n", mem_addr + 4 * i, pat, tmp, k);
				if (++nerr >= maxerr)
					goto done;
			}
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

	} while (maxerr);

done:	close_mem();

	if (nerr == 0)
		printf("MEMORY OK\n");

	return;

stop:	close_mem();

	printf("TEST STOPPED ON PASS %08x\n", k);
}

#define MEM16 ((unsigned short volatile *) mem)

void
test_memory_16(unsigned long a, unsigned long b, int maxerr)
{
	int i, k = 0, nerr = 0;
	unsigned short int pat, tmp;

	open_mem(a);

	if (mem == 0)
		return;

	if (b == 0)
		b = a + mem_size;

	a -= mem_addr;
	b -= mem_addr;

	if (b > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	a /= 2;
	b /= 2;

	do {
		k++;

		if (user())
			goto stop;

		// write walking 0

		pat = 1 << (k % 17);
		for (i = a; i < b; i++) {
			MEM16[i] = ~pat;
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// read walking 0

		pat = 1 << (k % 17);
		for (i = a; i < b; i++) {
			tmp = MEM16[i];
			if (tmp != (unsigned short) ~pat) {
				printf("ADDRESS %08lx EXPECTED %04x RECEIVED %04x ON PASS %08x\n", mem_addr + 4 * i, pat ^ 0xffff, (unsigned int) tmp, k);
				if (++nerr >= maxerr)
					goto done;
			}
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// write walking 1

		pat = 1 << (k % 17);
		for (i = a; i < b; i++) {
			MEM16[i] = pat;
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

		// read walking 1

		pat = 1 << (k % 17);
		for (i = a; i < b; i++) {
			tmp = MEM16[i];
			if (tmp != pat) {
				printf("ADDRESS %08lx EXPECTED %04x RECEIVED %04x ON PASS %08x\n", mem_addr + 4 * i, (unsigned int) pat, (unsigned int) tmp, k);
				if (++nerr >= maxerr)
					goto done;
			}
			if (pat)
				pat <<= 1;
			else
				pat = 1;
			if ((i & 0xffff) == 0 && user())
				goto stop;
		}

	} while (maxerr);

done:	close_mem();

	if (nerr == 0)
		printf("MEMORY OK\n");

	return;

stop:	close_mem();

	printf("TEST STOPPED ON PASS %08x\n", k);
}
// copy a through b into c

void
copy_memory(unsigned long a, unsigned long b, unsigned long c)
{
	int i;
	unsigned long d;

	if (a > b) {
		printf("ERROR a > b\n");
		return;
	}

	d = c + b - a;

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;
	b -= mem_addr;
	c -= mem_addr;
	d -= mem_addr;

	if (b > mem_size || c > mem_size || d > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	a /= 4;
	b /= 4;
	c /= 4;
	d /= 4;

	for (i = a; i < b; i++)
		MEM[c++] = MEM[i];

	close_mem();
}

// verify from a to b with c

void
verify_memory(unsigned long a, unsigned long b, unsigned long c)
{
	int i;
	unsigned int x, y;
	unsigned long d;

	if (a > b) {
		printf("ERROR a > b\n");
		return;
	}

	d = c + b - a;

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;
	b -= mem_addr;
	c -= mem_addr;
	d -= mem_addr;

	if (b > mem_size || c > mem_size || d > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	a /= 4;
	b /= 4;
	c /= 4;
	d /= 4;

	for (i = a; i < b; i++) {
		x = MEM[i];
		y = MEM[c];
		if (x != y) {
			printf("ADDRESS %08lx EXPECTED %08x RECEIVED %08x\n", mem_addr + 4 * c, x, y);
			if (user())
				break;
		}
		c++;
		if ((c & 0xffff) == 0 && user())
			break;
	}

	close_mem();
}

// print from a to b

void
print_memory(unsigned long a, unsigned long b)
{
	int i, j;
	char *s;

	if (a > b) {
		printf("ERROR a > b\n");
		return;
	}

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;
	b -= mem_addr;

	if (b > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	a /= 4;
	b /= 4;

	for (i = a; i < b; i += 4) {

		printf("%08x: %08x %08x %08x %08x ", mem_addr + 4 * i, MEM[i], MEM[i + 1], MEM[i + 2], MEM[i + 3]);

		s = (char *) &MEM[i];

		for (j = 0; j < 16; j++)
			if (isprint(s[j]))
				printf("%c", s[j]);
			else
				printf("_");

		printf("\n");
	}

	close_mem();
}

// fill from a to b

void
fill_memory(unsigned long a, unsigned long b, int val, int incr, int nchar)
{
	int i;

	if (a > b) {
		printf("ERROR a > b\n");
		return;
	}

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;
	b -= mem_addr;

	if (b > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	switch (nchar) {
	case 1:
	case 2:
		for (i = a; i < b; i++) {
			mem[i] = val;
			val += incr;
		}
		break;
	case 3:
	case 4:
		a /= 2;
		b /= 2;
		for (i = a; i < b; i++) {
			((unsigned short *) mem)[i] = val;
			val += incr;
		}
		break;
	default:
		a /= 4;
		b /= 4;
		for (i = a; i < b; i++) {
			((unsigned int *) mem)[i] = val;
			val += incr;
		}
		break;
	}

	close_mem();
}

void
inspect_memory(unsigned long a, int width, int incr)
{
	int n;
	unsigned int val;

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;

	while (1) {

		if (a > mem_size)
			break;

		switch (width) {
		case 1:
			printf("%08x: %02x ", mem_addr + a, mem[a]);
			break;
		case 2:
			printf("%08x: %04x ", mem_addr + a, *((unsigned short *) (mem + a)));
			break;
		default:
			printf("%08x: %08x ", mem_addr + a, *((unsigned int *) (mem + a)));
		}

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

		switch (width) {
		case 1:
			mem[a] = val;
			break;
		case 2:
			*((unsigned short *) (mem + a)) = val;
			break;
		default:
			*((unsigned int *) (mem + a)) = val;
			break;
		}
	}

	close_mem();
}

// search from a to b

void
search_memory(unsigned long a, unsigned long b, unsigned int pattern, unsigned int mask, int nchar, int mode)
{
	int i, match;
	unsigned int val;

	if (a > b) {
		printf("ERROR a > b\n");
		return;
	}

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;
	b -= mem_addr;

	if (b > mem_size) {
		close_mem();
		printf("ERROR exceeds memory size of %08lx\n", mem_size);
		return;
	}

	switch (nchar) {
	case 1:
	case 2:
		for (i = a; i < b; i++) {
			val = mem[i];
			if (((val ^ pattern) & mask) == 0)
				match = 1;
			else
				match = 0;
			if (match == mode)
				printf("%08x: %02x\n", mem_addr + i, val);
		}
		break;
	case 3:
	case 4:
		a /= 2;
		b /= 2;
		for (i = a; i < b; i++) {
			val = ((unsigned short *) mem)[i];
			if (((val ^ pattern) & mask) == 0)
				match = 1;
			else
				match = 0;
			if (match == mode)
				printf("%08x: %04x\n", mem_addr + 2 * i, val);
		}
		break;
	default:
		a /= 4;
		b /= 4;
		for (i = a; i < b; i++) {
			val = ((unsigned int *) mem)[i];
			if (((val ^ pattern) & mask) == 0)
				match = 1;
			else
				match = 0;
			if (match == mode)
				printf("%08x: %08x\n", mem_addr + 4 * i, val);
		}
		break;
	}

	close_mem();
}

void
write_memory(unsigned long a, unsigned int val, unsigned int inv, int nchar)
{
	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;

	switch (nchar) {
	case 1:
	case 2:
		while (1) {
			*((unsigned char volatile *) (mem + a)) = val;
			val ^= inv;
			if (user())
				break;
		}
		break;
	case 3:
	case 4:
		while (1) {
			*((unsigned short volatile *) (mem + a)) = val;
			val ^= inv;
			if (user())
				break;
		}
		break;
	default:
		while (1) {
			*((unsigned int volatile *) (mem + a)) = val;
			val ^= inv;
			if (user())
				break;
		}
		break;
	}

	close_mem();
}

void
read_memory(unsigned long a, int width)
{
	unsigned int val, tmp;

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;

	switch (width) {
	case 1:
		val = *((unsigned char volatile *) (mem + a));
		printf("%08lx: %02x\n", mem_addr + a, val);
		while (1) {
			tmp = *((unsigned char volatile *) (mem + a));
			if (tmp != val) {
				val = tmp;
				printf("%08lx: %02x\n", mem_addr + a, val);
			}
			if (user())
				break;
		}
		break;
	case 2:
		val = *((unsigned short volatile *) (mem + a));
		printf("%08lx: %04x\n", mem_addr + a, val);
		while (1) {
			tmp = *((unsigned short volatile *) (mem + a));
			if (tmp != val) {
				val = tmp;
				printf("%08lx: %04x\n", mem_addr + a, val);
			}
			if (user())
				break;
		}
		break;
	default:
		val = *((unsigned int volatile *) (mem + a));
		printf("%08lx: %08x\n", mem_addr + a, val);
		while (1) {
			tmp = *((unsigned int volatile *) (mem + a));
			if (tmp != val) {
				val = tmp;
				printf("%08lx: %08x\n", mem_addr + a, val);
			}
			if (user())
				break;
		}
		break;
	}

	close_mem();
}

void
test_address_lines(unsigned long a, int n)
{
	int i;
	unsigned int addr_chk, addr_err, data_err;
	unsigned int mask, pat, tmp;

	open_mem(a);

	if (mem == 0)
		return;

	a -= mem_addr;

	// test data lines

	data_err = 0;
	mask = 1;
	for (i = 0; i < 32; i++) {
		*((unsigned int volatile *) (mem + a)) = mask;
		tmp = *((unsigned int volatile *) (mem + a));
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
			// write to base addr
			*((unsigned int volatile *) (mem + a)) = pat;
			// write to aliased addr
			*((unsigned int volatile *) (mem + (a ^ mask))) = 0;
			tmp = *((unsigned int volatile *) (mem + a));
			if (tmp != pat)
				addr_err |= mask; // accumulate errors
			addr_chk |= mask;
			mask <<= 1;
			if (mask == 0 || mask >= mem_size)
				break;
		}
	}

	close_mem();

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
find_bar(unsigned long a)
{
	int i, n;
	unsigned int pci_addr, id, irq;

	unsigned long addr[6], size[6];
	unsigned long rom_addr, rom_size;

	FILE *f;

	mem_addr = 0;
	mem_size = 0;

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
		for (i = 0; i < 6; i++) {
			if (addr[i] & 1)
				continue; // not mem addr
			addr[i] &= ~15;
			if (addr[i] <= a && a < addr[i] + size[i]) {
				mem_addr = addr[i];
				mem_size = size[i];
				return;
			}
		}
	}
	printf("ERROR address %08lx not found in /proc/bus/pci/devices\n", a);
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

	mem_addr = 0;
	mem_size = 0;

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

void
toe(void)
{
	int cmd, fd;
	unsigned char secondary_bus;
	unsigned int mac1_bar, mac2_bar;

	if (find_device(0x18df, 0xbd9e) == -1) {
		printf("cannot find a toe card\n");
		return;
	}

	// get the secondary bus number

	sprintf(buf, "/proc/bus/pci/%02x/%02x.%d", bus, device, function);
	fd = open(buf, O_RDWR);
	if (fd == -1) {
		printf("cannot open %s\n", buf);
		return;
	}
	if (pread(fd, &secondary_bus, 1, 0x19) != 1) {
		perror("pread");
		close(fd);
		return;
	}
	close(fd);

	// get mac 1 addr

	sprintf(buf, "/proc/bus/pci/%02x/00.0", secondary_bus);
	fd = open(buf, O_RDWR);
	if (fd == -1) {
		printf("cannot open %s\n", buf);
		return;
	}
	if (pread(fd, &mac1_bar, 4, 0x14) != 4) {
		perror("pread");
		close(fd);
		return;
	}
	close(fd);

	// get mac 2 addr

	sprintf(buf, "/proc/bus/pci/%02x/01.0", secondary_bus);
	fd = open(buf, O_RDWR);
	if (fd == -1) {
		printf("cannot open %s\n", buf);
		return;
	}
	if (pread(fd, &mac2_bar, 4, 0x14) != 4) {
		perror("pread");
		close(fd);
		return;
	}
	close(fd);

	// printf("secondary bus %d port 1 bar %08x port 2 bar %08x\n", (int) secondary_bus, mac1_bar, mac2_bar);

	printf(" 0 exit\n");
	printf(" 1 print port 1 eeprom\n");
	printf(" 2 print port 2 eeprom\n");
	printf("31 edit port 1 eeprom\n");
	printf("32 edit port 2 eeprom\n");

	while (1) {
		printf("toe> ");
		fgets(buf, sizeof buf, stdin);
		if (sscanf(buf, "%d", &cmd) < 1)
			continue;
		switch (cmd) {
		case 0:
			return;
		case 1:
			toe_print_eeprom(mac1_bar);
			break;
		case 2:
			toe_print_eeprom(mac2_bar);
			break;
		case 31:
			toe_edit_eeprom(mac1_bar);
			break;
		case 32:
			toe_edit_eeprom(mac2_bar);
			break;
		}
	}
}

// each mac on the toe card has a 128 by 16-bit eeprom

#define EEPROM_CTRL ((unsigned short volatile *) (mem + a + 0x4a))
#define EEPROM_DATA ((unsigned short volatile *) (mem + a + 0x48))

void
toe_print_eeprom(unsigned long a)
{
	int i;
	unsigned int val;
	open_mem(a);
	if (mem == NULL)
		return;
	a -= mem_addr;
	for (i = 0; i < 128; i++) {
		if (i % 8 == 0)
			printf("%02x:", i);
		*EEPROM_CTRL = 0x0200 + i;
		do
			val = *EEPROM_CTRL;
		while (val & 0x8000);
		val = *EEPROM_DATA;
		printf(" %04x", val);
		if (i % 8 == 7)
			printf("\n");
	}
	close_mem();
}

void
toe_edit_eeprom(unsigned long a)
{
	int x = 0;
	unsigned int tmp, val;
	open_mem(a);
	if (mem == NULL)
		return;
	a -= mem_addr;
	while (1) {
		*EEPROM_CTRL = 0x0200 + x;	// read
		do
			tmp = *EEPROM_CTRL;
		while (tmp & 0x8000);
		val = *EEPROM_DATA;
		printf("%02x: %04x ", x, val);
		fgets(buf, sizeof buf, stdin);
		if (*buf == ',') {
			x = (x + 127) % 128;
			continue;
		}
		if (*buf == '\n') {
			x = (x + 1) % 128;
			continue;
		}
		if (sscanf(buf, "%x", &val) < 1)
			break;
		*EEPROM_CTRL = 0x00c0;		// enable write
		*EEPROM_CTRL = 0x0300 + x;	// erase
		do
			tmp = *EEPROM_CTRL;
		while (tmp & 0x8000);
		*EEPROM_DATA = val;
		*EEPROM_CTRL = 0x0100 + x;	// write
		do
			tmp = *EEPROM_CTRL;
		while (tmp & 0x8000);
		*EEPROM_CTRL = 0x0000;		// disable write
		do
			tmp = *EEPROM_CTRL;
		while (tmp & 0x8000);
		x = (x + 1) % 128;
	}
}
