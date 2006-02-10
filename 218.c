// Another flash device programming utility.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>
#include "sigc641x.h"

#define Y16M (16 * 1024 * 1024)

void read_file(char *, unsigned int);
void WriteReg(int, int);
int ReadReg(int);
void reset_flash(void);
void erase_flash(void);
unsigned int read_flash(unsigned int);
void write_flash(unsigned int, unsigned int);

unsigned char image[Y16M];
int fd;

main(int argc, char *argv[])
{
	int i, j, n = 0;
	unsigned int d, t;

	memset(image, 0xff, Y16M);

	read_file("resetcfg.srec", 0x2000);
	read_file("vxboot.srec", 0xc00000 - 0xbfc00000);

  	fd = open("/dev/sigc641x", O_RDWR);

	if (fd == -1) {
		perror("/dev/sigc641x");
		exit(1);
	}

	WriteReg(REG_CHIP_SEL, 1); // Select first dsp

	reset_flash();

	printf("erasing flash\n");

	erase_flash();

	printf("writing flash\n");

	for (i = 0; i < Y16M / 2; i++) {
		if ((i & 0xfffff) == 0)
			printf("%06x\n", i);
		d = (image[2 * i + 1] << 8) | image[2 * i];
		if (d == 0xffff)
			continue;
		write_flash(i, d);
		n++;
	}

	printf("%d writes\n", n);

	printf("verifying flash\n");

	reset_flash();

	for (i = 0; i < Y16M / 2; i++) {
		d = (image[2 * i + 1] << 8) | image[2 * i];
		t = read_flash(i);
		if (d == t)
			continue;
		printf("failed at addr=%x %x %x\n", i, d, t);
		exit(1);
	}

	printf("ok\n");
}

void
read_file(char *filename, unsigned int offset)
{
	int i;
	unsigned int c, len, addr;
	unsigned char buf[1000], sum;
	FILE *f;
	printf("reading %s\n", filename);
	f = fopen(filename, "r");
	if (f == NULL) {
		printf("cannot open %s\n", filename);
		exit(1);
	}
	while(fgets(buf, 1000, f)) {
		if (buf[0] != 'S') {
			printf("the file has something that is not an S record\n");
			exit(1);
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
			exit(1);
		}
		len -=5;
		addr += offset;
		if (addr + len > Y16M) {
			printf("address range error\n");
			exit(1);
		}
		for (i = 0; i < len; i++) {
			sscanf(buf + 12 + 2 * i, "%02x", &c);
			image[addr + i] = c;
		}
	}
	fclose(f);
}

void 
WriteReg(int reg, int data)
{
	int buf[2];
	buf[0] = reg;
	buf[1] = data;
	ioctl(fd, SIGC6415_WRITE_REG, buf);
}

int 
ReadReg(int reg)
{
	int buf[2], k;
	buf[0] = reg;
	buf[1] = 0;
	k = ioctl(fd, SIGC6415_READ_REG, buf);
	if (k) {
		perror("ReadReg");
		exit(1);
	}
	return buf[1];
}

void
reset_flash(void)
{
	WriteReg(REG_FLASH_ADDR, 0x00);
	WriteReg(REG_FLASH_DATA, 0xf0);
}

void 
erase_flash(void)
{
	int n = 0;

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x0AA);

	WriteReg(REG_FLASH_ADDR, 0x2AA);
	WriteReg(REG_FLASH_DATA, 0x055);

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x080);

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x0AA);

	WriteReg(REG_FLASH_ADDR, 0x2AA);
	WriteReg(REG_FLASH_DATA, 0x055);

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x010);

	while (read_flash(0) != 0xffff)
		n++;

	printf("%d\n", n);
}

unsigned int
read_flash(unsigned int addr)
{
	int d;
	WriteReg(REG_FLASH_ADDR, addr);
	d = ReadReg(REG_FLASH_DATA) & 0xffff;
	return d;
}

void 
write_flash(unsigned int addr, unsigned int data)
{
//	printf("write_flash addr=0x%08x data=0x%04x\n", addr, data);

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x0AA);

	WriteReg(REG_FLASH_ADDR, 0x2AA);
	WriteReg(REG_FLASH_DATA, 0x055);

	WriteReg(REG_FLASH_ADDR, 0x555);
	WriteReg(REG_FLASH_DATA, 0x0A0);

	WriteReg(REG_FLASH_ADDR, addr);
	WriteReg(REG_FLASH_DATA, data);

	while (read_flash(addr) != data)
		;
}
