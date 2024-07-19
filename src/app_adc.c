/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_adc.h"

//  ======== globals ============================================
uint16_t sp_buf;		

static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

struct adc_sequence adc_ch0_seq = {
	.buffer 		= &sp_buf,
	.buffer_size	= sizeof(sp_buf),
	.calibrate 		= true,
};

//  ======== app_adc_get_val ====================================
uint16_t app_adc_get_val(void)
{
	int8_t err;
	int32_t val_mv;

	// getting STM32 ADC device at GPIO IN0 PB13
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
		return 0;
		}
		
		// getting channel setup in devicetree
		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("could not setup channel 0. error: %d\n", err);
			return 0;
		}
	}

	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		// initialization of channel and getting data
		(void)adc_sequence_init_dt(&adc_channels[i], &adc_ch0_seq);

		// getting value and printing numerical data
		// resolution 12 bits: 0 to 4095 (uint16)
		(void)adc_read(adc_channels[i].dev, &adc_ch0_seq);
		val_mv = (int32_t)(sp_buf);
		err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);
		if (err < 0) {
			printk("value in mV not available. error: %d", err);
		} else {
			printk("adc val: %"PRIu16" - adc mv: %"PRId32"\n", sp_buf, val_mv);
		}
	}
	return sp_buf;
}