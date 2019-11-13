#ifndef COLOR_UTIL_H_
#define COLOR_UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "led.h"
#include "../grid/grid.h"

//use hsv color space then convert to rgb, probably best, accounts for walls
void set_colors_hsv(led_inst_t *led_inst, grid_t* grid_inst, size_t buf);

void set_colors_hsv_range(led_inst_t *led_inst, size_t start, size_t end, size_t buf);

void set_colors_hsv_with_sv(led_inst_t *led_inst, size_t buf, double saturation, double value);

///////these do not yet account for walls/////

//uses 2 channels at a time for greater variation in the color space, probably use this if hsv doesnt work
void set_colors_2channel(led_inst_t *led_inst, int size);

//factor the color space among the three channels
void set_colors_factored(led_inst_t *led_inst);

//set colors to rainbow, bad for unique id
void set_colors_rainbow(led_inst_t* led_inst);

//rgb cube
void set_colors_cube(led_inst_t* led_inst);




//extra utils//
uint8_t are_colors_equal(uint8_t* rgb1, uint8_t* rgb2);

uint8_t is_wall(uint8_t* rgb);


#endif /* COLOR_UTIL_H_ */
