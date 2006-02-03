// This is a Linux utility that programs the Gigatek boot flash in-situ.
// The boot flash is 512 Kbytes at addresses 0xff000000-0xff07ffff.
// The flash is aliased 16 times in the 8 meg region 0xff000000-0xff7fffff.
// The address range 0xff800000-0xffffffff is occupied by 512 Kbytes of SRAM
// which is also aliased 16 times.
// On power up, the flash data is copied into SRAM.
// Then the Power PC 7447A starts at address 0xfff00100.
// The actual range where the Power PC runs the code is 0xfff00000-0xffffffff.
// This utility expects the S record file to have addresses in that range.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>

#define YYLEN (512 * 1024)

void write_boot_flash(char *);

main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("usage: %s foo.srec\n", argv[0]);
		exit(1);
	}
	write_boot_flash(argv[1]);
}

int fd;
unsigned char *mem;
unsigned long mem_addr, mem_size;

void
open_mem(unsigned long addr)
{
	mem = 0;
	fd = open("/dev/mem", O_RDWR);
	if (fd == -1) {
		printf("error opening /dev/mem\n");
		return;
	}
	mem_addr = addr & ~0xfff;
	mem_size = YYLEN;
	mem = mmap(0, mem_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mem_addr);
	if (mem == 0) {
		perror("mmap");
		close(fd);
	}
}

void
close_mem(void)
{
	munmap(mem, mem_size);
	close(fd);
}

#define YYMEM ((unsigned char volatile *) mem)

unsigned char image[YYLEN];

int
read_srec(char *filename)
{
	int i;
	unsigned int c, len, addr;
	unsigned char buf[1000], sum;
	FILE *f;
	printf("reading %s\n", filename);
	f = fopen(filename, "r");
	if (f == NULL) {
		printf("cannot open %s\n", filename);
		return -1;
	}
	memset(image, 0xff, YYLEN);
	while(fgets(buf, 1000, f)) {
		if (buf[0] != 'S') {
			printf("the file has something that is not an S record\n");
			return -1;
		}
		if (buf[1] != '3')
			continue;
		sscanf(buf + 2, "%02x%08x", &len, &addr);
		sum = 0;
		for (i = 0; i < len + 1; i++) {
			sscanf(buf + 2 + 2 * i, "%02x", &c);
			sum += c;
		}
		if (sum != 0xff) {
			printf("checksum error\n");
			return -1;
		}
		len -=5;
		if (addr < 0xfff00000 || addr > 0xfff7ffff || len > 0xfff80000 - addr) {
			printf("address range error\n");
			exit(1);
		}
		addr -= 0xfff00000;
		for (i = 0; i < len; i++) {
			sscanf(buf + 12 + 2 * i, "%02x", &c);
			image[addr + i] = c;
		}
	}
	fclose(f);
	return 0;
}

unsigned int
read_byte(unsigned int addr)
{
	int d;
	open_mem(0xff000000);
	d = YYMEM[addr];
	close_mem();
	return d;
}

void
write_byte(unsigned int addr, unsigned int data)
{
	open_mem(0xff000000);
	YYMEM[addr] = data;
	close_mem();
}

void
erase_boot_flash(void)
{
	printf("erasing boot flash\n");

	write_byte(0x555, 0xaa);
	write_byte(0x2aa, 0x55);
	write_byte(0x555, 0x80);

	write_byte(0x555, 0xaa);
	write_byte(0x2aa, 0x55);
	write_byte(0x555, 0x10);

	while (read_byte(0) != 0xff)
		;

	write_byte(0, 0xf0); // reset
}

void
write_boot_flash(char *filename)
{
	int i;

	if (read_srec(filename))
		return;

	erase_boot_flash();

	for (i = 0; i < YYLEN; i++) {
		if ((i & 0xfff) == 0)
			printf("%08x\n", i);
		if (image[i] == 0xff)
			continue;
		write_byte(0x555, 0xaa);
		write_byte(0x2aa, 0x55);
		write_byte(0x555, 0xa0);
		write_byte(i, image[i]);
		while (read_byte(i) != image[i])
			;
	}

	write_byte(0, 0xf0); // reset

	// verify

	printf("verifying boot flash\n");

	open_mem(0xff000000);
	for (i = 0; i < YYLEN; i++)
		if (YYMEM[i] != image[i])
			break;
	close_mem();

	if (i == YYLEN)
		printf("ok\n");
	else
		printf("verify failed\n");

	close_mem();
}
