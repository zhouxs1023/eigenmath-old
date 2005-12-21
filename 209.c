// Demo API code for a DSP board.

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <unistd.h>
#include <linux/unistd.h>
#include <errno.h>
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

#define MODE_NOWAIT 4

#define BUFLEN 1000
#define Y1MB (1024 * 1024)

static unsigned char buf[BUFLEN], image[Y1MB];
static unsigned end;

static void read_string(char *filename, int offset);

unsigned int
scan2(unsigned char *p)
{
	return 256 * p[1] + p[0];
}

unsigned int
scan4(unsigned char *p)
{
	return p[0] + 256 * p[1] + 256 * 256 * p[2] + 256 * 256 * 256 * p[3];
}

// read a board register

int
dsp_ioctl_get(int fd, int reg, int *data)
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
dsp_ioctl_set(int fd, int reg, int data)
{
	int buf[2], t;
	buf[0] = reg;
	buf[1] = data;
	t = ioctl(fd, WRITE_REG, buf);
	return t;
}

// read len bytes from DSP memory

int
dsp_read(int fd, unsigned char *buf, int len, int dspnum, int addr)
{
	int i, n, t;
	struct pollfd pollfd;

	// check alignment

	if ((len & 3) || (addr & 3)) {
		errno = EINVAL;
		return -1;
	}

	if (dspnum < 0 || dspnum > 7) {
		errno = EINVAL;
		return -1;
	}

	// ensure DMA channel is available

	pollfd.fd = fd;
	pollfd.events = POLLIN;
	t = poll(&pollfd, 1, -1);
	if (t < 1)
		return -1;

	// select the DSP

	t = dsp_ioctl_set(fd, REG_CHIP_SEL, 1 << dspnum);
	if (t < 0)
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
dsp_write(int fd, unsigned char *buf, int len, int mask, int addr)
{
	int i, n, t;
	struct pollfd pollfd;

	// check alignment

	if ((len & 3) || (addr & 3)) {
		errno = EINVAL;
		return -1;
	}

	// ensure DMA channel is available

	pollfd.fd = fd;
	pollfd.events = POLLOUT;
	t = poll(&pollfd, 1, 1000);
	if (t < 1)
		return -1;

	// select the DSPs

	t = dsp_ioctl_set(fd, REG_CHIP_SEL, mask);
	if (t < 0)
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

// read a section of COFF data

static int
read_coff_section_data(char *filename, int src, int dst, int len)
{
	int fd, n;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return -1;

	n = lseek(fd, src, SEEK_SET);
	if (n < src) {
		close(fd);
		return -1;
	}
	n = read(fd, image + dst, len);

	close(fd);

	if (n == len)
		return 0;
	else
		return -1;
}

// read a COFF file and store the binary in image[]

static int
read_coff_file(char *filename)
{
	int fd, i, n;
	int section_count;
	int addr, len, seek;

	// init vars

	memset(image, 0, Y1MB);
	end = 0;

	// open coff file

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return -1;

	// read the file header

	n = read(fd, buf, 22);
	if (n < 22) {
		close(fd);
		return -1;
	}

	section_count = scan2(buf + 2);

	// optional header

	if (buf[16])
		read(fd, buf, 28);

	// read each section

	for (i = 0; i < section_count; i++) {

		// read section header

		n = read(fd, buf, 48);
		if (n < 48) {
			close(fd);
			return -1;
		}

		addr = scan4(buf + 8);		// phys addr
		len = scan4(buf + 16);		// section size
		seek = scan4(buf + 20);		// file ptr

		// section name

		if (scan4(buf) == 0)
			read_string(filename, scan4(buf + 4));
		else
			buf[8] = 0;

		printf("section %d name=%s addr=0x%x len=%d seek=0x%x\n", i, buf, addr, len, seek);

		if (strncmp(buf, ".debug", 6) == 0)
			continue;

		if (end < addr + len)
			end = addr + len;

		if (end > Y1MB) {
			close(fd);
			return -1;
		}

		if (seek == 0 || len == 0)
			continue;

		n = read_coff_section_data(filename, seek, addr, len);

		if (n) {
			close(fd);
			return -1;
		}
	}

	close(fd);

	return 0;
}

// read a COFF file and download the code to DSPs

int
dsp_download(int fd, int mask, char *filename, int type)
{
	int t;

	// file type must be COFF

	if (type) {
		errno = EINVAL;
		return -1;
	}

	t = read_coff_file(filename);
	if (t)
		return -1;

	end = (end + 3) & ~3;

	t = dsp_write(fd, image, end, mask, 0);
	if (t)
		return -1;

	return 0;
}

// bounce DSP reset lines

int
dsp_reset(int fd, int mask)
{
	int t;
	t = dsp_ioctl_set(fd, REG_RESET, 0x00);
	if (t)
		return -1;
	usleep(100000);
	t = dsp_ioctl_set(fd, REG_RESET, 0xff);
	if (t)
		return -1;
	usleep(100000);
	return 0;
}

// start running downloaded DSP code

int
dsp_run(int fd, int dspmask)
{
	int i, t;
	for (i = 0; i < 8; i++) {
		if ((dspmask & (1 << i)) == 0)
			continue;
		t = dsp_ioctl_set(fd, REG_CHIP_SEL, 1 << i);
		if (t)
			return -1;
		dsp_ioctl_set(fd, REG_HPIC, 0xa00a);
		usleep(100000);
	}
	return 0;
}

static int symtab_offset, symtab_count;

// read a string from a COFF file

static void
read_string(char *filename, int offset)
{
	FILE *f;
	f = fopen(filename, "r");
	fseek(f, symtab_offset + 18 * symtab_count + offset, SEEK_SET);
	fgets(buf, BUFLEN, f);
	fclose(f);
}

// read a COFF file to get the value of a program symbol

int
dsp_symbol(char *filename, int type, char *symbol, unsigned *value)
{
	int aux, fd, i, t;

	// must be COFF

	if (type) {
		errno = EINVAL;
		return -1;
	}

	// open the file

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return -1;

	// read the file header

	t = read(fd, buf, 22);
	if (t < 22) {
		close(fd);
		return -1;
	}

	symtab_offset = scan4(buf + 8);
	symtab_count = scan4(buf + 12);

	// seek to start of symbol table

	t = lseek(fd, symtab_offset, SEEK_SET);
	if (t < symtab_offset) {
		close(fd);
		return -1;
	}

	for (i = 0; i < symtab_count; i++) {

		// read symbol record

		t = read(fd, buf, 18);
		if (t < 18) {
			close(fd);
			return -1;
		}

		aux = buf[17];

		// get symbol value

		*value = scan4(buf + 8);

		// get symbol name

		if (scan4(buf) == 0)
			read_string(filename, scan4(buf + 4));
		else
			buf[8] = 0;

		if (strcmp(symbol, buf) == 0) {
			close(fd);
			return 0;
		}

		// extra?

		if (aux) {
			t = read(fd, buf, 18);
			if (t < 18)
				return -1;
			i++;
		}
	}

	close(fd);

	return -1;
}

// for debugging

static void
print_symbols(char *filename)
{
	int aux, fd, i, value;
	fd = open(filename, O_RDONLY);
	read(fd, buf, 22);
	symtab_offset = scan4(buf + 8);
	symtab_count = scan4(buf + 12);
	lseek(fd, symtab_offset, SEEK_SET);
	for (i = 0; i < symtab_count; i++) {
		read(fd, buf, 18);
		aux = buf[17];
		value = scan4(buf + 8);
		if (scan4(buf) == 0)
			read_string(filename, scan4(buf + 4));
		else
			buf[8] = 0;
		printf("%08x %s\n", value, buf);
		if (aux) {
			read(fd, buf, 18);
			i++;
		}
	}
	close(fd);
}

// run c64timer.out, print a DSP program var every second

#define FILENAME "c64timer.out"

main()
{
	int addr, fd, val, i;
	int dspnum, dspmask;
	print_symbols(FILENAME);
	dspnum = 0;
	dspmask = 1 << dspnum;
	dsp_symbol(FILENAME, 0, "_cnt", &addr);
	fd = open("/dev/sigc641x", O_RDWR);
	val = MODE_NOWAIT;
	ioctl(fd, SET_READ_MODE, &val);
	ioctl(fd, SET_WRITE_MODE, &val);
	dsp_reset(fd, dspmask);
	dsp_download(fd, dspmask, FILENAME, 0);
	dsp_run(fd, dspmask);
	while (1) {
		sleep(1);
		dsp_read(fd, (unsigned char *) &val, 4, dspnum, addr);
		printf("count = %d\n", val);
	}
}
