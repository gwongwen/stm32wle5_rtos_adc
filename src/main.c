/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 

#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include "app_rom.h"

void adc_work_handler(struct k_work *work_adc)
{
	const struct device *rom_dev = NULL;
	uint16_t raw_val = app_adc_handler();
	uint8_t payload[2];
	int8_t ret;

	printk("ADC handler called\n");

	payload[0] = raw_val >> 8;
	payload[1] = raw_val;

	ret = app_rom_write(rom_dev, raw_val);
	printk("value write uint16: %d, MSB payload: %d, LSB payload: %d\n", raw_val, payload[0], payload[1]);

	raw_val = app_rom_read(rom_dev);
	printk("value read uint16: %d, MSB payload: %d, LSB payload: %d\n", raw_val, payload[0], payload[1]);
}
K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *adc_dum)
{
    printk("ADC timer called\n");
	k_work_submit(&adc_work);
}
K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

int8_t main(void)
{
	const struct device *rom_dev = NULL;
	app_rom_init(rom_dev);

	printk("ADC STM32 Example\nBoard: %s\n", CONFIG_BOARD);

	k_timer_start(&adc_timer, K_SECONDS(1), K_SECONDS(1));

	return 0;
}