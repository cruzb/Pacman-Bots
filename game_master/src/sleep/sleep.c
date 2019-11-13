#include "sleep.h"
#include "mss_timer/mss_timer.h"
#include "../clk/clk.h"
#include <stdlib.h>

int timer_complete;
timer_int_t user_timer_int;

uint32_t usecs_to_cycles(uint32_t usecs) {
	return (uint32_t)((double)usecs * ((double)clk_get_freq_hz(CLK_P0) / (double)1E6));
}

uint32_t nsecs_to_cycles(uint32_t nsecs) {
	return (uint32_t)((double)nsecs * ((double)clk_get_freq_hz(CLK_P0) / (double)1E9));
}

uint32_t timer_get() {
	return MSS_TIM2_get_current_value();
}

int timer_done() {
	return !MSS_TIM2_get_current_value();
}

void timer_set(uint32_t usecs, timer_int_t timer_int)
{
	if (usecs == 0)
		return;
	user_timer_int = timer_int;
	MSS_TIM2_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM2_enable_irq();
	MSS_TIM2_load_immediate(usecs_to_cycles(usecs));
	MSS_TIM2_start();
}

void usleep(uint32_t usecs) {
	if(usecs == 0) {
		return;
	}
	MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM1_enable_irq();
	MSS_TIM1_load_immediate(usecs_to_cycles(usecs));
	timer_complete = 0;
	MSS_TIM1_start();
	while(!timer_complete) {}
}

void nsleep(uint32_t nsecs) {
	if(nsecs == 0) {
		return;
	}
	MSS_TIM1_init(MSS_TIMER_ONE_SHOT_MODE);
	MSS_TIM1_enable_irq();
	MSS_TIM1_load_immediate(nsecs_to_cycles(nsecs));
	timer_complete = 0;
	MSS_TIM1_start();
	while(!timer_complete) {}
}

void Timer1_IRQHandler(void) {
	timer_complete = 1;
	MSS_TIM1_disable_irq();
}

void Timer2_IRQHandler(void) {
	MSS_TIM2_disable_irq();
	if (user_timer_int) {
		user_timer_int();
	}
	user_timer_int = NULL;
	MSS_TIM2_disable_irq();
}
