#ifndef LED_H
#define LED_H

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../grid/grid.h"

typedef struct led_inst {
	size_t num_leds;
	size_t num_grid_rows;
	size_t num_grid_cols;
	size_t num_color_bufs;
	uint8_t** color_bufs;
} led_inst_t;

typedef struct color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

#define COLOR_BLACK		(color_t){0, 0, 0}
#define COLOR_RED 		(color_t){255, 0, 0}
#define COLOR_GREEN 	(color_t){0, 255, 0}
#define COLOR_BLUE 		(color_t){0, 0, 255}
#define COLOR_YELLOW 	(color_t){255, 255, 0}
#define COLOR_MAGENTA 	(color_t){255, 0, 255}
#define COLOR_CYAN 		(color_t){0, 255, 255}
#define COLOR_WHITE 	(color_t){255, 255, 255}
#define COLOR_VIOLET 	(color_t){127, 0, 255}

void led_init(led_inst_t* led_inst,
				size_t num_grid_rows,
				size_t num_grid_cols,
				size_t num_color_bufs,
				volatile uint32_t* UART_base_addr);

void led_destroy(led_inst_t* led_inst);

int led_strip_idx_inbounds(led_inst_t* led_inst, size_t strip_idx);

int led_color_buf_idx_inbounds(led_inst_t* led_inst, size_t color_buf_idx);

void led_set_color(led_inst_t* led_inst, size_t strip_idx, size_t color_buffer_idx, color_t color);

void led_set_colors(led_inst_t* led_inst,
					size_t strip_idx_start,
					size_t strip_idx_end,
					size_t color_buffer_idx,
					color_t color);

void led_set_color_grid(led_inst_t* led_inst, cell_t cell, size_t color_buffer_idx, color_t color);

void led_send(led_inst_t* led_inst, size_t color_buffer_idx, uint32_t delay_usecs);

void led_pwm(led_inst_t* led_inst,
		size_t color_buffer_idx_on,
		size_t color_buffer_idx_off,
		uint32_t period,
		double duty_cycle);

void led_swap(led_inst_t *led_inst, size_t strip_idx1, size_t color_buf_idx1, size_t strip_idx2, size_t color_buf_idx2);

void led_swap_range(led_inst_t *led_inst, size_t start1, size_t end1, size_t color_buf_idx1, size_t start2, size_t end2, size_t color_buf_idx2);

void led_rand(led_inst_t *led_inst, size_t start, size_t end, size_t color_buffer_idx);

void led_rand_show(led_inst_t *led_inst, size_t start, size_t end, size_t color_buffer_idx, uint32_t delay_usecs, uint32_t show_len);

void led_test_colors(led_inst_t* led_inst, size_t color_buf_idx, uint32_t delay_usecs);

size_t led_grid_idx_to_strip_idx(led_inst_t* led_inst, size_t grid_row_idx, size_t grid_col_idx);

#endif
