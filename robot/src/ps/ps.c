#include <inttypes.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <mss_assert.h>

#include "ps.h"
#include "../sleep/sleep.h"
#include "../clk/clk.h"



void ps_init(ps_inst_t* ps_inst,  char** ps_names, size_t ps_count) {
	ps_inst->ps_count = ps_count;
	ps_inst->ps_vals = (uint16_t*)malloc(ps_count * sizeof(uint16_t));
	ps_inst->ps_offsets = (uint16_t*)malloc(ps_count * sizeof(uint16_t));
	ps_inst->ps_offset_max = 0;
	ps_inst->ps_val_ambient = 0;
	ps_inst->ps_rank = (size_t*)malloc(ps_count * sizeof(size_t));
	ps_inst->ps_samples = (uint16_t**)malloc(ps_count * sizeof(uint16_t*));
	ps_inst->ps_sample_count = 0;
	ps_inst->ace_handlers = (ace_channel_handle_t*)malloc(ps_count * sizeof(ace_channel_handle_t));

	ACE_init();
	size_t i = 0;
	for(; i < ps_count; ++i) {
		ps_inst->ace_handlers[i] = ACE_get_channel_handle((const uint8_t *)ps_names[i]);
		ps_inst->ps_vals[i] = 0;
		ps_inst->ps_rank[i] = 0;
		ps_inst->ps_offsets[i] = 0;
		ps_inst->ps_samples[i] = NULL;
	}
}

void ps_destroy(ps_inst_t* ps_inst) {
	free(ps_inst->ps_vals);
	free(ps_inst->ps_rank);
	free(ps_inst->ps_offsets);
	free(ps_inst->ace_handlers);
	ps_sample_destroy(ps_inst);
	free(ps_inst->ps_samples);
}

int ps_id_inbounds(ps_inst_t* ps_inst, size_t ps_id) {
	return (ps_id < ps_inst->ps_count);
}

size_t ps_id_get_min(ps_inst_t* ps_inst) {
	return ps_rank_get(ps_inst, 0);
}

void ps_sample_init(ps_inst_t* ps_inst, uint32_t ps_sample_count) {
	size_t i = 0;
	for(; i < ps_inst->ps_count; ++i) {
		ps_inst->ps_samples[i] = (uint16_t*)malloc(ps_sample_count * sizeof(uint16_t));
	}
	ps_inst->ps_sample_count = ps_sample_count;
}

void ps_sample_destroy(ps_inst_t* ps_inst) {
	size_t i = 0;
	for(; i < ps_inst->ps_count; ++i) {
		if(ps_inst->ps_samples[i]) {
			free(ps_inst->ps_samples[i]);
			ps_inst->ps_samples[i] = NULL;
		}
	}
	ps_inst->ps_sample_count = 0;
}

void ps_aggregate_mean(ps_inst_t* ps_inst) {
	uint32_t totals[ps_inst->ps_count];
	size_t i, j;
	for(i = 0; i < ps_inst->ps_count; ++i) {
		totals[i] = 0;
		for(j = 0; j < ps_inst->ps_sample_count; ++j) {
			totals[i] += ps_inst->ps_samples[i][j];
		}
		ps_inst->ps_vals[i] = (uint16_t)((double)totals[i] / (double)(ps_inst->ps_sample_count));
	}
}

void ps_aggregate_median(ps_inst_t* ps_inst) {}

void ps_sample(ps_inst_t* ps_inst, uint32_t ps_sample_count, ps_aggregate_t ps_aggregate) {
	size_t i, j;
	ps_sample_destroy(ps_inst);
	ps_sample_init(ps_inst, ps_sample_count);

	uint32_t ps_sample_nsecs = (uint32_t)((double)1E9/(double)clk_get_freq_hz(CLK_ADC));
	for(i = 0; i < ps_sample_count; ++i) {
		for(j = 0; j < ps_inst->ps_count; ++j) {
			uint16_t ace_sample = ACE_get_ppe_sample(ps_inst->ace_handlers[j]);
			//Guarantees result >= 0. Susceptible to overflow if adc returns vals near min of uint16_t
			ps_inst->ps_samples[j][i] = ace_sample + ps_inst->ps_offset_max - ps_inst->ps_offsets[j];
		}
		nsleep(ps_sample_nsecs);
	}
	if(ps_aggregate) {
		ps_aggregate(ps_inst);
	}
	else {
		for(j = 0; j < ps_inst->ps_count; ++j) {
			ps_inst->ps_vals[j]= ps_inst->ps_samples[j][ps_inst->ps_sample_count - 1];
		}
	}
	ps_rank(ps_inst);
}

uint16_t ps_val_get(ps_inst_t* ps_inst, size_t ps_id) {
	ASSERT(ps_id_inbounds(ps_inst, ps_id));
	return ps_inst->ps_vals[ps_id];
}

uint16_t ps_val_get_ambient(ps_inst_t* ps_inst) {
	return ps_inst->ps_val_ambient;
}

uint16_t ps_val_get_min(ps_inst_t* ps_inst) {
	return ps_val_get(ps_inst, ps_rank_get(ps_inst, 0));
}

void ps_rank(ps_inst_t* ps_inst) {

	size_t i, j;
	uint16_t tmp_el1;
	size_t tmp_el2;

	uint16_t tmp_ary[ps_inst->ps_count];
	memcpy(tmp_ary, ps_inst->ps_vals, ps_inst->ps_count * sizeof(uint16_t));

	for(i = 0; i < ps_inst->ps_count; ++i) {
		ps_inst->ps_rank[i] = i;
	}

    for(i = 0; i < ps_inst->ps_count; ++i) {
        for(j = i + 1; j < ps_inst->ps_count; ++j) {
            if(tmp_ary[j] < tmp_ary[i]) {
            	tmp_el1 = tmp_ary[i];
            	tmp_ary[i] = tmp_ary[j];
            	tmp_ary[j] = tmp_el1;

            	tmp_el2 = ps_inst->ps_rank[i];
                ps_inst->ps_rank[i] = ps_inst->ps_rank[j];
                ps_inst->ps_rank[j] = tmp_el2;
            }
        }
    }
}

size_t ps_rank_get(ps_inst_t* ps_inst, size_t ith) {
	return ps_inst->ps_rank[ith];
}

void ps_offset_set(ps_inst_t* ps_inst, size_t ps_id, uint16_t offset) {
	ps_inst->ps_offsets[ps_id] = offset;
}

void ps_sensitivity_set(ps_inst_t* ps_inst, size_t ps_id, double sensitivity) {
	uint16_t offset = (uint16_t)((double)ps_val_get_ambient(ps_inst) * sensitivity);
	ps_offset_set(ps_inst, ps_id, offset);
}

void ps_calibrate(ps_inst_t* ps_inst, uint32_t ps_sample_count, ps_aggregate_t ps_aggregate, double sensitivity) {
	ps_sample(ps_inst, ps_sample_count, ps_aggregate);
	ps_rank(ps_inst);

	uint16_t ps_val_min = ps_val_get_min(ps_inst);

	uint16_t ps_offset_max = 0;
	size_t ps_id = 0;
	for(; ps_id < ps_inst->ps_count; ++ps_id) {
		uint16_t ps_val = ps_val_get(ps_inst, ps_id);
		uint16_t ps_offset = ps_val - ps_val_min;
		if(ps_offset > ps_offset_max) {
			ps_offset_max = ps_offset;
		}
		ps_inst->ps_offsets[ps_id] = ps_offset;
	}
	ps_inst->ps_offset_max = ps_offset_max;
	ps_inst->ps_val_ambient = ps_val_min + ps_offset_max;
	ps_inst->ps_val_ambient = (uint16_t)((double)ps_inst->ps_val_ambient * sensitivity);
}

//void ps_wait_min(ps_inst_t* ps_inst,
//		size_t* ps_ids,
//		size_t ps_count,
//		uint32_t ps_sample_count,
//		ps_aggregate_t ps_aggregate,
//		uint32_t timeout_usecs) {
//	uint16_t ps_val_ambient =  ps_val_get_ambient(ps_inst);
//	timer_set(timeout_usecs, NULL);
//	while(!timer_done()) {
//		ps_sample(ps_inst, ps_sample_count, ps_aggregate);
//		size_t ps_id_min = ps_id_get_min(ps_inst);
//		uint16_t ps_val_min = ps_val_get_min(ps_inst);
//		size_t i = 0;
//		for(;i < ps_count; ++i) {
//			if((ps_id_min == ps_ids[i]) && (ps_val_min < ps_val_ambient)) {
//				break;
//			}
//		}
//	}
//}

void ps_wait_min(ps_inst_t* ps_inst,
		size_t ps_id,
		uint32_t ps_sample_count,
		ps_aggregate_t ps_aggregate) {
	uint16_t ps_val_ambient =  ps_val_get_ambient(ps_inst);
	while(1) {
		ps_sample(ps_inst, ps_sample_count, ps_aggregate);
		size_t ps_id_min = ps_id_get_min(ps_inst);
		uint16_t ps_val_min = ps_val_get_min(ps_inst);
		if((ps_id_min == ps_id) && (ps_val_min < ps_val_ambient)) {
			break;
		}
	}
}

void ps_wait_ambient(ps_inst_t* ps_inst,
		size_t* ps_ids,
		size_t ps_count,
		uint32_t ps_sample_count,
		ps_aggregate_t ps_aggregate,
		uint32_t timeout_usecs)
{
	timer_set(timeout_usecs, NULL);
	uint16_t ps_val_ambient =  ps_val_get_ambient(ps_inst);
	size_t i = 0;
	while(!timer_done()) {
		ps_sample(ps_inst, ps_sample_count, ps_aggregate);
		for(i = 0; i < ps_count; ++i) {
			if(ps_val_get(ps_inst, ps_ids[i]) < ps_val_ambient) {
				break;
			}
		}
	}
}
//void ps_motor_match_pair(ps_inst_t* ps_inst,
//					motor_inst_t* motor_inst_l,
//					motor_inst_t* motor_inst_r,
//					size_t ps_id_l,
//					size_t ps_id_r,
//					uint16_t match_threshold,
//					double kp) {
//	uint16_t ps_val_l = ps_val_get(ps_inst, ps_id_l);
//	uint16_t ps_val_r = ps_val_get(ps_inst, ps_id_r);
//	int err = (int)ps_val_l-(int)ps_val_r;
//	if(abs(err) > match_threshold) {
//		motor_inst_t* motor_inst = (err > 0) ? motor_inst_r : motor_inst_l;
//		size_t ps_id = (err > 0) ? ps_id_r : ps_id_l;
//		double speed = motor_get_speed(motor_inst) + (double)err * kp;
//		if(speed < MIN_SPEED) {
//			speed = MIN_SPEED;
//		}
//		if(speed > MAX_SPEED) {
//			speed = MAX_SPEED;
//		}
//		if(ps_id == PS_FL || ps_id == PS_FR) {
//			motor_set(motor_inst, MOTOR_CMD_FORWARD, speed);
//		}
//		else {
//			motor_set(motor_inst, MOTOR_CMD_BACKWARD, speed);
//		}
//	}
//}
//ps_motor_match_pair(ps_inst, motor_inst_l, motor_inst_r, PS_FL, PS_FR, match_threshold, kp);
