/*
 * rgb_sensor.h
 *
 *  Created on: Apr 16, 2019
 *      Author: cruzb
 */

#ifndef RGB_SENSOR_H_
#define RGB_SENSOR_H_

#include <inttypes.h>

// ISL29125 I2C Address
#define TARGET_ADDRESS 0x44

// ISL29125 Registers
#define DEVICE_ID 0x00
#define CONFIG_REG_1 0x01
#define CONFIG_REG_2 0x02
#define CONFIG_REG_3 0x03
#define THRESHOLD_LL 0x04
#define THRESHOLD_LH 0x05
#define THRESHOLD_HL 0x06
#define THRESHOLD_HH 0x07
#define STATUS_REGISTER 0x08
#define GREEN_L 0x09
#define GREEN_H 0x0A
#define RED_L 0x0B
#define RED_H 0x0C
#define BLUE_L 0x0D
#define BLUE_H 0x0E

// Configuration Settings
#define CFG_DEFAULT 0x00

// CONFIG1
// Pick a mode, determines what color[s] the sensor samples, if any
#define CFG1_MODE_POWERDOWN 0x00
#define CFG1_MODE_G 0x01
#define CFG1_MODE_R 0x02
#define CFG1_MODE_B 0x03
#define CFG1_MODE_STANDBY 0x04
#define CFG1_MODE_RGB 0x05
#define CFG1_MODE_RG 0x06
#define CFG1_MODE_GB 0x07

// Light intensity range
// In a dark environment 375Lux is best, otherwise 10KLux is likely the best option
#define CFG1_375LUX 0x00
#define CFG1_10KLUX 0x08

// Change this to 12 bit if you want less accuracy, but faster sensor reads
// At default 16 bit, each sensor sample for a given color is about ~100ms
#define CFG1_16BIT 0x00
#define CFG1_12BIT 0x10

// Unless you want the interrupt pin to be an input that triggers sensor sampling, leave this on normal
#define CFG1_ADC_SYNC_NORMAL 0x00
#define CFG1_ADC_SYNC_TO_INT 0x20

// CONFIG2
// Selects upper or lower range of IR filtering
#define CFG2_IR_OFFSET_OFF 0x00
#define CFG2_IR_OFFSET_ON 0x80

// Sets amount of IR filtering, can use these presets or any value between 0x00 and 0x3F
// Consult datasheet for detailed IR filtering calibration
#define CFG2_IR_ADJUST_LOW 0x00
#define CFG2_IR_ADJUST_MID 0x20
#define CFG2_IR_ADJUST_HIGH 0x3F

// CONFIG3
// No interrupts, or interrupts based on a selected color
#define CFG3_NO_INT 0x00
#define CFG3_G_INT 0x01
#define CFG3_R_INT 0x02
#define CFG3_B_INT 0x03


//default setup
uint8_t config_rgb_sensor_default();

//generic config set, and readback for confirmation
uint8_t config_rgb_sensor(uint8_t cfg1, uint8_t cfg2, uint8_t cfg3);

//utility function, read 8bit value
void read_8bit_i2c(uint8_t addr, uint8_t* data);

//utility function, write 8bit value
void write_8bit_i2c(uint8_t addr, uint8_t data);

//utility function, read 16bit value
uint16_t read_16bit_i2c(uint8_t addr);

//reset to factory default
uint8_t reset_rgb_sensor();

//directly read registers from sensor
uint16_t read_red();
uint16_t read_green();
uint16_t read_blue();
uint8_t read_status();


#endif /* RGB_SENSOR_H_ */
