#ifndef CLK_H
#define CLK_H
#include <inttypes.h>
#include <stdlib.h>

enum {
	CLK_FAB,
	CLK_P0,
	CLK_P1,
	CLK_ACE,
	CLK_ADC,
	CLK_COUNT
};

void clk_init(void);

void clk_set_freq_hz(size_t clk_id, uint32_t freq_hz);

uint32_t clk_get_freq_hz(size_t clk_id);

#endif //CLK_H
