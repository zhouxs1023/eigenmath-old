// This utility writes a PLX chip EEPROM from the file ``src.''

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>
#include <sys/poll.h>

void find_bar(unsigned long);
void edit_pci(int, int, int, int);
void scan_pci(void);
void open_mem(unsigned long);
void fix_crc(void);

int fd;
unsigned char *mem;
unsigned long mem_addr, mem_size;
int bus, device, function, irq;
unsigned long base_addr[6], size[6];
unsigned long rom_base_addr, rom_size;

#define LENGTH 12180
unsigned char buf[LENGTH];

#define CTRL ((volatile unsigned short *) (mem + 0x260))
#define STAT ((volatile unsigned short *) (mem + 0x262))
#define DATA ((volatile unsigned int *) (mem + 0x264))

#define BUSY 4

#define WREN 0xc000
#define CLEAR_WRITE_ENABLE_LATCH 0x8000
#define WRITE_EEPROM 0x4000
#define READ_EEPROM 0x6000

#define VENDOR_ID 0x10b5
#define DEVICE_ID 0x8516

char str[1000];

main(int argc, char *argv[])
{
	int i, n, y1, y2, y3, y4, y5, y6;
	unsigned int tmp;
	FILE *f;

	if (find_device(VENDOR_ID, DEVICE_ID) < 0)
		exit(1);

	printf("device BAR is %08x\n", base_addr[0] & ~15);

	open_mem(base_addr[0] & ~15);

	if (mem == NULL)
		exit(1);

	// read config file

	f = fopen("src", "r");
	if (f == NULL) {
		printf("cannot open src\n");
		exit(1);
	}

	for (i = 0; i < 3045; i++) {
		if (fgets(str, 1000, f) == NULL) {
			printf("error reading src\n");
			exit(1);
		}
		n = sscanf(str, "%x %x %x %x %x %x", &y1, &y2, &y3, &y4, &y5, &y6);
		if (n < 6) {
			printf("error reading src\n");
			exit(1);
		}
		buf[4 * i + 0] = y3;
		buf[4 * i + 1] = y4;
		buf[4 * i + 2] = y5;
		buf[4 * i + 3] = y6;
	}

	fclose(f);

	fix_crc();

	// write data

	for (i = 0; i < LENGTH; i += 4) {

		tmp = buf[i] << 24 | buf[i + 1] << 16 | buf[i + 2] << 8 | buf[i + 3];

		// read eeprom

		*CTRL = READ_EEPROM | (i / 4); do usleep(10); while (*STAT & BUSY);

		if (*DATA == tmp)
			continue;

		printf("write addr %04x data %08x\n", i, tmp);

		*CTRL = WREN; do usleep(10); while (*STAT & BUSY);
		*CTRL = 0xa000; do usleep(10); while (*STAT & BUSY);
		if ((*STAT & 0x200) == 0) {
			printf("problem with write enable i=%04x stat=%04x\n", i, *STAT);
			exit(1);
		}
		*DATA = tmp;
		usleep(10);
		*CTRL = WRITE_EEPROM | (i / 4); do usleep(10); while (*STAT & BUSY);

		// read status

		*CTRL = 0xa000; do usleep(10); while (*STAT & BUSY);
	}

	// reset write enable latch

	*CTRL = CLEAR_WRITE_ENABLE_LATCH;
	while (*STAT & BUSY)
		;

	printf("checking eeprom\n");

	for (i = 0; i < LENGTH; i += 4) {
		*CTRL = READ_EEPROM | (i / 4);
		while (*STAT & BUSY)
			;
		tmp = buf[i] << 24 | buf[i + 1] << 16 | buf[i + 2] << 8 | buf[i + 3];
		if (tmp == *DATA)
			continue;
		printf("verify failed at address %04x\n", i);
		printf("wrote %08x, read %08x\n", tmp, *DATA);
		exit(1);
	}
#if 0
	// dump eeprom

	for (i = 0; i < LENGTH; i += 4) {
		*CTRL = READ_EEPROM | (i / 4);
		while (*STAT & BUSY)
			;
		tmp = *DATA;
		printf("%04x %04x %02x %02x %02x %02x\n", i / 4, i, tmp >> 24, tmp >> 16 & 0xff, tmp >> 8 & 0xff, tmp & 0xff);
	}
#endif
}

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
		if (b == (vendor_id << 16) + device_id && (base_addr[0] & ~15)) {
			bus = a >> 8;
			device = a >> 3 & 0x1f;
			function = a & 7;
			return 0;
		}
	}
	printf("cannot find device\n");
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

#define POLY 0xdb710641

void
fix_crc(void)
{
	int i, j;
	unsigned int crc, tmp;

	crc = ~0;

	for (i = 0; i < LENGTH - 4; i += 4) {
		crc ^= buf[i] << 24 | buf[i + 1] << 16 | buf[i + 2] << 8 | buf[i + 3];
		for (j = 0; j < 32; j++)
			if (crc & 0x80000000)
				crc = (crc << 1) ^ POLY;
			else
				crc <<= 1;
	}

	tmp = buf[i] << 24 | buf[i + 1] << 16 | buf[i + 2] << 8 | buf[i + 3];

	if (tmp == crc) {
		printf("crc is ok\n");
		return;
	}

	printf("repairing the crc\n");

	buf[LENGTH - 4] = crc >> 24;
	buf[LENGTH - 3] = crc >> 16;
	buf[LENGTH - 2] = crc >> 8;
	buf[LENGTH - 1] = crc;
}
