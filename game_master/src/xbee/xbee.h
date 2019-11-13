/*
 * xbee.h
 *
 *  Created on: Mar 25, 2019
 *      Author: rmallya
 */

#ifndef XBEE_H_
#define XBEE_H_

#include "drivers/mss_uart/mss_uart.h"

#define ID_BROADCAST (~0)

typedef enum XType
{
	COORDINATOR,
	CLIENT
} XType;

// robot data struct
// TODO: figure out what will actually be transmitted and update struct
struct XData
{
	uint8_t data[16]; // limit to 32 bytes
};

// xbee packet
struct XPacket
{
	struct XData data;
	uint8_t sender_id;
	uint8_t target_id;
};

typedef void (*ReceiveFn)(const struct XData *);

struct XDevice
{
	XType type;
	uint8_t xid;
	ReceiveFn rcv;
};

extern struct XDevice device;

void XInit(XType, uint8_t, ReceiveFn);
void XBroadcast(const struct XData *);
void XSend(const struct XData *, uint8_t);

#endif /* XBEE_H_ */
