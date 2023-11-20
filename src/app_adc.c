/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_adc.h"

uint16_t m_sp_buf;

static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

struct adc_sequence adc_ch13_seq = {
	.buffer 		= &m_sp_buf,
	.buffer_size	= sizeof(m_sp_buf),
};

uint16_t app_adc_handler(void) {

	int8_t err;
	int32_t val_mv;

	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
		return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel 13. error: %d\n", err);
			return 0;
		}
	}

	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {

		printk("- %s, channel %d: ",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);

		(void)adc_sequence_init_dt(&adc_channels[i], &adc_ch13_seq);
		err = adc_read(adc_channels[i].dev, &adc_ch13_seq);
		val_mv = (int32_t)m_sp_buf;
		printk("%"PRId32, val_mv);

		err = adc_raw_to_millivolts_dt(&adc_channels[i], &val_mv);
		printk(" = %"PRId32" mV\n", val_mv);
	}
	return m_sp_buf;
}