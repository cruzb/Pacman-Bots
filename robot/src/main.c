#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "robot/robot.h"
#include "clk/clk.h"

int main()
{
	clk_init();
	clk_set_freq_hz(CLK_FAB, 1E8);
	clk_set_freq_hz(CLK_ADC, 833E4);
	clk_set_freq_hz(CLK_P0, 1E8);
	RobotRun();
	return 0;
}
