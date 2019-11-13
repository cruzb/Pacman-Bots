#ifndef RGB_UTIL_H_
#define RGB_UTIL_H_

#include <inttypes.h>
#include <stdlib.h>


#define TOTAL_LIGHT_VALIDITY_THRESHOLD 12000 //throw away comparison if not bright enough
#define TOTAL_LIGHT_EQUIVALENCE_THRESHOLD 12000 //for colors to be equal their total light must be this close
#define CHANNEL_EQUIVALENCE_THRESHOLD 9000 //for colors to be equal each channel must be this close
#define CHERRY_RED_CHANNEL 40000
#define CHERRY_DIFF_FROM_RED 35000

#define COLOR_CORRECTION_LVL2_THRESHOLD 12000 //following constants are an attempt to remove color noise for is_same_color
#define COLOR_CORRECTION_LVL2_AMOUNT 5000
#define COLOR_CORRECTION_LVL1_THRESHOLD 6000
#define COLOR_CORRECTION_LVL1_AMOUNT 3500
#define COLOR_CORRECTION_BASE_THRESHOLD 40000

#define TO16BIT 257

//is a red cherry, based on red color (255,0,0)
uint8_t is_cherry(uint16_t reading[3]);

//is a cyan powerup, based on cyan color (0,255,255)
uint8_t is_powerup(uint16_t reading[3]);

//check if reading is bright enough for consideration
//dim readings can be thrown out and called invalid
uint8_t is_valid_color(uint16_t first[3]);

//check if two colors are the same, rgb sensor readings are not good enough compared to
//actual values for a generic function like this
uint8_t is_same_color(uint16_t first[3], uint16_t second[3]);

//rgb to hsv conversion for debug
void rgb_to_hsv(double red, double green, double blue, double hsv[3]);

//add r,g,b channels to get total brightness
uint32_t total_light(uint16_t color[3]);

#endif /* RGB_UTIL_H_ */
