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
void geo_work_handler(struct k_work *work_geo)
{
	const struct device *rom_dev = NULL;
	uint8_t payload[2];

	printk("ADC handler called\n");
	app_rom_handler(rom_dev);
}
K_WORK_DEFINE(geo_work, geo_work_handler);

void geo_timer_handler(struct k_timer *geo_dum)
{
    printk("ADC timer called\n");
	k_work_submit(&geo_work);
}
K_TIMER_DEFINE(geo_timer, geo_timer_handler, NULL);

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