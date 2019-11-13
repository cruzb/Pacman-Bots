#ifndef GAME_MASTER_H
#define GAME_MASTER_H

#include "grid/grid.h"
#include "led/led.h"
typedef struct char_color_pair {
	char character;
	color_t color;
} char_color_pair_t;

void GameInit(char** occupancy, char_color_pair_t* char_color_pairs, volatile uint32_t *led_addr, volatile uint32_t *n64_addr);

void GameCalibrate(int init);

void GameReset(int init);

void GameStart(int init);

void GameAdvance(int init);

void GameWon(int init);

void GameLost(int init);

void GameIdle(int init);

// ends and cleans up game
void GameKill(int init);
#endif
