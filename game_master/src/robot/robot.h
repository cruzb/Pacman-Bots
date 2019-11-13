/*
 * robot.h
 *
 *  Created on: Mar 26, 2019
 *      Author: rmallya
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "../xbee/xbee.h"

typedef enum robot_id_t
{
	SERVER,
	PLAYER,
	GHOST_1,
	GHOST_2,
} robot_id_t;

typedef enum robot_cmd
{
	START,
	STOP,
	CALIBRATE
} robot_cmd_t;

struct Robot
{
	union
	{
		uint16_t seen_color[3];
		robot_cmd_t cmd;
	} data;
	robot_id_t id;
};

void RobotSetId(struct Robot *, robot_id_t);
void RobotSetColor(struct Robot *, const uint16_t *);
void RobotSetCmd(struct Robot *, robot_cmd_t);

robot_id_t RobotGetId(const struct Robot *);
const uint16_t *RobotGetColor(const struct Robot *);
robot_cmd_t RobotGetCmd(const struct Robot *);

typedef void (*RobotRcv)(const struct Robot *);

// initialize this robot (robot being controlled) or server
void RobotInit(robot_id_t, RobotRcv);
// convenience functions
void ServerInit(RobotRcv);
void ClientInit(robot_id_t, RobotRcv);

extern struct Robot robot;

void RobotSend();
void RobotBroadcast(const struct Robot *);

void RobotPrint(const struct Robot *);

#endif /* ROBOT_H_ */
