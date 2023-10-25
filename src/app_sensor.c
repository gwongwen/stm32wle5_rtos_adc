/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_sensor.h"

//uint16_t temp_data[8];
uint16_t temp_data;

/* data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

struct adc_sequence sequence = {
		.buffer = &temp_data,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(temp_data),
	};

int app_sensor_init(void) {
	int err;
	
	/* configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		/*if (!adc_is_ready_dt(&adc_channels[i])) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}*/

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}
	return 0;
}

uint16_t app_sensor_read(void) {
	int err;
	
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			printk("- %s, channel %d: \n",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);
			printk("adc ready\n");

			err = adc_read(&adc_channels[i], &sequence);
			printk("data ready\n");
			if (err < 0) {
				printk("could not read (%d)\n", err);
				continue;
			}
			printk("data saved\n");
		return temp_data;	
	}
}