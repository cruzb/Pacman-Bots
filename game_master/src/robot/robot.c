/*
 * robot.c
 *
 *  Created on: Mar 26, 2019
 *      Author: rmallya
 */

#include "robot.h"
#include <string.h>
#include <stdio.h>
#include "CMSIS/mss_assert.h"

RobotRcv robot_rcv;
struct Robot robot;

void robot_receive_data(const struct XData *data)
{
	struct Robot r;
	memcpy(&r, data->data, sizeof(r));
	robot_rcv(&r);
}

void RobotSetId(struct Robot *r, robot_id_t id)
{
	r->id = id;
}

void RobotSetColor(struct Robot *r, const uint16_t *color)
{
	memcpy(r->data.seen_color, color, sizeof(uint16_t) * 3);
}

void RobotSetCmd(struct Robot *r, robot_cmd_t cmd)
{
	r->data.cmd = cmd;
}

robot_id_t RobotGetId(const struct Robot *r)
{
	return r->id;
}

const uint16_t *RobotGetColor(const struct Robot *r)
{
	return r->data.seen_color;
}

robot_cmd_t RobotGetCmd(const struct Robot *r)
{
	return r->data.cmd;
}

void RobotInit(robot_id_t id, RobotRcv rcv)
{
	robot_rcv = rcv;
	memset(&robot, 0, sizeof(robot));
	RobotSetId(&robot, id);
	XType type = id == SERVER ? COORDINATOR : CLIENT;
	XInit(type, id, &robot_receive_data);
}

void ServerInit(RobotRcv rcv)
{
	ASSERT(rcv);
	RobotInit(SERVER, rcv);
}

void ClientInit(robot_id_t id, RobotRcv rcv)
{
	ASSERT(rcv);
	RobotInit(id, rcv);
}

void RobotSend()
{
	ASSERT(robot.id != SERVER);
	struct XData data;
	memset(&data, 0, sizeof(data));
	memcpy(data.data, &robot, sizeof(robot));
	XSend(&data, SERVER);
}

void RobotBroadcast(const struct Robot *r)
{
	ASSERT(robot.id == SERVER);
	struct XData data;
	memset(&data, 0, sizeof(data));
	memcpy(data.data, r, sizeof(robot));
	XBroadcast(&data);
}

void RobotPrint(const struct Robot *r)
{
	printf("Id: %d\r\n", r->id);
	const uint16_t *color = RobotGetColor(r);
	printf("Color Seen: %u,%u,%u\r\n", color[0], color[1], color[2]);
	printf("Cmd: %u\r\n", RobotGetCmd(r));
}
