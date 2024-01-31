/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>

#include "app_rom.h"
#include "app_rtc.h"

void adc_work_handler(struct k_work *work_adc)
{
	const struct device *rtc_dev = NULL;
	const struct device *rom_dev = NULL;

	uint16_t raw_val = app_adc_handler();
	uint8_t payload[2];

	printk("ADC handler called\n");

	app_rtc_handler(rtc_dev);
	payload[0] = raw_val >> 8;
	payload[1] = raw_val;
	
	printk("value uint16: %d, MSB payload: %d, LSB payload: %d\n", raw_val, payload[0], payload[1]);
}

K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *adc_dum)
{
    k_work_submit(&adc_work);
}

K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

int main(void)
{
	const struct device *rtc_dev = NULL;
	const struct device *rom_dev = NULL;

	app_rtc_init(rtc_dev);
	app_rom_init(rom_dev);

	printk("beginninig of test\n");

	k_timer_start(&adc_timer, K_MSEC(5000), K_MSEC(5000));

	return 0;
}