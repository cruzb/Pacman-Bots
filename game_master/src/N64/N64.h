#ifndef N64_H
#define N64_H

#include "drivers/Core16550/core_16550.h"
#include <stdio.h>
#include "CMSIS/a2fxxxm3.h"

typedef struct button {
	const char *name;
	uint32_t mask;
} button_t;

typedef enum button_type_t
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_Z,
	BUTTON_START,
	BUTTON_DU,
	BUTTON_DD,
	BUTTON_DL,
	BUTTON_DR,
	BUTTON_L,
	BUTTON_R,
	BUTTON_CU,
	BUTTON_CD,
	BUTTON_CL,
	BUTTON_CR,
	BUTTON_AX,
	BUTTON_AY,
	NUM_BUTTONS
} button_type_t;

typedef struct N64Inst_t
{
	volatile uint32_t *base_addr;
	uint32_t state;
} N64Inst_t;

typedef enum direction {
	NONE,
	UP,
	DOWN,
	LEFT,
	RIGHT
} direction_t;
#define ANALOG_THRESHOLD 60

void N64Init(N64Inst_t *, volatile uint32_t *addr);
void N64Enable(N64Inst_t *);
void N64Disable(N64Inst_t *);
void N64GetState(N64Inst_t *);
int N64ButtonPressed(N64Inst_t *, button_type_t);
void N64PrintState(N64Inst_t *, int);
direction_t N64ADir(N64Inst_t *inst);
direction_t N64DDir(N64Inst_t *inst);

#endif
