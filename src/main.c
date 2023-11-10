/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "app_bme280.h"
#include "app_nvs.h"
#include "app_rtc_stm32.h"
#include "app_sensor.h"
#include "app_stm32_vbat.h"

void rtc_work_handler(struct k_work *work_rtc)
{
	const struct device *bme280_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *timer_rtc_dev = NULL;

	printk("RTC handler called\n");

	app_rtc_stm32_handler(timer_rtc_dev);
	app_bme280_handler(bme280_dev);
	app_stm32_vbat_handler(bat_dev);
}

K_WORK_DEFINE(rtc_work, rtc_work_handler);

void rtc_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&rtc_work);
}

K_TIMER_DEFINE(rtc_timer, rtc_timer_handler, NULL);

void adc_work_handler(struct k_work *work_adc)
{
	int32_t ret;

	static struct nvs_fs *fs;

	uint16_t gsone_val;
    uint16_t payload[2];

	printk("ADC handler called\n");

	gsone_val = app_adc_stm32_handler();

    payload[1] = gsone_val >> 8;
    payload[2] = gsone_val;

	printk("payload: %d\n", gsone_val);
}

K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *adc_dum)
{
    k_work_submit(&adc_work);
}

K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

int main(void)
{
	static struct nvs_fs fs;
	const struct device *bme280_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *timer_rtc_dev = NULL;
	uint16_t value;

	app_bme280_init(bme280_dev);
	app_nvs_init(&fs);
	app_rtc_stm32_init(timer_rtc_dev);
	app_stm32_vbat_init(bat_dev);
	
	printk("beginninig of test\n");

	k_timer_start(&adc_timer, K_MSEC(5), K_MSEC(5));
	k_timer_start(&rtc_timer, K_MINUTES(30), K_MINUTES(30));

	return 0;
}