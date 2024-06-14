/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 
#ifndef APP_ROM_H_
#define APP_ROM_H_

//  ======== includes =============================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/flash.h>

//  ======== defines ==============================================
#define ROM_OFFSET              0x00
#define ROM_PAGE_SIZE           512
#define ROM_MAX_RECORDS         3   // 1 structure: ID+value = 3 bytes
                                    // number of structures to store per page
                                            
//  ======== prototypes ===========================================
int8_t app_rom_init(const struct device *dev);
int8_t app_rom_write(const struct device *dev, uint16_t data[]);
int8_t app_rom_read(const struct device *dev);
int8_t app_rom_handler(const struct device *dev);

#endif /* APP_ROM_H_ */