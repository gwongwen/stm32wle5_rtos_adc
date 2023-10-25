/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_stm32_vbat.h"
#include "app_nvs.h"

int8_t app_stm32_vbat_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

    if (dev == NULL) {
        printk("error: no stm32 vbat device found\n");
		return -ENODEV;
	}
    if (!device_is_ready(dev)) {
		printk("error: stm32 vbat is not ready\n");
		return 0;
	}
}

int8_t app_stm32_vbat_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value bat_raw;
    double bat;
    int8_t ret = 0;

	sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) {               // data not updated yet
	    printk("error: stm32 vbat sample is not up to date\n");
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &bat_raw);
    if (ret < 0) {
        printk("error: can't read sensor channels\n");
	    return 0;
    }

    bat = sensor_value_to_double(&bat_raw);
    printk("stm32 vbat: %f\n", bat);

    nvs_write(&fs, STM32_VBAT_ID, &bat_raw, sizeof(bat_raw));

    return 0;
}
