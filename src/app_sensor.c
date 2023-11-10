/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_sensor.h"

static uint16_t samp_buf[BUFFER_SIZE];

/* data of ADC io-channels specified in devicetree. */
static const struct adc_channel_cfg adc_chan0 = {
	.channel_id       = 0, // gets set during init
	.gain             = ADC_GAIN,
	.reference        = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.differential	  = 0,
};

struct adc_sequence sequence = {
		.options     = NULL,				// extra samples and callback
		.channels    = BIT(0),				// bit mask of channels to read
		.buffer = &samp_buf,
		.buffer_size = sizeof(samp_buf),	//buffer size in bytes, not number of samples
		.resolution  = ADC_RESOLUTION,		// desired resolution
		.oversampling = 0,					// don't oversample
		.calibrate = 0						// don't calibrate
	};
	
uint16_t app_adc_stm32_handler(void) {

	int err;
	uint16_t samp_val;

	(void)adc_sequence_init_dt(&adc_chan0, &sequence);

	err = adc_read(&adc_chan0, &sequence);
			if (err < 0) {
				printk("could not read (%d)\n", err);
			}
			if (err == 0) {
				samp_val = samp_buf[0];
			}
			return samp_val;
}