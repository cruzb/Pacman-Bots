#include "motor.h"
#include "../sleep/sleep.h"

void motor_init(motor_inst_t* motor_inst,
		volatile uint32_t * const motor_base_addr,
		uint32_t PWM_Period_hz)
{

	volatile uint32_t * const hbridge_base_addr = motor_base_addr;
	volatile uint32_t * const PWM_base_addr = motor_base_addr + 2;

	//volatile uint32_t * const encoder_base_addr = (uint32_t *)(motor_base_addr + 2);

	hbridge_init(&(motor_inst->hbridge_inst), hbridge_base_addr);
	PWM_init(&(motor_inst->PWM_inst),
				PWM_base_addr,
				PWM_Period_hz,
				0.0);

	//encoder_init(&encoder_inst, encoder_base_addr);

	motor_set(motor_inst, MOTOR_CMD_COAST, 0);
}

// void encoder_int_handler(void* ptr) {
// 	motor_inst_t* motor_inst = (motor_inst_t*)ptr;
// 	motor_set_cmd(motor_inst, MOTOR_CMD_COAST);
// }

void motor_set_cmd(motor_inst_t* motor_inst, motor_cmd_t motor_cmd)
{
	uint8_t motor_en = (motor_cmd != MOTOR_CMD_COAST);
	hbridge_set(&(motor_inst->hbridge_inst), motor_en, motor_cmd);
}

double motor_clamp_speed(double speed)
{
	if(speed > 1.0) {
		return 1.0;
	}
	if(speed < 0.0) {
		return 0.0;
	}
	return speed;
}

void motor_set_speed(motor_inst_t* motor_inst, double speed)
{
	double clamped_speed = motor_clamp_speed(speed);
	motor_inst->speed = clamped_speed;
	double PWM_duty_cycle = (clamped_speed * 0.5) + 0.5;
	PWM_set_duty_cycle(&(motor_inst->PWM_inst), PWM_duty_cycle);
}

double motor_get_speed(motor_inst_t* motor_inst)
{
	return motor_inst->speed;
}

void motor_set(motor_inst_t* motor_inst, motor_cmd_t motor_cmd, double speed) {
	motor_set_cmd(motor_inst, motor_cmd);
	motor_set_speed(motor_inst, speed);
}

void motor_test(motor_inst_t* motor_inst, uint32_t time_usecs) {
	uint8_t motor_cmd = 0;
	for(; motor_cmd < 3; ++motor_cmd) {
		motor_set(motor_inst, motor_cmd, 0.5);
		usleep(time_usecs);
		motor_set_cmd(motor_inst, MOTOR_CMD_COAST);
		usleep(5E5);
	}
}
