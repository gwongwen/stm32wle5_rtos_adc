/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_rom.h"

//  ======== globals ============================================
int8_t ind;		// index used by Interrupt Service Routine

//  ======== app_rom_init =========================================
int8_t app_rom_init(const struct device *dev)
{
	int8_t ret;
	ssize_t size, page;

	// getting eeprom device
	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

    if (dev == NULL) {
		printk("no eeprom device found. error: %d\n", dev);
		return 0;
	}
	
	if (!device_is_ready(dev)) {
		printk("eeprom is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", writing/reading data\n", dev->name);
    }

	// getting the size of eeprom in bytes
	size = eeprom_get_size(dev);
	printk("size of eeprom: %zu\n", size);

	// getting the total number of flash pages in bytes
	page = flash_get_page_count(dev);
	printk("number of pages: %zu\n", page);

	// erasing all page at @0x00
	ret  = flash_erase(dev, ROM_OFFSET, 1024*ROM_PAGE_SIZE);
	if (ret!=0) {
		printk("error erasing flash. error: %d\n", ret);
	} else {
		printk("erased all %zu pages\n", page);
	}
	
	// initialisation of isr index
	ind = 0;
	return 0;
}

//  ======== app_rom_write ========================================
int8_t app_rom_write(const struct device *dev, uint16_t data[])
{
	int8_t ret;
	
	// writing data in the first page of 2kbytes
	ret = flash_write(dev, ROM_OFFSET, &data, sizeof(data));

	if (ret!=0) {
		printk("error writing data. error: %d\n", ret);
	} else {
		printk("wrote %zu bytes to address 0x00\n", sizeof(data));
	}

	// printing data
	for (ind = 0; ind < ROM_MAX_RECORDS; ind++) {
		printk("wrt -> rom val: %"PRIu16"\n", data[ind]);
	}
	return 0;
}

//  ======== app_rom_read =========================================
int8_t app_rom_read(const struct device *dev)
{
	int8_t ret;
	uint16_t data[ROM_MAX_RECORDS];

	// reading the first page
	ret = flash_read(dev, ROM_OFFSET, &data, sizeof(data));
	if (ret) {
		printk("error reading data. error: %d\n", ret);
	} else {
		printk("read %zu bytes from address 0x00\n", sizeof(data));
	}

	// reading data
	for (ind = 0; ind < ROM_MAX_RECORDS; ind++) {
		printk("rd -> rom val: %"PRIu16"\n", data[ind]);
	}
	return 0;
}

//  ======== app_rom_handler =======================================
int8_t app_rom_handler(const struct device *dev)
{
	int8_t ret;
	uint16_t data[ROM_MAX_RECORDS];

	// getting eeprom device
	dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

	// putting n structures in fisrt page for this test
	while (ind < ROM_MAX_RECORDS) {
		data[ind] = app_adc_get_val();
		ind++;
	}

	// writing and reading stored data
	app_rom_write(dev, data);
	app_rom_read(dev);

	// cleaning data storage partition
	(void)flash_erase(dev, ROM_OFFSET, 1024*ROM_PAGE_SIZE);
	ind = 0;
	return 0;
}