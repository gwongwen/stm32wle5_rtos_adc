/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "app_bme280.h"
 #include "app_nvs.h"

int8_t app_bme280_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ANY(bosch_bme280);
    if (dev == NULL) {
        printk("error: no bme280 device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: bme280 is not ready\n");
		return 0;
	}

    return 0;
}

int8_t app_bme280_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value temp_raw;
    double temp;
    struct sensor_value hum_raw;
    double hum;
    int8_t ret = 0;

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp_raw);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }
    temp = sensor_value_to_double(&temp_raw);
    nvs_write(&fs, BME280_TEMP_ID, &temp_raw, sizeof(temp_raw));

	ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum_raw);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n");
	    return 0;
    }
    hum = sensor_value_to_double(&hum_raw);
    nvs_write(&fs, BME280_HUM_ID, &hum_raw, sizeof(hum_raw));
    
    printk("temp: %f; humidity: %f\n", temp, hum);

    return 0;
}