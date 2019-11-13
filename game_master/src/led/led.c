#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "led.h"
#include "color_util.h"
#include "led_consts.h"
#include "../sleep/sleep.h"
#include "../clk/clk.h"
#include "drivers/CoreUARTapb/core_uart_apb.h"
#include "CMSIS/mss_assert.h"

#define UART_BAUD_RATE	3000000
UART_instance_t uart_inst;

void led_init(led_inst_t* led_inst,
				size_t num_grid_rows,
				size_t num_grid_cols,
				size_t num_color_bufs,
				volatile uint32_t* UART_base_addr)
{
	ASSERT(((uint32_t)clk_get_freq_hz(CLK_FAB) % (UART_BAUD_RATE * 16)) == 0); //Ensure no precision loss in calculating baud value

	uint16_t baud_value = (clk_get_freq_hz(CLK_FAB) /(UART_BAUD_RATE * 16)) - 1;
	UART_init(&uart_inst, (unsigned int)UART_base_addr, baud_value, DATA_8_BITS | NO_PARITY);

	led_inst->num_leds = num_grid_rows * num_grid_cols;
	led_inst->num_grid_rows = num_grid_rows;
	led_inst->num_grid_cols = num_grid_cols;
	led_inst->num_color_bufs = num_color_bufs;

	int i = 0;
	led_inst->color_bufs = (uint8_t**)malloc(num_color_bufs * sizeof(uint8_t*));
	for(; i < num_color_bufs; ++i) {
		led_inst->color_bufs[i] = (uint8_t*)malloc(led_inst->num_leds * 12);
		memset(led_inst->color_bufs[i], 0, led_inst->num_leds * 12);
	}
	led_send(led_inst, 0, 0);
}

void led_destroy(led_inst_t* led_inst)
{
	size_t color_buf_idx = 0;
	for(; color_buf_idx < led_inst->num_color_bufs; ++color_buf_idx)
	{
		free(led_inst->color_bufs[color_buf_idx]);
	}
	free(led_inst->color_bufs);
}

int led_color_buf_idx_inbounds(led_inst_t* led_inst, size_t color_buf_idx)
{
	return (color_buf_idx < led_inst->num_color_bufs);
}

int led_strip_idx_inbounds(led_inst_t* led_inst, size_t strip_idx)
{
	return (strip_idx <= led_inst->num_leds);
}

uint8_t *get_start(led_inst_t *led_inst, size_t strip_idx, size_t color_buffer_idx)
{
	return led_inst->color_bufs[color_buffer_idx] + (12 * strip_idx);
}

void led_set_color(led_inst_t* led_inst, size_t strip_idx, size_t color_buffer_idx, color_t color)
{
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buffer_idx));
	ASSERT(led_strip_idx_inbounds(led_inst, strip_idx));
	uint8_t *start = get_start(led_inst, strip_idx, color_buffer_idx);
	memcpy(start, byte_to_word[color.b], 4);
	memcpy(start + 4, byte_to_word[color.r], 4);
	memcpy(start + 8, byte_to_word[color.g], 4);
}

void led_set_colors(led_inst_t* led_inst,
					size_t strip_idx_start,
					size_t strip_idx_end,
					size_t color_buffer_idx,
					color_t color)
{
	size_t strip_idx = strip_idx_start;
	for(;strip_idx < strip_idx_end; ++strip_idx) {
		led_set_color(led_inst, strip_idx, color_buffer_idx, color);
	}
}

void led_send(led_inst_t* led_inst, size_t color_buffer_idx, uint32_t delay_usecs){
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buffer_idx));
	UART_send(&uart_inst, (const uint8_t *)led_inst->color_bufs[color_buffer_idx], (12*led_inst->num_leds));
	usleep(delay_usecs);
}

void led_pwm(led_inst_t* led_inst,
		size_t color_buffer_idx_on,
		size_t color_buffer_idx_off,
		uint32_t period,
		double duty_cycle) {
	uint32_t on_delay = (uint32_t)(period * duty_cycle);
	uint32_t off_delay = period - on_delay;
	led_send(led_inst, color_buffer_idx_on, on_delay);
	led_send(led_inst, color_buffer_idx_off, off_delay);
}

void led_swap(led_inst_t *led_inst, size_t strip_idx1, size_t color_buf_idx1, size_t strip_idx2, size_t color_buf_idx2)
{
	/*ASSERT(led_color_buf_idx_inbounds(led_inst, color_buf_idx1));
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buf_idx2));
	ASSERT(led_strip_idx_inbounds(led_inst, strip_idx1));
	ASSERT(led_strip_idx_inbounds(led_inst, strip_idx2));*/
	uint32_t idxColor1[3];
	uint32_t idxColor2[3];
	uint8_t *start1 = get_start(led_inst, strip_idx1, color_buf_idx1);
	uint8_t *start2 = get_start(led_inst, strip_idx2, color_buf_idx2);
	memcpy(idxColor1 + 2, start1, 4);
	memcpy(idxColor1, start1 + 4, 4);
	memcpy(idxColor1 + 1, start1 + 8, 4);
	memcpy(idxColor2 + 2, start2, 4);
	memcpy(idxColor2, start2 + 4, 4);
	memcpy(idxColor2 + 1, start2 + 8, 4);
	memcpy(start1, idxColor2 + 2, 4);
	memcpy(start1 + 4, idxColor2, 4);
	memcpy(start1 + 8, idxColor2 + 1, 4);
	memcpy(start2, idxColor1 + 2, 4);
	memcpy(start2 + 4, idxColor1, 4);
	memcpy(start2 + 8, idxColor1 + 1, 4);
}

void led_swap_range(led_inst_t *led_inst, size_t start1, size_t end1, size_t color_buf_idx1, size_t start2, size_t end2, size_t color_buf_idx2)
{
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buf_idx1));
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buf_idx2));
	ASSERT(led_strip_idx_inbounds(led_inst, start1));
	ASSERT(led_strip_idx_inbounds(led_inst, start2));
	ASSERT(end1 > start1 && end2 > start2 && end1 - start1 == end2 - start2);
	for (; start1 < end1; start1++, start2++)
		led_swap(led_inst, start1, color_buf_idx1, start2, color_buf_idx2);
}

void led_rand(led_inst_t *led_inst, size_t start, size_t end, size_t color_buffer_idx)
{
	ASSERT(led_color_buf_idx_inbounds(led_inst, color_buffer_idx));
	ASSERT(led_strip_idx_inbounds(led_inst, start));
	ASSERT(end > start);
	for (; start < end; start++)
	{
		color_t rand_color;
		rand_color.r = ((uint32_t)rand()) % 255 + 1;
		rand_color.g = ((uint32_t)rand()) % 255 + 1;
		rand_color.b = ((uint32_t)rand()) % 255 + 1;
		led_set_color(led_inst, start, color_buffer_idx, rand_color);
	}
}

void led_rand_show(led_inst_t *led_inst, size_t start, size_t end, size_t color_buffer_idx, uint32_t delay_usecs, uint32_t show_len)
{
	timer_set(show_len, NULL);
	while(!timer_done())
	{
		led_rand(led_inst, start, end, color_buffer_idx);
		led_send(led_inst, color_buffer_idx, delay_usecs);
	}
}

size_t led_grid_idx_to_strip_idx(led_inst_t* led_inst, size_t grid_row_idx, size_t grid_col_idx) {
	//return (row * led_inst->num_grid_cols) + col;  //use this if indexing starts from left on each line

	//use this if lines are zig zagged
	if(grid_row_idx % 2 == 0) {
		return (grid_row_idx * led_inst->num_grid_cols) + grid_col_idx;
	}
	else {
		return (grid_row_idx * led_inst->num_grid_cols) + led_inst->num_grid_cols - grid_col_idx - 1;
	}
}

void led_set_color_grid(led_inst_t* led_inst, cell_t cell, size_t color_buffer_idx, color_t color) {
	led_set_color(led_inst, led_grid_idx_to_strip_idx(led_inst, cell.row, cell.col), color_buffer_idx, color);
}

void led_test_colors(led_inst_t* led_inst, size_t color_buf_idx, uint32_t delay_usecs) {
	color_t COLORS[9] = {
			COLOR_BLACK,
			COLOR_RED,
			COLOR_GREEN,
			COLOR_BLUE,
			COLOR_YELLOW,
			COLOR_MAGENTA,
			COLOR_CYAN,
			COLOR_WHITE,
			COLOR_VIOLET
	};
	size_t i = 0;
	for(;i < 9; ++i) {
		led_set_colors(led_inst, 0, led_inst->num_leds, color_buf_idx, COLORS[i]);
		led_send(led_inst, color_buf_idx, delay_usecs);
	}
}
