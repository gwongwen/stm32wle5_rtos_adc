/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vref.h"

//  ======== app_stm32_vbat_init ============================================
int8_t app_stm32_vref_init(const struct device *dev)
{
    // getting internal vbat ADC device at GPIO IN5 - PB1
    dev = DEVICE_DT_GET_ONE(st_stm32_vref);

    if (dev == NULL) {
        printk("error: no stm32 vref device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: stm32 vref is not ready\n");
		return 0;
	} else {
        printk("- found device \"%s\", getting vref data\n", dev->name);
    }
    return 0;
}

//  ======== app_stm32_get_vbat =============================================
uint16_t app_stm32_get_vref(const struct device *dev)
{
    struct sensor_value vref;
    uint16_t vref_uint16, vdda;
    int8_t ret = 0;

    // fetching data
    ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) {        
	    printk("error: stm32 vref sample is not up to date\n");
	    return 0;
    }

    // getting channel function
	ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &vref);
    if (ret < 0) {
        printk("error: can't read sensor channels\n");
	    return 0;
    }

    // reference level received and converted from channel get
    // resolution 12bits: 0 to 4095 (uint16)
    // vrefint = 1.212V typ (see datasheet pp 78/149)
    vref_uint16 = (uint16_t)(sensor_value_to_milli(&vref));
    vdda = (4096*1212)/vref_uint16;
    printk("stm32 vdda: %"PRIu16"\n", vdda);
    return vdda;
}
