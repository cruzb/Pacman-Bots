/*
 * xbee.cc
 *
 *  Created on: Mar 25, 2019
 *      Author: rmallya
 */

#include "xbee.h"
#include <string.h>
#include "CMSIS/mss_assert.h"

struct XDevice device;

struct
{
	struct XPacket packet;
	uint32_t curr_byte_pos;
} curr_packet;

void reset_curr_packet()
{
	memset(&curr_packet, 0, sizeof(curr_packet));
}

int handle_recv_data(uint8_t *data, size_t size_data)
{
	uint8_t *curr_pos = ((uint8_t *)(&curr_packet.packet)) + curr_packet.curr_byte_pos;
	memcpy(curr_pos, data, size_data);
	curr_packet.curr_byte_pos += size_data;
	return curr_packet.curr_byte_pos >= sizeof(struct XPacket);
}

struct XData *extract_data(struct XPacket *packet)
{
	if (packet->sender_id != device.xid &&
			(packet->target_id == device.xid || packet->target_id == (uint8_t)ID_BROADCAST))
	{
		return &packet->data;
	}
	return NULL;
}

void data_rcv_irq(mss_uart_instance_t *this_uart)
{
	uint8_t data[8];
	size_t num_bytes = MSS_UART_get_rx(&g_mss_uart1, data, sizeof(data));
	if (handle_recv_data(data, num_bytes))
	{
		struct XData *xdata = extract_data(&curr_packet.packet);
		if (xdata && device.rcv)
			device.rcv(xdata);
		reset_curr_packet();
	}
}

void send_data(const struct XPacket *packet)
{
	MSS_UART_polled_tx(&g_mss_uart1, (const uint8_t *)(packet), sizeof(*packet));
}

void XInit(XType type, uint8_t id, ReceiveFn fn)
{
	// init device
	device.type = type;
	device.xid = id;
	device.rcv = fn;

	// init current packet
	reset_curr_packet();

	// init uart
	MSS_UART_init(&g_mss_uart1, MSS_UART_9600_BAUD, MSS_UART_DATA_8_BITS | MSS_UART_NO_PARITY);
	MSS_UART_set_rx_handler(&g_mss_uart1, &data_rcv_irq, MSS_UART_FIFO_EIGHT_BYTES);
	MSS_UART_enable_irq(&g_mss_uart1, MSS_UART_RBF_IRQ);
}

void XBroadcast(const struct XData *data)
{
	ASSERT(device.type == COORDINATOR);
	XSend(data, ID_BROADCAST);
}

void XSend(const struct XData *data, uint8_t target_id)
{
	struct XPacket packet;
	packet.data = *data;
	packet.target_id = target_id;
	packet.sender_id = device.xid;
	send_data(&packet);
}
