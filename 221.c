// Write 5561 flash w/o driver.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>
#include <linux/unistd.h>
#include <asm/ioctl.h>

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
#define REG_FLASH_ADDR   15
#define REG_FLASH_DATA   16

#define Y16M (16 * 1024 * 1024)

#define ADDR 0xebffff80

void open_mem(void);
void read_file(char *, unsigned int);
void WriteReg(int, int);
int ReadReg(int);
void reset_flash(void);
void erase_flash(void);
unsigned int read_flash(unsigned int);
void write_flash(unsigned int, unsigned int);

unsigned char image[Y16M];
int fd;
unsigned char *mem;
unsigned int mem_addr, mem_size;

main(int argc, char *argv[])
{
	int i, j, n = 0;
	unsigned int d, t;

	memset(image, 0xff, Y16M);

#if 0

	read_file("resetcfg.srec", 0x2000);
	read_file("vxboot.srec", 0xc00000 - 0xbfc00000);

  	fd = open("/dev/sigc641x", O_RDWR);

	if (fd == -1) {
		perror("/dev/sigc641x");
		exit(0);
	}

#endif

//	WriteReg(REG_CHIP_SEL, 1); // Select first dsp

//	reset_flash();

#if 1
	for (i = 0; i < 8 * 1024 * 1024; i++) {
		d = read_flash(i);
		if (d)
			printf("%08x %04x\n", i, d);
	}
	exit(1);
#endif


//	for (i = 0; i < 1000; i++)
//		printf("%04x\n", read_flash(0));
//exit(1);

	printf("erasing flash\n");

	erase_flash();
exit(1);
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
			break;
		printf("failed at addr=%x %x %x\n", i, d, t);
		exit(1);
	}

	printf("ok\n");
}

void
open_mem(void)
{
	mem = 0;
//	find_bar(addr);
//	if (mem_addr == 0)
//		return;
	fd = open("/dev/mem", O_RDWR);
	if (fd == -1) {
		printf("error opening /dev/mem\n");
		return;
	}
mem_addr = ADDR & ~0xfff;
mem_size = 4096;
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
		if (addr + len > 16 * 1024 * 1024) {
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

int
swap(unsigned int x)
{
	unsigned char d[4];
	*((unsigned int *) d) = x;
	return (d[3] << 24) | (d[2] << 16) | (d[1] << 8) | d[0];
}

#define MEM ((unsigned int volatile *) (mem + ADDR - mem_addr))

void 
WriteReg(int reg, int data)
{
#if 0
	int buf[2];
	buf[0] = reg;
	buf[1] = data;
	ioctl(fd, WRITE_REG, buf);
#else
	open_mem();
	MEM[reg] = data;
	close_mem();
#endif
}

int 
ReadReg(int reg)
{
#if 0
	int buf[2], k;
	buf[0] = reg;
	buf[1] = 0;
	k = ioctl(fd, READ_REG, buf);
	if (k) {
		perror("ReadReg");
		exit(1);
	}
	return buf[1];
#else
	int d;
	open_mem();
	d = MEM[reg];
	close_mem();
	return d;
#endif
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
	int d, n = 0;

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

	while (1) {
		d = read_flash(0);
//		printf("%d %04x\n", n++, d);
		if ((d & 0xff) == 0xff)
			break;
		sleep(1);
	}
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
