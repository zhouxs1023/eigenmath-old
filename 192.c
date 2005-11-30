/* Demo code for reading and writing an I${}^2$C EEPROM. */

static void
i2c_delay(void)
{
	mdelay(2);
}

#define DebugCtrl (mac->reg + 0x2c)

// GPIO 0 is the I2C clock (SCL)

// GPIO 1 is the I2C data (SDA)

static void
i2c_start(struct mac *mac)
{
	int tmp;

	// sda and scl both outputs, both high

	tmp = readw(DebugCtrl);
	tmp |= 15;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// sda high to low

	tmp &= ~8;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// scl high to low

	tmp &= ~4;
	writew(tmp, DebugCtrl);
	i2c_delay();
}

static void
i2c_stop(struct mac *mac)
{
	int tmp;

	// sda output low

	tmp = readw(DebugCtrl);
	tmp |= 2;
	tmp &= ~8;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// scl high

	tmp |= 4;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// sda and scl inputs (both pulled up high)

	tmp |= 8;
	tmp &= ~3;
	writew(tmp, DebugCtrl);
	i2c_delay();
}

static int
i2c_read_bit(struct mac *mac)
{
	int bit = 0, tmp = readw(DebugCtrl);

	// clock high

	tmp |= 4;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// read bit

	if (readw(DebugCtrl) & 8)
		bit = 1;
	mb();

	// clock low

	tmp &= ~4;
	writew(tmp, DebugCtrl);
	i2c_delay();

	return bit;
}

static void
i2c_write_bit(struct mac *mac, int bit)
{
	int tmp = readw(DebugCtrl);

	// data out

	tmp |= 10;
	if (bit == 0)
		tmp &= ~8;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// clock high

	tmp |= 4;
	writew(tmp, DebugCtrl);
	i2c_delay();

	// clock low and data in

	tmp &= ~6;
	writew(tmp, DebugCtrl);
	i2c_delay();
}

static int
i2c_read_reg(struct mac *mac, int addr)
{
	int i, tmp = 0;

	// dummy write to set reg addr

	i2c_start(mac);

	i2c_write_bit(mac, 1); // dev addr 6
	i2c_write_bit(mac, 0); // dev addr 5
	i2c_write_bit(mac, 0); // dev addr 4
	i2c_write_bit(mac, 0); // dev addr 3
	i2c_write_bit(mac, 1); // dev addr 2
	i2c_write_bit(mac, 0); // dev addr 1
	i2c_write_bit(mac, 0); // dev addr 0
	i2c_write_bit(mac, 0); // 0 = write

	i2c_read_bit(mac); // ack

	i2c_write_bit(mac, addr & 0x80); // reg addr 7
	i2c_write_bit(mac, addr & 0x40); // reg addr 6
	i2c_write_bit(mac, addr & 0x20); // reg addr 5
	i2c_write_bit(mac, addr & 0x10); // reg addr 4
	i2c_write_bit(mac, addr & 0x08); // reg addr 3
	i2c_write_bit(mac, addr & 0x04); // reg addr 2
	i2c_write_bit(mac, addr & 0x02); // reg addr 1
	i2c_write_bit(mac, addr & 0x01); // reg addr 0

	i2c_read_bit(mac); // ack

	i2c_stop(mac);

	// read MS byte

	i2c_start(mac);

	i2c_write_bit(mac, 1); // dev addr 6
	i2c_write_bit(mac, 0); // dev addr 5
	i2c_write_bit(mac, 0); // dev addr 4
	i2c_write_bit(mac, 0); // dev addr 3
	i2c_write_bit(mac, 1); // dev addr 2
	i2c_write_bit(mac, 0); // dev addr 1
	i2c_write_bit(mac, 0); // dev addr 0
	i2c_write_bit(mac, 1); // 1 = read

	i2c_read_bit(mac); // ack

	for (i = 0; i < 8; i++) {
		tmp <<= 1;
		tmp |= i2c_read_bit(mac);
	}

	i2c_read_bit(mac); // ack

	i2c_stop(mac);

	// read LS byte

	i2c_start(mac);

	i2c_write_bit(mac, 1); // dev addr 6
	i2c_write_bit(mac, 0); // dev addr 5
	i2c_write_bit(mac, 0); // dev addr 4
	i2c_write_bit(mac, 0); // dev addr 3
	i2c_write_bit(mac, 1); // dev addr 2
	i2c_write_bit(mac, 0); // dev addr 1
	i2c_write_bit(mac, 0); // dev addr 0
	i2c_write_bit(mac, 1); // 1 = read

	i2c_read_bit(mac); // ack

	for (i = 0; i < 8; i++) {
		tmp <<= 1;
		tmp |= i2c_read_bit(mac);
	}

	i2c_read_bit(mac); // ack

	i2c_stop(mac);

	return tmp;
}

void
i2c_write_reg(struct mac *mac, int addr, int val)
{
	// write ms byte

	i2c_start(mac);

	i2c_write_bit(mac, 1); // dev addr 6
	i2c_write_bit(mac, 0); // dev addr 5
	i2c_write_bit(mac, 0); // dev addr 4
	i2c_write_bit(mac, 0); // dev addr 3
	i2c_write_bit(mac, 1); // dev addr 2
	i2c_write_bit(mac, 0); // dev addr 1
	i2c_write_bit(mac, 0); // dev addr 0
	i2c_write_bit(mac, 0); // 0 = write

	i2c_read_bit(mac); // ack

	i2c_write_bit(mac, addr & 0x80); // reg addr 7
	i2c_write_bit(mac, addr & 0x40); // reg addr 6
	i2c_write_bit(mac, addr & 0x20); // reg addr 5
	i2c_write_bit(mac, addr & 0x10); // reg addr 4
	i2c_write_bit(mac, addr & 0x08); // reg addr 3
	i2c_write_bit(mac, addr & 0x04); // reg addr 2
	i2c_write_bit(mac, addr & 0x02); // reg addr 1
	i2c_write_bit(mac, addr & 0x01); // reg addr 0

	i2c_read_bit(mac); // ack

	i2c_write_bit(mac, val & 0x8000); // data 7
	i2c_write_bit(mac, val & 0x4000); // data 6
	i2c_write_bit(mac, val & 0x2000); // data 5
	i2c_write_bit(mac, val & 0x1000); // data 4
	i2c_write_bit(mac, val & 0x0800); // data 3
	i2c_write_bit(mac, val & 0x0400); // data 2
	i2c_write_bit(mac, val & 0x0200); // data 1
	i2c_write_bit(mac, val & 0x0100); // data 0

	i2c_read_bit(mac); // ack

	i2c_stop(mac);

	// write ls byte

	i2c_start(mac);

	i2c_write_bit(mac, 1); // dev addr 6
	i2c_write_bit(mac, 0); // dev addr 5
	i2c_write_bit(mac, 0); // dev addr 4
	i2c_write_bit(mac, 0); // dev addr 3
	i2c_write_bit(mac, 1); // dev addr 2
	i2c_write_bit(mac, 0); // dev addr 1
	i2c_write_bit(mac, 0); // dev addr 0
	i2c_write_bit(mac, 0); // 0 = write

	i2c_read_bit(mac); // ack

	i2c_write_bit(mac, addr & 0x80); // reg addr 7
	i2c_write_bit(mac, addr & 0x40); // reg addr 6
	i2c_write_bit(mac, addr & 0x20); // reg addr 5
	i2c_write_bit(mac, addr & 0x10); // reg addr 4
	i2c_write_bit(mac, addr & 0x08); // reg addr 3
	i2c_write_bit(mac, addr & 0x04); // reg addr 2
	i2c_write_bit(mac, addr & 0x02); // reg addr 1
	i2c_write_bit(mac, addr & 0x01); // reg addr 0

	i2c_read_bit(mac); // ack

	i2c_write_bit(mac, val & 0x0080); // data 7
	i2c_write_bit(mac, val & 0x0040); // data 6
	i2c_write_bit(mac, val & 0x0020); // data 5
	i2c_write_bit(mac, val & 0x0010); // data 4
	i2c_write_bit(mac, val & 0x0008); // data 3
	i2c_write_bit(mac, val & 0x0004); // data 2
	i2c_write_bit(mac, val & 0x0002); // data 1
	i2c_write_bit(mac, val & 0x0001); // data 0

	i2c_read_bit(mac); // ack

	i2c_stop(mac);
}
