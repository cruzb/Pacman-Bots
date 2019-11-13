#ifndef PHOTOSENSOR_H
#define PHOTOSENSOR_H
#include "drivers/mss_ace/mss_ace.h"

typedef struct ps_inst {
	size_t 		ps_count;
	uint16_t* 	ps_vals;

	uint16_t* 	ps_offsets;
	uint16_t	ps_offset_max;
	uint16_t 	ps_val_ambient;

	size_t* 	ps_rank;

	uint16_t** 	ps_samples;
	size_t 		ps_sample_count;

	ace_channel_handle_t* ace_handlers;
} ps_inst_t;

typedef void (*ps_aggregate_t)(ps_inst_t*);

enum {
	PS_F,
	PS_L,
	PS_B,
	PS_R,
	PS_BL,
	PS_BR
};

void ps_init(ps_inst_t* ps_inst,  char** ps_names, size_t ps_count);

void ps_destroy(ps_inst_t* ps_inst);

int ps_id_inbounds(ps_inst_t* ps_inst, size_t ps_id);

size_t ps_id_get_min(ps_inst_t* ps_inst);

void ps_aggregate_mean(ps_inst_t* ps_inst);

void ps_aggregate_median(ps_inst_t* ps_inst);

void ps_sample_init(ps_inst_t* ps_inst, uint32_t ps_sample_count);

void ps_sample_destroy(ps_inst_t* ps_inst);

void ps_sample(ps_inst_t* ps_inst, uint32_t ps_sample_count, ps_aggregate_t ps_aggregate);

uint16_t ps_val_get(ps_inst_t* ps_inst, size_t ps_id);

uint16_t ps_val_get_ambient(ps_inst_t* ps_inst);

uint16_t ps_val_get_min(ps_inst_t* ps_inst);

void ps_rank(ps_inst_t* ps_inst);

size_t ps_rank_get(ps_inst_t* ps_inst, size_t ith);

void ps_calibrate(ps_inst_t* ps_inst, uint32_t ps_sample_count, ps_aggregate_t ps_aggregate,  double sensitivity);

void ps_offset_set(ps_inst_t* ps_inst, size_t ps_id, uint16_t offset);

void ps_sensitivity_set(ps_inst_t* ps_inst, size_t ps_id, double sensitivity);

//void ps_wait_min(ps_inst_t* ps_inst,
//		size_t* ps_ids,
//		size_t ps_count,
//		uint32_t ps_sample_count,
//		ps_aggregate_t ps_aggregate,
//		uint32_t timeout_usecs);

void ps_wait_min(ps_inst_t* ps_inst,
		size_t ps_id,
		uint32_t ps_sample_count,
		ps_aggregate_t ps_aggregate);

void ps_wait_ambient(ps_inst_t* ps_inst,
		size_t* ps_ids,
		size_t ps_count,
		uint32_t ps_sample_count,
		ps_aggregate_t ps_aggregate,
		uint32_t timeout_usecs);
#endif //PHOTOSENSOR_H
