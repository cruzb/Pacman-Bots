Generic library for setting and retrieving global clockspeeds. Implemented because the provided MSS functions to do this provide incorrect values
This library is used in conjunction with sleep.h to allow the user to call sleep functions for the specified number of microseconds or nanoseconds
without explicitly specifying the clock period.

Initialize all clocks to 0 using clk_init.

Set clock speed usinng clk_set_freq_hz.

Retrieve clock speed using clk_get_freq_hz(size_t clk_id). This function will cause an assertion failure if it is called before clock_set_freq_hz.