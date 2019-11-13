Library to control photosensor(ps) array. Photosensor values are retrieve from MSS ADC inputs using drivers/mss_ace/mss_ace.h.

The ps instance maintains the calibartion offset for each ps, the rank of ps values from smallest to largest,
a history of "ps_num_samples" raw sample values for each ps and a ps_val for each ps resulting from a the 
application fo a user supplied aggregate function to the raw sample values. An aggregate function which calculates
the mean sample value is provided. A median aggregate is specified, but not implemented. If a NULL aggragate is passed,
the first sample value is used.

Our particular application used 6 ps, but the library makes no assumptions about the number or function of ps.

Initialize the ps instance with ps_init

Cleanup dynamically allocated memory for ps_inst using ps_destroy

Get id of the ps with the minimum value based of the most recent sampling with ps_id_get_min

Get the minimum ps value based on the most recent sample with ps_val_get_min

Aggregete functions to be passed into ps_sample:
ps_aggregate_mean(ps_inst_t* ps_inst);
ps_aggregate_median(ps_inst_t* ps_inst);

Sample ps_num_samples times, computer the ps values using the supplied aggregate function,
and rank the ps values from smallest to largest with ps_sample.

Get a particular ps value based on the most recent sample with ps_val_get.

Get the ambient ps value determined by calling ps_calibrate with ps_val_get_ambient.

Rank the ps values from smallest to largest with ps_rank(ps_inst_t* ps_inst). Called by ps_sample.

Get the id of the ps with the ith smalleset value with ps_rank_get.

Compute the ambient ps value and ps value offsets with ps_calibrate. 
Assumes constant abient conditions and must be called while all sensors are exposed to even lighting. 
Proper calibration can compensate for sensitivity differences due sensor placement or use of multiple
sensors with different properties.

Manually set the ps offset with ps_offset_set.

Manually set the ps offset as a percentage of the ps ambient value with ps_sensitivity_set.

Wait for any of several specified ps to report the minimum value with ps_wait_min. A timeout may be specified if desired.

Wait for any of several specified ps to report a value below ambient with ps_wait_ambient. A timeout may be specified if desired.

#endif //PHOTOSENSOR_H
