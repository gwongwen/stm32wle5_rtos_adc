/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "app_rom.h"

 int8_t app_rom_init(const struct device *dev)
 {
    dev = DEVICE_DT_GET(DT_ALIAS(eeprom0));

    if (dev = NULL) {
		printk("no eeprom device found. error: %d\n", dev);
		return 0;
	}
    if (!device_is_ready(dev)) {
		printk("eeprom is not ready. error: %d\n", dev);
		return 0;
	}
}