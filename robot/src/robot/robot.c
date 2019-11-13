/*
 * robot.c
 *
 *  Created on: Mar 26, 2019
 *      Author: rmallya
 */

#include "robot.h"
#include <string.h>
#include "mss_assert.h"
#include <stdio.h>
#include "../sleep/sleep.h"
#include "../sfled/sfled.h"
#include "../motor/motor.h"
#include "../rgb_sensor/rgb_sensor.h"
#include "../ps/ps.h"

struct Robot robot;
direction_t robot_direction;
motor_inst_t motor_inst_l;
motor_inst_t motor_inst_r;
ps_inst_t ps_inst;

#define SPEED_TURN_L 0.35
#define SPEED_TURN_R 0.35
#define SPEED_FORWARD_L 0.3
#define SPEED_FORWARD_R 0.3
#define SPEED_CORRECTION 0.35

void RobotSetId(struct Robot *r, robot_id_t id)
{
	r->id = id;
}

void RobotSetColor(struct Robot *r)
{
	uint16_t color[3] = {read_red(), read_green(), read_blue()};
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

void RobotInit(robot_id_t id)
{
	memset(&robot, 0, sizeof(robot));
	RobotSetId(&robot, id);
	XType type = id == SERVER ? COORDINATOR : CLIENT;
	XInit(type, id, RobotReceive);

	volatile uint32_t * const MOTOR_L_BASE_ADDRESS = (uint32_t *)0x40050000;
	motor_init(&motor_inst_l, MOTOR_L_BASE_ADDRESS, 2E4);
	//motor_test(&motor_inst_l, 2E6);

	volatile uint32_t * const MOTOR_R_BASE_ADDRESS = (uint32_t *)0x40051000;
	motor_init(&motor_inst_r, MOTOR_R_BASE_ADDRESS, 2E4);
	//motor_test(&motor_inst_r, 2E6);

	char* ps_names[] = {
		"F", "L", "B", "R", "BL", "BR"
	};
	ps_init(&ps_inst, ps_names, 6);

	config_rgb_sensor_default();
	robot.data.cmd = STOP;
	robot_direction = UP;
}

void ServerInit()
{
	RobotInit(SERVER);
}

void ClientInit(robot_id_t id)
{
	RobotInit(id);
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
	ASSERT(robot.id = SERVER);
	struct XData data;
	memset(&data, 0, sizeof(data));
	memcpy(data.data, r, sizeof(robot));
	XSend(&data, r->id);
}

void RobotReceive(const struct XData *data)
{
	struct Robot r;
	memcpy(&r, data->data, sizeof(r));
	robot.data.cmd = r.data.cmd;
}

void RobotPrint(const struct Robot *r)
{
	printf("Id: %d\r\n", r->id);
	const uint16_t *color = RobotGetColor(r);
	printf("Color Seen: %u,%u,%u\r\n", color[0], color[1], color[2]);
	printf("Cmd: %u\r\n", RobotGetCmd(r));
}

void RobotSetDirection(size_t ps_id) {
	switch(ps_id) {
		case PS_L :
			switch(robot_direction) {
				case UP : robot_direction = LEFT;
				case DOWN : robot_direction = RIGHT;
				case RIGHT : robot_direction = UP;
				case LEFT : robot_direction = DOWN;
			}
		case PS_R :
			switch(robot_direction) {
				case UP : robot_direction = RIGHT;
				case DOWN : robot_direction = LEFT;
				case RIGHT : robot_direction = DOWN;
				case LEFT : robot_direction = UP;
			}
		case PS_B :
			switch(robot_direction) {
				case UP : robot_direction = DOWN;
				case DOWN : robot_direction = UP;
				case RIGHT : robot_direction = LEFT;
				case LEFT : robot_direction = RIGHT;
			}
	}
}

direction_t RobotGetDirection() {
	return robot_direction;
}

void RobotCalibrate() {
	motor_set(&motor_inst_l, MOTOR_CMD_COAST, 0);
	motor_set(&motor_inst_r, MOTOR_CMD_COAST, 0);
	robot.data.cmd = STOP;
	ps_calibrate(&ps_inst, 10, ps_aggregate_mean, 0.85);
}
void RobotMotorUpdate() {
	ps_sample(&ps_inst, 2, ps_aggregate_mean);
	size_t ps_id_min = ps_id_get_min(&ps_inst);
	uint16_t ps_min_val = ps_val_get(&ps_inst, ps_id_min);
	if (ps_min_val > ps_val_get_ambient(&ps_inst)) {
		motor_set(&motor_inst_l, MOTOR_CMD_COAST, 0);
		motor_set(&motor_inst_r, MOTOR_CMD_COAST, 0);
		return;
	}
	RobotSetDirection(ps_id_min);
	if(ps_id_min == PS_L) {
		motor_set(&motor_inst_l, MOTOR_CMD_BACKWARD, SPEED_TURN_L);
		motor_set(&motor_inst_r, MOTOR_CMD_FORWARD, SPEED_TURN_R);
		ps_wait_min(&ps_inst, PS_F, 2, ps_aggregate_mean);
		if(robot_direction == UP || robot_direction == DOWN) {
			usleep(270000);

		}
	}
	else if(ps_id_min == PS_R) {
		motor_set(&motor_inst_l, MOTOR_CMD_FORWARD, SPEED_TURN_L);
		motor_set(&motor_inst_r, MOTOR_CMD_BACKWARD, SPEED_TURN_R);
		ps_wait_min(&ps_inst, PS_F, 2, ps_aggregate_mean);
		if(robot_direction == UP || robot_direction == DOWN) {
			usleep(270000);
		}
	}
	else if(ps_id_min == PS_F) {
		motor_set(&motor_inst_l, MOTOR_CMD_FORWARD, SPEED_FORWARD_L);
		motor_set(&motor_inst_r, MOTOR_CMD_FORWARD, SPEED_FORWARD_R);
		usleep(100000);
		if(robot_direction == LEFT || robot_direction == RIGHT) {
			usleep(500000);
		}
	}
	else if(ps_id_min == PS_B) {
		motor_set(&motor_inst_l, MOTOR_CMD_BACKWARD, SPEED_FORWARD_L);
		motor_set(&motor_inst_r, MOTOR_CMD_FORWARD, SPEED_FORWARD_R);
		ps_wait_min(&ps_inst, PS_F, 2, ps_aggregate_mean);
	}
	else if(ps_id_min == PS_BL) {
		motor_set_speed(&motor_inst_r, SPEED_CORRECTION);
	}
	else if(ps_id_min == PS_BR) {
		motor_set_speed(&motor_inst_l, SPEED_CORRECTION);
	}
}

void RobotStop() {
	motor_set(&motor_inst_l, MOTOR_CMD_COAST, 0);
	motor_set(&motor_inst_r, MOTOR_CMD_COAST, 0);
}

void RobotDestroy() {
	ps_destroy(&ps_inst);
}

void RobotRun() {
	ClientInit(PLAYER);
	sfled_init(MSS_GPIO_0);
	sfled_flash(MSS_GPIO_0, 5, 500000);
	while(1){
		robot_cmd_t robot_cmd = robot.data.cmd;
		if(timer_done()) {
			RobotSetColor(&robot);
			RobotSend();
			timer_set(500000, NULL);
		}
		if(robot_cmd == CALIBRATE) {
			RobotCalibrate();
			sfled_flash(MSS_GPIO_0, 2, 500000);
		}
		else if(robot_cmd == START) {
			sfled_on(MSS_GPIO_0);
			RobotMotorUpdate();
		}
		else {
			RobotStop();
		}
	}
	RobotDestroy();
}
