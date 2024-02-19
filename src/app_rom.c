/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_rom.h"

#define LORAWAN_DEV_EUI			{ 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x21, 0xA5 }

//  ======== globals ============================================
uint8_t dev_eui[] = LORAWAN_DEV_EUI;
int32_t times = 12345;

struct eeprom_data {
	uint8_t id;
	int32_t timestamp;
	uint16_t val;
};

int8_t rom_isr_rd_ind;		// index used by Interrupt Service Routine
int8_t rom_isr_wrt_ind;	// index used by Interrupt Service Routine

//  ======== app_rom_init =========================================
 int8_t app_rom_init(const struct device *dev)
 {
    size_t eeprom_size;
	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

    if (dev = NULL) {
		printk("no eeprom device found. error: %d\n", dev);
		return 0;
	}
    if (!device_is_ready(dev)) {
		printk("eeprom is not ready. error: %d\n", dev);
		return 0;
	}

	eeprom_size = eeprom_get_size(dev);
	printk("eeprom size: %zu\n", eeprom_size);

	rom_isr_rd_ind = 0;
	rom_isr_wrt_ind = 0;
	return 0;
}

//  ======== app_rom_write ========================================
int8_t app_rom_write(const struct device *dev, uint16_t adc_val_c)
{
	int8_t ret;
	struct eeprom_data data;

	data.id = dev_eui;
	data.timestamp = times;
	data.val = adc_val_c;

	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

	if (rom_isr_wrt_ind < ADC_BUFFER_SYZE) {
		ret = eeprom_write(dev, EEPROM_SAMPLE_OFFSET+(rom_isr_wrt_ind*136), &data, sizeof(data));
		if (ret  < 0){
				printk("couldn't write eeprom. error: %d\n", ret);
			}
		} else {
			rom_isr_wrt_ind =0 ;
		}
	return 0;
}

//  ======== app_rom_read =========================================
uint16_t app_rom_read(const struct device *dev)
{
	int8_t ret;
	uint16_t data;

	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

	if (rom_isr_rd_ind < ADC_BUFFER_SYZE) {
		ret = eeprom_read(dev, EEPROM_SAMPLE_OFFSET+rom_isr_rd_ind , &data, sizeof(data));
		if (ret  < 0){
			printk("couldn't read eeprom. error: %d\n", ret);
		}
	} else {
		rom_isr_rd_ind = 0;
	}
	return data;
}