/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_SENSOR_H
#define APP_SENSOR_H

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#define ADC_RESOLUTION		    12
#define ADC_GAIN			    ADC_GAIN_1
#define ADC_REFERENCE		    ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME	ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 5)
#define BUFFER_SIZE			    6

//#define LOOP_DELAY K_MSEC(60000)
uint16_t app_adc_stm32_handler();

#endif /* APP_SENSOR_H */

