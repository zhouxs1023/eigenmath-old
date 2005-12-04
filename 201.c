/* a simple ospf implementation */

#define MAX_INTERFACES 16

#define MAX_NEIGHBORS 16

struct interface {

	int link_state;

	int type;

	int state;

	unsigned int ipaddr;

	unsigned int netmask;

	int area;

	int hello_interval;

	int router_dead_interval;

	int transmit_delay;

	int router_priority;

	int hello_timer;

	int wait_timer;

	int neighbors;

	int designated_router;

	int backup_designated_router;

	int cost;

	int rx_tx_interval;

	int auth_key;

} interface[MAX_INTERFACES];

check_ospf()

{

	int i;

	for (i = 0; i < num_interfaces; i++)

		check_hello_interval(p)

}

check_hello_interval(struct interface *p)

{

	unsigned char *buf;

	unsigned int t;

	t = time(NULL);

	if (t - p->hello_timer < p->hello_interval)

		return;

	p->hello_timer += p->hello_interval;

	buf = alloc_buf();

	if (buf == NULL)

		return;

	make_hello_packet(buf);

	send_packet(p, buf);

}

make_hello_packet(unsigned char *buf, struct interface *p)

{

	make_ospf_header(buf, p);

	for (i = 0; i < p->neighbor_count; i++)

		((unsigned int *) buf)[i + 10] = p->neighbor[i];

	((unsigned short *) buf)[1] = htons(4 * p->neighbor_count + 40);

	make_ospf_checksum(buf);

}

make_ospf_header(unsigned char *buf, interface *p)

{

	buf[0] = 1;

	buf[1] = 1;

	*((int *) (buf + 4)) = p->router_id;

	*((int *) (buf + 8)) = p->area_id;

	*((short *) (buf + 14)) = 0;

	*((int *) (buf + 16)) = 0;

	*((int *) (buf + 20)) = 0;

}

make_ospf_checksum(unsigned char *buf)

{

	int i, n;

	unsigned int checksum = 0;

	n = (htons(((unsigned short *) buf)[1]) + 1) / 2;

	for (i = 0; i < 6; i++) {

		checksum += htons(((unsigned short *) buf)[i];)

		if (checksum & 0x10000)

			checksum = (checksum + 1) & 0xffff;

	}

	checksum += htons(((unsigned short *) buf)[7]);

	if (checksum & 0x10000)

		checksum = (checksum + 1) & 0xffff;

	for (i = 12; i < n; i++) {

		checksum += htons(((unsigned short *) buf)[i]);

		if (checksum & 0x10000)

			checksum = (checksum + 1) & 0xffff;

	}

	((unsigned short *) buf)[6] = ~ntohs(checksum);

}

receive_hello_packet(struct interface *p, unsigned char *buf)
{
}


/* Notes

	1. There are two state machines, the interface state machine and the neighbor state
	machine.

*/

/* interface states */

#define INTERFACE_DOWN 0
#define INTERFACE_LOOPBACK 1
#define INTERFACE_WAITING 2
#define INTERFACE_POINT_TO_POINT 3
#define INTERFACE_DR_OTHER 4
#define INTERFACE_BACKUP 5
#define INTERFACE_DR 6

/* interface events */

#define INTERFACE_UP 1
#define WAIT_TIMER 2
#define BACKUP_SEEN 3
#define NEIGHBOR_CHANGE 4
#define LOOP_IND 5
#define UNLOOP_IND 6
#define INTERFACE_DOWN 7

/* neighbor states */

#define DOWN 0
#define INIT 1
#define TWOWAY 2
#define EXSTART 3
#define EXCHANGE 4
#define LOADING 5
#define FULL 6

/* events */

#define HELLO_RECEIVED 0
#define START 1
#define TWO_WAY_RECEIVED 2
#define NEGOTIATION_DONE 3
#define EXCHANGE_DONE 4
#define SEQ_NUMBER_MISMATCH 5
#define BAD_LS_REQ 6
#define LOADING_DONE 7
#define ADJ_OK 8
#define ONE_WAY 9
#define KILL_NBR 10
#define INACTIVITY_TIMER 11
#define LL_DOWN 12

/* the neighbor state machine */

check_neighbor_state(struct interface *p, int event)

{

	switch (p->ns) {

	case DOWN:

		if (event == HELLO_RECEIVED)

			p->ns = INIT;

		break;

	}
}

check_down_state(struct interface *p, int event)
{
	switch (event) {
	case START:
		send_hello_packet(p);
		p->state = ATTEMPT;
		break;
	case HELLO_RECEIVED:
		p->state = INIT;
		break;
	default:
		break;
	}
}

check_attempt_state(struct interface *p, int event)
{
	switch (event) {
	case HELLO_RECEIVED:
		p->state = INIT;
		break;
	default:
		break;
	}
}

scan_hello_packet(struct interface *p, unsigned char *buf)

{

	int n;

	/* number of neighbors */

	n = scan_uint16(buf + 2);

	n = (n - 40) / 4;

	/* skip headers */

	buf += 40;

	/* check to see if this router is in the list */

	for (i = 0; i < n; i++)

		if (p->
		scan_hello_neighbor(buf + 4 * i);

}

scan_uint16(unsigned short *p)
{
	return ntohs(*p);
}
