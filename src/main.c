/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 
#include "app_adc.h"
#include "app_vref.h"
#include "app_rom.h"


//  ======== interrupt sub-routine ===============================
void geo_work_handler(struct k_work *work_geo)
{
	const struct device *rom_dev = NULL;

	printk("ADC handler called\n");
	app_rom_handler(rom_dev);
}
K_WORK_DEFINE(geo_work, geo_work_handler);

void geo_timer_handler(struct k_timer *geo_dum)
{
	k_work_submit(&geo_work);
}
K_TIMER_DEFINE(geo_timer, geo_timer_handler, NULL);

//  ======== main ===============================================
int8_t main(void)
{
	const struct device *rom_dev = NULL;
	const struct device *vref_dev = NULL;
	uint16_t vdda;

	// setup all devices
	app_rom_init(rom_dev);
	app_stm32_vref_init(vref_dev);

	printk("ADC STM32 Example\nBoard: %s\n", CONFIG_BOARD);

	// beginning of interrupt subroutine
//	k_timer_start(&geo_timer, K_NO_WAIT, K_MSEC(5000));
	vref_dev = DEVICE_DT_GET_ONE(st_stm32_vref);
	vdda = app_stm32_get_vref(vref_dev);

	while(1) {
		uint16_t val = app_adc_get_val(vdda);

		k_sleep(K_MSEC(1000)); // for the test
	}
	return 0;
}