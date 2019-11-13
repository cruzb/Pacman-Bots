#include <inttypes.h>
#ifndef PWM_H
#define PWM_H

typedef struct PWM_inst {
	volatile uint32_t * pwm_period_cc_reg;
	volatile uint32_t * pwm_pulse_width_cc_reg;
	uint32_t PWM_period_cc;
} PWM_inst_t;

void PWM_init(PWM_inst_t* PWM_inst,
		volatile uint32_t * const PWM_base_Address,
		uint32_t PWM_period_freq_hz,
		double PWM_duty_cycle);

void PWM_set_period_cc(PWM_inst_t* PWM_inst, uint32_t PWM_period_cc);

void PWM_set_pulse_width_cc(PWM_inst_t* PWM_inst, double PWM_pulse_width_cc);

void PWM_set_cc(PWM_inst_t* PWM_inst, uint32_t PWM_pulse_width_cc, uint32_t PWM_period_cc);

void PWM_set_period_freq(PWM_inst_t* PWM_inst, uint32_t PWM_period_freq_hz);

void PWM_set_duty_cycle(PWM_inst_t* PWM_inst, double PWM_duty_cycle);

void PWM_set(PWM_inst_t* PWM_inst, uint32_t PWM_period_freq_hz, double PWM_duty_cycle);
#endif //PWM_H
