#include "clk.h"
#include "mss_assert.h"

uint32_t clks_freq_hz[CLK_COUNT];
int clks_set[CLK_COUNT];

void clk_init(void) {
	size_t i = 0;
	for(; i < CLK_COUNT; ++i) {
		clks_freq_hz[i] = 0;
		clks_set[i] = 0;
	}
}

void clk_set_freq_hz(size_t clk_id, uint32_t freq_hz) {
	clks_freq_hz[clk_id] = freq_hz;
	clks_set[clk_id] = 1;
}

uint32_t clk_get_freq_hz(size_t clk_id) {
	ASSERT(clks_set[clk_id]);
	return clks_freq_hz[clk_id];
}
