/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include "app_adc.h"
#include "app_rom.h"

//  ======== interrupt sub-routine ===============================
void adc_work_handler(struct k_work *work_adc)
{
	const struct device *rom_dev = NULL;
	uint8_t payload[2];
	int8_t ret;
	uint16_t adc_val;

	printk("ADC handler called\n");

	payload[0] = adc_val >> 8;
	payload[1] = adc_val;

	ret = app_rom_write(rom_dev, adc_val);
	printk("value write uint16: %d, MSB payload: %d, LSB payload: %d\n", adc_val, payload[0], payload[1]);

	adc_val = app_rom_read(rom_dev);
	printk("value read uint16: %d, MSB payload: %d, LSB payload: %d\n", adc_val, payload[0], payload[1]);
}
K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *adc_dum)
{
    printk("ADC timer called\n");
	k_work_submit(&adc_work);
}
K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *rom_dev = NULL;

	// setup eeprom device
	app_rom_init(rom_dev);

	printk("ADC STM32 Example\nBoard: %s\n", CONFIG_BOARD);

//	k_timer_start(&adc_timer, K_SECONDS(5), K_SECONDS(5));

	return 0;
}