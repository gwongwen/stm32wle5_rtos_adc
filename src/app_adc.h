/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_SENSOR_H
#define APP_SENSOR_H

//  ======== includes =============================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

//  ======== defines ==============================================
#define DT_SPEC_AND_COMMA(node_id, prop, idx)   ADC_DT_SPEC_GET_BY_IDX(node_id, idx)

//  ======== prototypes ============================================
uint16_t app_adc_handler(void);
void adc_timer_handler(struct k_timer *adc_dum);
void adc_work_handler(struct k_work *work_adc);

#endif /* APP_SENSOR_H */
