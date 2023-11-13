/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_adc.h"

static uint16_t samp_buf[BUFFER_SIZE];

/* data of ADC io-channels specified in devicetree. */
static const struct adc_channel_cfg adc_chan0_cfg = {
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

static const struct device* app_adc_init(void) {

    	int ret;

        const struct device *adc_dev = device_get_binding(DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels));
		if (adc_dev!= NULL) {
            ret = adc_channel_setup(adc_dev, &adc_chan0_cfg);

		    if(ret != 0) {
			//LOG_INF("Setting up of the first channel failed with code %d", ret);
			    adc_dev = NULL;
		    }

        }

        memset(samp_buf, 0, sizeof(samp_buf));
	    return adc_dev;
}
	
uint16_t app_adc_handler(void) {

	int err;
	uint16_t samp_val;

	const struct device *adc_dev = app_adc_init();
	if (adc_dev) {

		err = adc_read(adc_dev, &sequence);
		if (err ==0){
			samp_val = samp_buf[0];
		}
	}

	return samp_val;
} 