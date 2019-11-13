#include <inttypes.h>

typedef struct encoder_inst {
	volatile uint32_t* int_count_reg;
	volatile uint32_t* clr_reg;

} encoder_inst_t;

void encoder_init(encoder_inst_t * encoder_inst, volatile uint32_t* const encoder_base_address);

void encoder_set_int(encoder_inst_t * encoder_inst, uint32_t int_count, void(*encoder_int_handler)(void*));

void encoder_disable_int(encoder_inst_t * encoder_inst);

uint32_t encoder_get_count(encoder_inst_t * encoder_inst);

void encoder_clr(encoder_inst_t * encoder_inst);
