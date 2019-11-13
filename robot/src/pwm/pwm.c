#include "pwm.h"
#include "mss_assert.h"
#include "../clk/clk.h"

void PWM_init(PWM_inst_t* PWM_inst,
		volatile uint32_t * const PWM_base_address,
		uint32_t PWM_period_freq_hz,
		double PWM_duty_cycle)
{
	PWM_inst->pwm_period_cc_reg = PWM_base_address;
	PWM_inst->pwm_pulse_width_cc_reg = PWM_base_address + 1;
	PWM_set(PWM_inst, PWM_period_freq_hz, PWM_duty_cycle);
}

void PWM_set_period_cc(PWM_inst_t* PWM_inst, uint32_t PWM_period_cc)
{
	*(PWM_inst->pwm_period_cc_reg) = PWM_period_cc;
}

void PWM_set_pulse_width_cc(PWM_inst_t* PWM_inst, double PWM_pulse_width_cc)
{
	*(PWM_inst->pwm_pulse_width_cc_reg) = PWM_pulse_width_cc;
}

uint32_t PWM_get_period_cc(PWM_inst_t* PWM_inst)
{
	return *(PWM_inst->pwm_period_cc_reg);
}

uint32_t PWM_get_pulse_width_cc(PWM_inst_t* PWM_inst)
{
	return *(PWM_inst->pwm_pulse_width_cc_reg);
}

void PWM_set_cc(PWM_inst_t* PWM_inst, uint32_t PWM_pulse_width_cc, uint32_t PWM_period_cc)
{
	PWM_set_period_cc(PWM_inst, PWM_period_cc);
	PWM_set_pulse_width_cc(PWM_inst, PWM_pulse_width_cc);
}

void PWM_set_period_freq(PWM_inst_t* PWM_inst, uint32_t PWM_period_freq_hz) {
	uint32_t PWM_clock_freq_hz = clk_get_freq_hz(CLK_FAB);
	ASSERT(PWM_period_freq_hz < PWM_clock_freq_hz);
	uint32_t PWM_period_cc = (uint32_t)((double)PWM_clock_freq_hz / (double)PWM_period_freq_hz);
	PWM_set_period_cc(PWM_inst, PWM_period_cc);
}

void PWM_set_duty_cycle(PWM_inst_t* PWM_inst, double PWM_duty_cycle) {
	ASSERT(PWM_duty_cycle <= 1.0);
	uint32_t PWM_period_cc = PWM_get_period_cc(PWM_inst);
	uint32_t PWM_pulse_width_cc = (uint32_t)(PWM_duty_cycle * (double)PWM_period_cc);
	PWM_set_pulse_width_cc(PWM_inst, PWM_pulse_width_cc);
}

void PWM_set(PWM_inst_t* PWM_inst, uint32_t PWM_period_freq_hz, double PWM_duty_cycle) {
	PWM_set_period_freq(PWM_inst, PWM_period_freq_hz);
	PWM_set_duty_cycle(PWM_inst, PWM_duty_cycle);
}
