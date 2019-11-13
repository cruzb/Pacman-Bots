#include "hbridge.h"
#include "mss_assert.h"

void hbridge_init(hbridge_inst_t* hbridge_inst, volatile uint32_t * const hbridge_base_address)
{
	hbridge_inst->hbridge_cmd_reg = hbridge_base_address;
	hbridge_inst->hbridge_en_reg = hbridge_base_address + 1;
	hbridge_set(hbridge_inst, 0, 0);
}

void hbridge_set_cmd(hbridge_inst_t* hbridge_inst, uint8_t hbridge_cmd)
{
	ASSERT(hbridge_cmd < 4);
	*(hbridge_inst->hbridge_cmd_reg) = hbridge_cmd;
}

void hbridge_set_en(hbridge_inst_t* hbridge_inst, uint8_t hbridge_en)
{
	ASSERT(hbridge_en < 2);
	*(hbridge_inst->hbridge_en_reg) = hbridge_en;
}

void hbridge_set(hbridge_inst_t* hbridge_inst, uint8_t hbridge_en, uint8_t hbridge_cmd) {
	hbridge_set_cmd(hbridge_inst, hbridge_cmd);
	hbridge_set_en(hbridge_inst, hbridge_en);
}
