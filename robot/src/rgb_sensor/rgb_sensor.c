/*
 * rgb_sensor.c
 *
 *  Created on: Apr 16, 2019
 *      Author: cruzb
 */
#include <inttypes.h>
#include "drivers/mss_i2c/mss_i2c.h"
#include "rgb_sensor.h"


uint8_t config_rgb_sensor_default() {
	uint8_t cfg1 = CFG1_MODE_RGB | CFG1_10KLUX;
	uint8_t cfg2 = CFG2_IR_ADJUST_HIGH;
	uint8_t cfg3 = CFG_DEFAULT;

	return config_rgb_sensor(cfg1, cfg2, cfg3);
}

uint8_t config_rgb_sensor(uint8_t cfg1, uint8_t cfg2, uint8_t cfg3) {
	MSS_I2C_init(&g_mss_i2c1, TARGET_ADDRESS, MSS_I2C_PCLK_DIV_960);
	uint8_t id;
	read_8bit_i2c(DEVICE_ID, &id);
	if(id != 0x7D) return 0;
	id = reset_rgb_sensor();
	if(id == 0) return 0;
	write_8bit_i2c(CONFIG_REG_1, cfg1);
	write_8bit_i2c(CONFIG_REG_2, cfg2);
	write_8bit_i2c(CONFIG_REG_3, cfg3);

	uint8_t data = 0x00;
	read_8bit_i2c(CONFIG_REG_1, &data);
	if(data != cfg1) return 0;
	read_8bit_i2c(CONFIG_REG_2, &data);
	if(data != cfg2) return 0;
	read_8bit_i2c(CONFIG_REG_3, &data);
	if(data != cfg3) return 0;
	return 1;
}

void read_8bit_i2c(uint8_t addr, uint8_t* data) {
	/*MSS_I2C_write(
					&g_mss_i2c1,
					TARGET_ADDRESS,
					&addr,
					1,
					MSS_I2C_RELEASE_BUS
				);
	MSS_I2C_read(
				&g_mss_i2c1,
				TARGET_ADDRESS,
				data,
				1,
				MSS_I2C_RELEASE_BUS
			);*/
	MSS_I2C_write_read(
					&g_mss_i2c1,
					TARGET_ADDRESS,
					&addr,
					1,
					data,
					1,
					MSS_I2C_RELEASE_BUS
				);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	MSS_I2C_write(
						&g_mss_i2c1,
						TARGET_ADDRESS,
						&addr,
						0,
						MSS_I2C_RELEASE_BUS
					);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
}

uint16_t read_16bit_i2c(uint8_t addr) {
	uint8_t data[2] = {0,0};
	/*MSS_I2C_write_read(
					&g_mss_i2c1,
					TARGET_ADDRESS,
					&addr,
					1,
					data,
					2,
					MSS_I2C_RELEASE_BUS
				);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	MSS_I2C_write(
							&g_mss_i2c1,
							TARGET_ADDRESS,
							&addr,
							0,
							MSS_I2C_RELEASE_BUS
						);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);*/

	MSS_I2C_write(
				&g_mss_i2c1,
				TARGET_ADDRESS,
				&addr,
				1,
				MSS_I2C_RELEASE_BUS
			);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	MSS_I2C_read(
				&g_mss_i2c1,
				TARGET_ADDRESS,
				data,
				2,
				MSS_I2C_RELEASE_BUS
			);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	MSS_I2C_write(
				&g_mss_i2c1,
				TARGET_ADDRESS,
				&addr,
				0,
				MSS_I2C_RELEASE_BUS
			);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
	return (data[1] << 8) + data[0];
	//return (uint16_t)((data[0] << 8) | (data[1] & 0xff));
}


void write_8bit_i2c(uint8_t addr, uint8_t data) {
	uint8_t tx[2] = {addr, data};
	MSS_I2C_write(
				&g_mss_i2c1,
				TARGET_ADDRESS,
				tx,
				2,
				MSS_I2C_RELEASE_BUS
			);
	MSS_I2C_wait_complete( &g_mss_i2c1, MSS_I2C_NO_TIMEOUT);
}

uint8_t reset_rgb_sensor() {
	uint8_t data = 0x00;
	uint8_t buff = 0x00;
	write_8bit_i2c(DEVICE_ID, 0x46);
	read_8bit_i2c(CONFIG_REG_1, &buff);
	data = data | buff;
	read_8bit_i2c(CONFIG_REG_2, &buff);
	data = data | buff;
	read_8bit_i2c(CONFIG_REG_3, &buff);
	data = data | buff;
	read_8bit_i2c(STATUS_REGISTER, &buff);
	data = data | buff;
	if(data != 0x00) return 0;
	return 1;
}

uint16_t read_color(uint8_t addr, uint8_t addr2) {
	uint16_t value = 0x0000;
	uint8_t low = 0x00;
	uint8_t high = 0x00;
	read_8bit_i2c(addr, &low);
	read_8bit_i2c(addr2, &high);
	value = (high << 8) | (low & 0xff);
	return value;
}

uint16_t read_red() {
	return read_16bit_i2c(RED_L);
	//return red_2byte;
	//uint16_t red_2byte = read_color(RED_H, RED_L);
	//return read_16bit_i2c(RED_L);
}

uint16_t read_green() {
	//return read_color(GREEN_H, GREEN_L);
	return read_16bit_i2c(GREEN_L);
}

uint16_t read_blue() {
	//return read_color(BLUE_H, BLUE_L);
	return read_16bit_i2c(BLUE_L);
}

uint8_t read_status() {
	uint8_t data = 0x00;
	read_8bit_i2c(STATUS_REGISTER, &data);
	return data;
}
