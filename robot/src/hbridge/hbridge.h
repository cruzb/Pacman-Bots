#ifndef HBRIDGE_H
#define HBRIDGE_H

#include <inttypes.h>

typedef struct hbidge_inst {
	volatile uint32_t * hbridge_cmd_reg;
	volatile uint32_t * hbridge_en_reg;
}hbridge_inst_t;

void hbridge_init(hbridge_inst_t* hbridge_inst, volatile uint32_t * const hbridge_base_address);

void hbridge_set_cmd(hbridge_inst_t* hbridge_inst, uint8_t hbridge_cmd);

void hbridge_set_en(hbridge_inst_t* hbridge_inst, uint8_t hbridge_en);

void hbridge_set(hbridge_inst_t* hbridge_inst, uint8_t hbridge_en, uint8_t);

#endif //HBRIDGE_H
