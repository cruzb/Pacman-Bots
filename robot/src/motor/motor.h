#include <inttypes.h>
#include "../pwm/pwm.h"
#include "../hbridge/hbridge.h"
//#include "../encoder/encoder.h"

#ifndef MOTOR_H
#define MOTOR_H

typedef enum motor_cmd {
	MOTOR_CMD_COAST,
	MOTOR_CMD_FORWARD,
	MOTOR_CMD_BACKWARD,
	MOTOR_CMD_BRAKE,
} motor_cmd_t;

typedef struct motor_inst {
	PWM_inst_t PWM_inst;
	hbridge_inst_t hbridge_inst;
	//encoder_inst_t encoder_inst;
	double speed;
} motor_inst_t;

void motor_init(motor_inst_t* motor_inst,
				volatile uint32_t * const motor_base_addr,
				uint32_t PWM_Period_hz);

void motor_set_cmd(motor_inst_t* motor_inst, motor_cmd_t motor_cmd);

double motor_clamp_speed(double speed);

void motor_set_speed(motor_inst_t* motor_inst, double speed);

double motor_get_speed(motor_inst_t* motor_inst);

void motor_set(motor_inst_t* motor_inst, motor_cmd_t motor_cmd, double speed);

void motor_test(motor_inst_t* motor_inst, uint32_t time_usecs);

#endif //MOTOR_H
