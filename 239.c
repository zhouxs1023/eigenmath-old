// firewire test

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <libraw1394/raw1394.h>

int num_ports;

main()
{
	int i, j;
	count_ports();
	printf("There are %d firewire ports\n", num_ports);
	for (i = 0; i < num_ports; i++) {
		for (j = 0; j < num_ports; j++) {
			if (i == j)
				continue;
			test_async(i, j);
			test_isoch(i, j);
		}
	}
	exit(0);
}

char src_buf[1000], dst_buf[1000];

// async mode addresses (one per port)

#define A0 0x00000

#if 0
#define A1 0x000100010000ULL
#define A2 0x000100020000ULL
#else
#define A1 0x000080010000ULL
#define A2 0x000080020000ULL
#endif

struct raw1394_portinfo info[100];
struct raw1394_arm_reqhandle a;
struct raw1394_reqhandle r;

raw1394handle_t h, h0, h1, h2;
int node0, node1, node2;

// This is called when a packet is received.

int arm_callback(
	raw1394handle_t h,
	struct raw1394_arm_request_response *arr,
	unsigned int requested_length,
	void *pcontext,
	byte_t request_type)
{
#if 0
	printf("*** arm_callback() requested_length=%d request_type=%d\n", requested_length, (int) request_type);
	if (h == h0)
		printf("From port 0:\n");
	else if (h == h1)
		printf("From port 1:\n");
	else if (h == h2)
		printf("From port 2:\n");
	if (arr && arr->request) {
		printf("    arm request:\n");
		printf("      buffer_length = %d\n", arr->request->buffer_length);
		printf("      buffer = %llx\n", arr->request->buffer);
		printf("%s\n", arr->request->buffer);
	}
	if (arr && arr->response) {
		printf("    arm response:\n");
		printf("      response_code = %d\n", arr->response->response_code);
	}
	printf("    type=%d\n", (int) request_type);
#endif
	memcpy(dst_buf, arr->request->buffer, arr->request->buffer_length);
	return 0;
}

// This is called when a packet is written.

int write_callback(
	raw1394handle_t h,
	void *data,
	raw1394_errcode_t err)
{
#if 0
	printf("*** callback() %llx %s %08x\n", h, data, (int) err);
	if (h == h0)
		printf("h0\n");
	else if (h == h1)
		printf("h1\n");
	else if (h == h2)
		printf("h2\n");
#endif
	return 0;
}

test_async(int src, int dst)
{
	dst_buf[0] = 0;
	sprintf(src_buf, "sending an async message from port %d to port %d", src, dst);
	printf("%s\n", src_buf);

	h0 = raw1394_new_handle_on_port(src);
	h1 = raw1394_new_handle_on_port(dst);

	// node id

	node0 = raw1394_get_local_id(h0);
	node1 = raw1394_get_local_id(h1);

	// register

	a.arm_callback = &arm_callback;

	raw1394_arm_register(h0, A0, 4096, NULL, (octlet_t) &a, 2, 2, 0);
	raw1394_arm_register(h1, A1, 4096, NULL, (octlet_t) &a, 2, 2, 0);

	// send

	r.callback = write_callback;
	raw1394_start_write(h0, node1, A1, strlen(src_buf) + 1, (quadlet_t *) src_buf, (unsigned long) &r);

	// loop iterate

	raw1394_loop_iterate(h0);
	raw1394_loop_iterate(h1);

	// arm unregister

	raw1394_arm_unregister(h0, A0);
	raw1394_arm_unregister(h1, A1);

	raw1394_destroy_handle(h0);
	raw1394_destroy_handle(h1);

	if (strcmp(src_buf, dst_buf) == 0)
		printf("ok\n");
	else {
		printf("not ok\n");
		exit(1);
	}
}

int fd0, fd1;

void
check_fd0(void)
{
	int n;
	struct pollfd k;

	k.fd = fd0;
	k.events = POLLIN;

	if (poll(&k, 1, 1000) < 1)
		return;

	if (k.revents & POLLIN)
		n = raw1394_loop_iterate(h0);
}

void
check_fd1(void)
{
	struct pollfd k;

	k.fd = fd1;
	k.events = POLLIN;

	if (poll(&k, 1, 1000) < 1)
		return;

	if (k.revents & POLLIN)
		raw1394_loop_iterate(h1);
}

// send src_buf

enum raw1394_iso_disposition
raw1394_iso_xmit_handler(
        raw1394handle_t handle,
	unsigned char *data,
	unsigned int *len,
	unsigned char *tag,
	unsigned char *sy,
	int cycle,
	unsigned int dropped)
{
	memcpy(data, src_buf, strlen(src_buf) + 1);
	*len = strlen(src_buf) + 1;
	return RAW1394_ISO_OK;
}

// isochronous receive callback

enum raw1394_iso_disposition
raw1394_iso_recv_handler(
	raw1394handle_t handle,
	unsigned char *data,
	unsigned int len,
	unsigned char channel,
	unsigned char tag,
	unsigned char sy,
	unsigned int cycle,
	unsigned int dropped)
{
	strcpy(dst_buf, data);
	return RAW1394_ISO_OK;
}

// buf_packets must be set to 1 to interrupt on each received packet

#define BUF_PACKETS 1
#define MAX_PACKET_SIZE 1000
#define CHANNEL 2
#define IRQ_INTERVAL 1

test_isoch(int src, int dst)
{
	dst_buf[0] = 0;
	sprintf(src_buf, "sending an isochronous message from port %d to port %d", src, dst);
	printf("%s\n", src_buf);

	h0 = raw1394_new_handle_on_port(src);
	h1 = raw1394_new_handle_on_port(dst);

	fd0 = raw1394_get_fd(h0);
	fd1 = raw1394_get_fd(h1);

	int n;

	// init

	raw1394_iso_xmit_init(
		h0,
		raw1394_iso_xmit_handler,
		BUF_PACKETS,
		MAX_PACKET_SIZE,
		CHANNEL,
		RAW1394_ISO_SPEED_400,
		IRQ_INTERVAL);

	raw1394_iso_recv_init(
		h1,
		raw1394_iso_recv_handler,
		BUF_PACKETS,
		MAX_PACKET_SIZE,
		CHANNEL,
		RAW1394_DMA_PACKET_PER_BUFFER,
		IRQ_INTERVAL);

	// start

	raw1394_iso_xmit_start(h0, -1, -1);

	raw1394_iso_recv_start(h1, -1, -1, 0);

	check_fd0();	// transmit
	check_fd1();	// receive

	// done

	raw1394_iso_stop(h0);
	raw1394_iso_shutdown(h0);

	raw1394_iso_stop(h1);
	raw1394_iso_shutdown(h1);

	raw1394_destroy_handle(h0);
	raw1394_destroy_handle(h1);

	if (strcmp(src_buf, dst_buf) == 0)
		printf("ok\n");
	else {
		printf("not ok\n");
		printf("sent: %s\n", src_buf);
		printf("recv: %s\n", dst_buf);
		exit(1);
	}
}

// Determine the number of firewire ports.

count_ports()
{
	num_ports = 0;
	while (1) {
		h0 = raw1394_new_handle_on_port(num_ports);
		if (h0 == NULL)
			break;
		raw1394_destroy_handle(h0);
		num_ports++;
	}
}
