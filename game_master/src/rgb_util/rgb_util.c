/*
 * rgb_util.c
 *
 *  Created on: Apr 21, 2019
 *      Author: cruzb
 */
#include "rgb_util.h"

uint16_t powerup_color[3] = {0, 200*TO16BIT, 200*TO16BIT};
uint16_t cherry_color[3] = {255*TO16BIT, 0, 0};
uint16_t dot_color[3] = {255*TO16BIT, 204*TO16BIT, 0};

uint8_t min_index(uint16_t rgb[3]) {
	uint16_t min = 0;
	if(rgb[0] < rgb[1])
		min = 0;
	else min = 1;
	if(rgb[2] < rgb[min])
		min = 2;
	return min;
}


uint8_t is_valid_color(uint16_t first[3]) {
	uint32_t light = total_light(first);
	if(light < TOTAL_LIGHT_EQUIVALENCE_THRESHOLD)
		return 0;
	return 1;
}

uint16_t subtract_without_underflow(uint16_t a, uint16_t b) {
	if(a < b)
		a = 0;
	else a = a - b;
	return a;
}

uint8_t is_cherry(uint16_t reading[3]) {
	//inr diff = abs(reading[0] - 150000);
	if(reading[0] < CHERRY_RED_CHANNEL)
		return 0;
	if((int)reading[0] - (int)reading[1] > CHERRY_DIFF_FROM_RED && (int)reading[0] - (int)reading[2] > CHERRY_DIFF_FROM_RED)
		return 1;
	return 0;
}

uint8_t is_powerup(uint16_t reading[3]) {
	uint16_t diff = abs((int)total_light(powerup_color) - (int)total_light(reading));
	if(diff > TOTAL_LIGHT_EQUIVALENCE_THRESHOLD)
		return 0;
	int i = 0;
	for(; i < 3; i++) {
		diff = abs((int)powerup_color[i] - (int)reading[i]);
		if(diff > CHANNEL_EQUIVALENCE_THRESHOLD)
			return 0;
	}
	return 1;
}

uint8_t is_same_color(uint16_t base[3], uint16_t reading[3]) {
	//correct for bad color readings
	int i = 0;
	for(; i < 3; i++) {
		if(reading[i] < COLOR_CORRECTION_LVL1_THRESHOLD)
			reading[i] = subtract_without_underflow(reading[i], COLOR_CORRECTION_LVL1_AMOUNT);
		else if(reading[i] < COLOR_CORRECTION_LVL2_THRESHOLD)
			reading[i] = subtract_without_underflow(reading[i], COLOR_CORRECTION_LVL2_AMOUNT);
	}


	uint16_t diff = abs(total_light(base) - total_light(reading));
	if(diff > TOTAL_LIGHT_EQUIVALENCE_THRESHOLD)
		return 0;
	i = 0;
	for(; i < 3; i++) {
		diff = abs(base[i] - reading[i]);
		if(diff > CHANNEL_EQUIVALENCE_THRESHOLD)
			return 0;
	}
	return 1;
}

uint32_t total_light(uint16_t color[3]) {
	uint32_t sum = 0;
	int i = 0;
	for(; i < 3; i++)
		sum += color[i];
	return sum;
}


double max(double a, double b, double c) {
	if(a > b)
		b = a;
	if(b > c)
		c = b;
	return c;
}

double min(double a, double  b, double c) {
	if(a < b)
		b = a;
	if(b < c)
		c = b;
	return c;
}

//red green and blue are between 0 and 1, hsv returned between 0 and 1
void rgb_to_hsv(double red, double green, double blue, double hsv[3]) {
	double Cmax = max(red, green, blue);
	double Cmin = min(red, green, blue);
	double delta = Cmax - Cmin;
	double hue;
	double saturation;
	double value = Cmax;

	if(Cmax != 0)
		saturation = delta / Cmax;
	else saturation = 0;

	if(delta == 0)
		hue = 0;
	else if(Cmax == red) {
		hue = 60 * ((green - blue) / delta);
	}
	else if(Cmax == green) {
		hue = 60 * (((blue - red) / delta) + 2);
	}
	else hue = 60 * (((red - green) / delta) + 4);

	if(hue < 0)
		hue += 360;

	hue /= 360;

	hsv[0] = hue;
	hsv[1] = saturation;
	hsv[2] = value;
}
