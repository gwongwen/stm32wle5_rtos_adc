/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_BME280_H
#define APP_BME280_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

int8_t app_bme280_init(const struct device *dev);
int8_t app_bme280_handler(const struct device *dev);

#endif /* APP_BME280_H */