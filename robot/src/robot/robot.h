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

typedef enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
} direction_t;

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

void RobotSetColor(struct Robot *);

void RobotSetCmd(struct Robot *, robot_cmd_t);

robot_id_t RobotGetId(const struct Robot *);

const uint16_t *RobotGetColor(const struct Robot *);

robot_cmd_t RobotGetCmd(const struct Robot *);


// initialize this robot (robot being controlled) or server
void RobotInit(robot_id_t);

// convenience functions
void ServerInit();

void ClientInit(robot_id_t);

extern struct Robot robot;

void RobotSend();

void RobotBroadcast(const struct Robot *);

void RobotReceive(const struct XData *data);

void RobotPrint(const struct Robot *);

void RobotSetDirection(size_t ps_id);

direction_t RobotGetDirection();

void RobotCalibrate();

void RobotMotorUpdate();

void RobotStop();

void RobotDestroy();

void RobotRun();

#endif /* ROBOT_H_ */
