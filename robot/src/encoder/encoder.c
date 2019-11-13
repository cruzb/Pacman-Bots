#include "encoder.h"
void encoder_init(encoder_inst_t * encoder_inst, volatile uint32_t* const encoder_base_address) {
	encoder_inst->int_count_reg = encoder_base_address;
	encoder_inst->clr_reg = encoder_base_address + 1;
}

void encoder_set_int(encoder_inst_t * encoder_inst, uint32_t int_count, void(*encoder_int_handler)(void*)) {
	*(encoder_inst->int_count_reg) = int_count;
}
void encoder_disable_int(encoder_inst_t * encoder_inst) {
	*(encoder_inst->int_count_reg) = 0;
}

uint32_t encoder_get_count(encoder_inst_t * encoder_inst) {
	return *(encoder_inst->int_count_reg);
}

void encoder_clr(encoder_inst_t * encoder_inst) {
	*(encoder_inst->clr_reg) = 1;
}
