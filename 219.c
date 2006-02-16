// T8110 config utility by g. weigt

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

#define CPLD_BASE 0xe1000000
#define CPLD_CSR (CPLD_BASE + 4) // clock select register
#define CPLD_HRR (CPLD_BASE + 17) // hardware revision register

// T8110 device address

#define T8110_BASE 0xe2000000

#define T8110_MASTER_OUTPUT_ENABLE_REGISTER 0x103
#define T8110_DEVICE_ID_REGISTER 0x12a
#define T8110_MAIN_INPUT_SELECTOR 0x200
#define T8110_LSC01_SELECT 0x228
#define T8110_LSC23_SELECT 0x22a

#define LOCAL_BUS 0
#define H110_BUS 1

// T8110 connection memory base address

#define T8110_CONNECTION_MEMORY_BASE 0x40000

int read_byte(int);
void write_byte(int, int);

// t8110 functions

int read_t8110(int);
void write_t8110(int, int);
int read_t8110_device_id(void);
void enable_local_bus(void);
void disable_local_bus(void);
void enable_h_bus(void);
void disable_h_bus(void);
void print_clock_errors(void);
void set_pattern_mode(int, int, int);
void clear_connection_memory(void);
void reset_connection_memory(void);
int read_connection_memory(int);
void write_connection_memory(int, int);
void connect_stream_to_stream(int, int);
void connect_timeslot_to_timeslot(int, int);
void connect_timeslot_to_stream(int, int);
void disconnect_timeslot(int);
void disconnect_stream(int);
void connect_pattern_to_stream(int, int);
void connect_pattern_to_timeslot(int, int);

// cpld functions

int read_cpld_clock_select_register(void);
void write_cpld_clock_select_register(int);
void enable_local_bus_clocks(void);
void disable_local_bus_clocks(void);

void help(void);
void init_tags(void);

int old_hardware;
int connection_count;
int tag_count;
int tags[4096];

main(int argc, char *argv[])
{
	int c, d, m, n, p;
	int a = 0, state = 0;

	if ((read_byte(CPLD_HRR) & 0xf0) < 0x20)
		old_hardware = 1;

	if (read_t8110_device_id() ^ 0x8110) {
		printf("T8110 device error\n");
		exit(1);
	}

	if (argc < 2) {
		help();
		exit(1);
	}

	init_tags();

	for (;;) {

		c = getopt(argc, argv, "a:cdehp:rs:t:w:xz");

		switch (c) {

		case 'h':
			help();
			break;

		case 'a':
			a = strtol(optarg, NULL, 0);
			break;

		case 'z':
			init_tags();
			printf("%d connections, %d tags\n", connection_count, tag_count);
			break;

		case 'c':
			clear_connection_memory();
			break;

		case 'd':
			disable_local_bus();
			break;

		case 'e':
			enable_local_bus();
			break;

		case 'r':
			d = read_t8110(a);
			printf("%04x\n", d);
			break;

		case 'w':
			d = strtol(optarg, NULL, 0);
			write_t8110(a, d);
			break;

		case 'p':
			p = strtol(optarg, NULL, 0);
			state = 4;
			break;

		case 'x':
			state = 3;
			break;

		case 's':
			n = strtol(optarg, NULL, 0);
			if (n < 0 || n > 63) {
				printf("-s range error\n");
				exit(1);
			}
			switch (state) {
			case 0:
				m = n;
				state = 1;
				break;
			case 1:
				connect_stream_to_stream(m, n);
				state = 0;
				break;
			case 2:
				connect_timeslot_to_stream(m, n);
				state = 0;
				break;
			case 3:
				disconnect_stream(n);
				state = 0;
				break;
			case 4:
				connect_pattern_to_stream(p, n);
				state = 0;
				break;
			}
			break;

		case 't':
			n = strtol(optarg, NULL, 0);
			if (n < 0 || n > 8191) {
				printf("-t range error\n");
				exit(1);
			}
			switch (state) {
			case 0:
				m = n;
				state = 2;
				break;
			case 1:
				printf("cannot connect stream to timeslot\n");
				exit(1);
			case 2:
				connect_timeslot_to_timeslot(m, n);
				state = 0;
				break;
			case 3:
				disconnect_timeslot(n);
				state = 0;
				break;
			case 4:
				connect_pattern_to_timeslot(p, n);
				state = 0;
				break;
			}
			break;

		default:
			exit(1);
		}
	}
}

void
help(void)
{
	printf("-a0x200         set register offset to 0x200\n");
	printf("-c	        clear connection memory\n");
	printf("-d              disable local bus\n");
	printf("-e              enable local bus\n");
	printf("-r              read register\n");
	printf("-w0x1234        write 0x1234 to register\n");
	printf("-s0 -s1         connect stream 0 to 1 (streams 0-63)\n");
	printf("-t0 -t1         connect timeslot 0 to 1 (timeslots 0-8191)\n");
	printf("-t0 -s1         broadcast timeslot 0 to stream 1\n");
	printf("-x -s0          disconnect stream 0\n");
	printf("-x -t0          disconnect timeslot 0\n");
	printf("-p0x55 -s1      send pattern 0x55 to stream 1\n");
	printf("-p0x55 -t1      send pattern 0x55 to timeslot 1\n");
}

// read connection memory and set up the tags array

void
init_tags(void)
{
	int i, x, tag;
	connection_count = 0;
	tag_count = 0;
	memset(tags, 0, sizeof tags);
	for (i = 0; i < 8192; i++) {
		x = read_connection_memory(i);
		if (x & 0x08000000)
			connection_count++;
		if ((x & 0x0f000000) == 0x08000000) {
			tag = (x >> 8) & 0xfff;
			tags[tag] = i + 1;
			tag_count++;
		}
	}
}

int
alloc_tag(int n)
{
	int i;

	// see if already connected

	for (i = 0; i < 4096; i++)
		if (tags[i] == n + 1)
			return i;

	// find an unused tag

	for (i = 0; i < 4096; i++)
		if (tags[i] == 0)
			break;

	if (i == 4096) {
		printf("out of T8110 data memory\n");
		exit(1);
	}

	tags[i] = n + 1;

	return i;
}

// scan the tags table for n

// disconnect if found

void
free_tag(int n)
{
	int i;
	for (i = 0; i < 4096; i++)
		if (tags[i] == n + 1)
			tags[i] = 0;
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
	mem_size = 256 * 1024;
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

int
read_byte(int addr)
{
	int c;
	open_mem(addr);
	c = mem[addr - mem_addr];
	close_mem();
	return c;
}

void
write_byte(int addr, int data)
{
	int c;
	open_mem(addr);
	mem[addr - mem_addr] = data;
	close_mem();
}

// returns a 16-bit value

int
read_t8110(int addr)
{
	int c;
	if (old_hardware)
		addr *= 2;
	addr += T8110_BASE;
	open_mem(addr);
	c = (int) *((unsigned short *) (mem + addr - mem_addr));
	close_mem();
	return c & 0xffff;
}

void
write_t8110(int addr, int data)
{
	if (old_hardware)
		addr *= 2;
	addr += T8110_BASE;
	open_mem(addr);
	*((unsigned short *) (mem + addr - mem_addr)) = data;
	close_mem();
}

int
read_t8110_device_id(void)
{
	return read_t8110(T8110_DEVICE_ID_REGISTER);
}

int
read_connection_memory(int n)
{
	int addr, data, bus, stream, timeslot;
	bus = (n >> 12) & 1;
	stream = (n >> 7) & 0x1f;
	timeslot = n & 0x7f;
	addr = (timeslot << 8) | (bus << 7) | (stream << 2);
	addr += T8110_CONNECTION_MEMORY_BASE;
	data = read_t8110(addr);
	data |= read_t8110(addr + 2) << 16;
	return data;
}

void
write_connection_memory(int n, int data)
{
	int addr, bus, stream, timeslot;
	bus = (n >> 12) & 1;
	stream = (n >> 7) & 0x1f;
	timeslot = n & 0x7f;
	addr = (timeslot << 8) | (bus << 7) | (stream << 2);
	addr += T8110_CONNECTION_MEMORY_BASE;
	write_t8110(addr, data);
	write_t8110(addr + 2, data >> 16);
}

void
reset_connection_memory(void)
{
	write_t8110(0x48000, 0x0101);
	write_t8110(0x48002, 0x0101);
}

// writing 0x08000000 breaks the connection

// but on read-back, bit 27 when set indicates valid connection

// so on read-back 0x00000000 indicates a broken connecton

// write 0x08000000

// read back 0x00000000

void
clear_connection_memory(void)
{
	int i;
	for (i = 0; i < 8193; i++)
		write_connection_memory(i, 0x08000000);
}

void
write_t8110_main_input_selector(int d)
{
	d &= 0x00ff;
	d |= read_t8110(T8110_MAIN_INPUT_SELECTOR) & 0xff00;
	write_t8110(T8110_MAIN_INPUT_SELECTOR, d);
}

int
read_t8110_main_input_selector(void)
{
	return read_t8110(T8110_MAIN_INPUT_SELECTOR) & 0xff;
}

void
write_cpld_clock_select_register(int d)
{
	write_byte(CPLD_CSR, d);
}

int
read_cpld_clock_select_register(void)
{
	return read_byte(CPLD_CSR);
}

// enable local bus clock and data

void
enable_local_bus(void)
{
	int d;

	d = read_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER) | 3;
	write_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER, d);

	//	0x42		8.192 MHz clock
	//	0x80		frame clock
	//	0x81		netref 1
	//	0x82		netref 2

	write_t8110(T8110_LSC01_SELECT, 0x8042);
	write_t8110(T8110_LSC23_SELECT, 0x8281);

	enable_local_bus_clocks();
}

void
disable_local_bus(void)
{
	int d;

	d = read_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER) & ~3;
	write_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER, d);

	write_t8110(T8110_LSC01_SELECT, 0x0000);
	write_t8110(T8110_LSC23_SELECT, 0x0000);

	disable_local_bus_clocks();
}

// enable H.110 clock and data

void
enable_h_bus(void)
{
	int d;
	d = read_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER) | 0x0c;
	write_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER, d);
}

void
disable_h_bus(void)
{
	int d;
	d = read_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER) & ~0x0c;
	write_t8110(T8110_MASTER_OUTPUT_ENABLE_REGISTER, d);
}

void
print_clock_errors(void)
{
	int a, b;
	a = read_t8110(0x120);
	b = read_t8110(0x122);
	printf("%02x %02x %02x %02x\n",
		b >> 8 & 0xff,
		b & 0xff,
		(a >> 8) & 0xff,
		a & 0xff);
}

void
enable_local_bus_clocks(void)
{
	int d;
	d = read_cpld_clock_select_register() | 0x30;
	write_cpld_clock_select_register(d);
}

void
disable_local_bus_clocks(void)
{
	int d;
	d = read_cpld_clock_select_register() & ~0x30;
	write_cpld_clock_select_register(d);
}

// m, n = 0-63

// m, n = 32 * bus + stream

// bus = 0, 1 (bus 0 is the local bus)

// stream = 0-31

void
connect_stream_to_stream(int m, int n)
{
	int timeslot;
	for (timeslot = 0; timeslot < 128; timeslot++)
		connect_timeslot_to_timeslot(128 * m + timeslot, 128 * n + timeslot);
}

// m, n = 0-8193

// m, n = 4096 * bus + 128 * stream + timeslot

// bus = 0, 1 (bus 0 is the local bus)

// stream = 0-31

// timeslot = 0-127

void
connect_timeslot_to_timeslot(int m, int n)
{
	int tag;
	free_tag(n);
	tag = alloc_tag(m);
	write_connection_memory(m, tag << 8);
	write_connection_memory(n, (tag << 8)  | 0x04000000);
}

void
connect_timeslot_to_stream(int m, int n)
{
	int timeslot;
	for (timeslot = 0; timeslot < 128; timeslot++)
		connect_timeslot_to_timeslot(m, 128 * n + timeslot);
}

void
connect_pattern_to_timeslot(int pattern, int n)
{
	free_tag(n);
	pattern = ((pattern & 0xff) << 8) | 0x05000000;
	write_connection_memory(n, pattern);
}

void
connect_pattern_to_stream(int pattern, int n)
{
	int timeslot;
	for (timeslot = 0; timeslot < 128; timeslot++)
		connect_pattern_to_timeslot(pattern, 128 * n + timeslot);
}

void
disconnect_timeslot(int n)
{
	free_tag(n);
	write_connection_memory(n, 0x08000000);
}

void
disconnect_stream(int n)
{
	int timeslot;
	for (timeslot = 0; timeslot < 128; timeslot++)
		disconnect_timeslot(128 * n + timeslot);
}
