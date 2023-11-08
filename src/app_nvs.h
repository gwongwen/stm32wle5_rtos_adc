/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_NVS_H
#define APP_NVS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>

#include <zephyr/fs/nvs.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>

#define NVS_PARTITION			storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define TIMER_RTC_ID            1
#define STM32_VBAT_ID           2
#define BME280_TEMP_ID          3
#define BME280_HUM_ID           4

int app_nvs_init(struct nvs_fs *fs);

#endif /* APP_NVS_H */